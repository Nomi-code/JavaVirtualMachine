/*
 * C++ Parallel Stop-The-World GC Coordinator Demo
 * ------------------------------------------------
 * - Simplified heap with Objects (vector of pointers)
 * - Multiple mutator threads allocate/link objects
 * - Coordinator triggers STW, parallel mark (tri-color style simplified), then sweep
 * - Cooperative safepoints to stop mutators
 *
 * Build:
 *   g++ -std=c++20 -O2 -pthread gc_demo.cpp -o gc_demo
 * Run:
 *   ./gc_demo
 */

#include <bits/stdc++.h>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <random>
#include <thread>

using namespace std;

struct Object {
    uint64_t id;
    atomic<bool> marked{false};
    vector<Object*> fields; // outgoing references

    explicit Object(uint64_t id_, size_t slots = 2) : id(id_) {
        fields.resize(slots, nullptr);
    }
};

struct Heap {
    mutex mtx;               // protects objects
    vector<Object*> objects; // the heap space
    atomic<uint64_t> next_id{1};

    Object* allocate(size_t slots = 2) {
        auto* obj = new Object(next_id++, slots);
        lock_guard<mutex> lk(mtx);
        objects.push_back(obj);
        return obj;
    }

    size_t size() const {
        // approximate (not thread-safe), good enough for demo
        return objects.size();
    }

    // Called during sweep while STW
    template <class F> void for_each_object(F f) {
        for (auto* o : objects) f(o);
    }

    // Compacts the vector by removing dead pointers (does not move objects)
    void remove_if_dead(function<bool(Object*)> is_dead) {
        objects.erase(remove_if(objects.begin(), objects.end(),
                                [&](Object* o) {
                                    if (is_dead(o)) {
                                        delete o;
                                        return true;
                                    }
                                    return false;
                                }),
                      objects.end());
    }
};

struct MutatorRoot {
    // Each mutator has a root set (locals / stacks). We model as small vector
    vector<Object*> roots;
};

struct Coordinator {
    Heap& heap;
    vector<thread> workers;
    vector<MutatorRoot*>& mutator_roots; // references to per-thread roots

    // Safepoint coordination
    atomic<bool> stw_requested{false};
    atomic<int> mutators_in_safepoint{0};
    int mutator_count;
    mutex safepoint_mtx;
    condition_variable safepoint_cv;

    // Worklist for mark phase
    mutex work_mtx;
    vector<Object*> worklist;

    Coordinator(Heap& h, vector<MutatorRoot*>& roots, int worker_threads)
        : heap(h), mutator_roots(roots), mutator_count((int)roots.size()) {
        for (int i = 0; i < worker_threads; ++i) {
            workers.emplace_back([&, i] { worker_loop(i); });
        }
    }

    ~Coordinator() {
        // Stop workers by posting a special null job (stw_requested=true with empty worklist)
        stw_requested = true; // signal exit
        {
            lock_guard<mutex> lk(work_mtx);
            worklist.clear();
        }
        // Wake workers
        work_cv.notify_all();
        for (auto& t : workers)
            if (t.joinable()) t.join();
    }

    // Public API: trigger a GC cycle (blocks until finished)
    void gc() {
        request_stw();         // 1) stop-the-world
        mark_phase_parallel(); // 2) mark
        sweep_phase();         // 3) sweep
        resume_world();        // 4) resume
    }

  private:
    // ==== STW control ====
    void request_stw() {
        stw_requested.store(true, memory_order_release);
        // Wait until all mutators report safepoint reached
        unique_lock<mutex> lk(safepoint_mtx);
        safepoint_cv.wait(lk, [&] { return mutators_in_safepoint.load() == mutator_count; });
    }

    void resume_world() {
        stw_requested.store(false, memory_order_release);
        // reset counter for next GC
        mutators_in_safepoint.store(0);
        // No explicit CV here; mutators spin on stw flag in this demo
    }

  public:
    // Mutators call this periodically to cooperate with safepoints
    void reach_safepoint_blocking() {
        if (stw_requested.load(memory_order_acquire)) {
            // Announce arrival
            int prev = mutators_in_safepoint.fetch_add(1);
            if (prev + 1 == mutator_count) {
                // Last mutator arrived; wake coordinator
                safepoint_cv.notify_one();
            }
            // Busy-wait until resume (simple for demo; real systems would park)
            while (stw_requested.load(memory_order_acquire)) {
                this_thread::yield();
            }
        }
    }

  private:
    // ==== Parallel Mark ====
    condition_variable work_cv;

    void push_work(Object* o) {
        if (!o) return;
        // CAS mark before enqueue to avoid duplicates
        bool expected = false;
        if (o->marked.compare_exchange_strong(expected, true, memory_order_acq_rel)) {
            lock_guard<mutex> lk(work_mtx);
            worklist.push_back(o);
            work_cv.notify_one();
        }
    }

    Object* pop_work() {
        lock_guard<mutex> lk(work_mtx);
        if (worklist.empty()) return nullptr;
        Object* o = worklist.back();
        worklist.pop_back();
        return o;
    }

    void mark_phase_parallel() {
        // Seed roots
        {
            lock_guard<mutex> lk(work_mtx);
            worklist.clear();
        }
        for (auto* mr : mutator_roots) {
            for (auto* r : mr->roots) push_work(r);
        }
        // Wake all workers and wait until queue drains
        // Simple drain detection: poll until both queue empty and no worker is active.
        // We'll use an atomic active counter.
        active_workers.store(0);

        // Wait until the mark queue is empty and no worker is active
        while (true) {
            // If there is no work and no one active, we're done
            if (is_mark_done()) break;
            // Nudge sleepers
            work_cv.notify_all();
            this_thread::sleep_for(chrono::milliseconds(1));
        }
    }

    atomic<int> active_workers{0};

    bool is_mark_done() {
        // check empty queue
        {
            lock_guard<mutex> lk(work_mtx);
            if (!worklist.empty()) return false;
        }
        return active_workers.load() == 0;
    }

    void worker_loop(int /*id*/) {
        // Workers live forever; do mark work only during GC when stw_requested==true
        while (true) {
            // wait for work or exit
            unique_lock<mutex> lk(work_mtx);
            work_cv.wait(lk, [&] { return stw_requested.load() || !worklist.empty(); });
            if (stw_requested.load() && worklist.empty()) {
                // Could be shutdown if destructor set stw_requested true and cleared worklist
                // Keep spinning until destructor joins us
            }
            lk.unlock();

            // Pop and process tasks while available
            Object* o = nullptr;
            while ((o = pop_work()) != nullptr) {
                active_workers.fetch_add(1);
                // scan children
                for (auto* c : o->fields) {
                    if (c) push_work(c);
                }
                active_workers.fetch_sub(1);
            }
        }
    }

    // ==== Sweep ====
    void sweep_phase() {
        // Iterate heap; free unmarked, clear mark from survivors
        heap.remove_if_dead([&](Object* o) {
            bool dead = !o->marked.load(memory_order_acquire);
            if (!dead) {
                o->marked.store(false, memory_order_release); // clear for next cycle
            }
            return dead;
        });
    }
};

// === Demo program ===
struct Mutator {
    Heap& heap;
    Coordinator& coord;
    MutatorRoot root;
    atomic<bool> running{true};
    thread th;

    Mutator(Heap& h, Coordinator& c) : heap(h), coord(c) {
    }

    void start(int id) {
        th = thread([&, id] { run(id); });
    }

    void stop() {
        running.store(false);
        if (th.joinable()) th.join();
    }

    void run(int id) {
        mt19937_64 rng(id * 1469598103934665603ULL);
        uniform_int_distribution<int> slot_dist(0, 1);
        uniform_int_distribution<int> choice(0, 9);

        // Create an initial root
        root.roots.push_back(heap.allocate());

        while (running.load()) {
            // Periodic safepoint check (cooperative)
            coord.reach_safepoint_blocking();

            int op = choice(rng);
            if (op < 6) {
                // allocate and link from some root
                Object* o = heap.allocate();
                // link from a random root or random existing object in root graph
                Object* parent = root.roots[0];
                int slot = slot_dist(rng);
                parent->fields[slot] = o;
            } else if (op < 8) {
                // mutate: relink a field to another existing object (if available)
                Object* parent = root.roots[0];
                if (heap.size() > 2) {
                    // pick a random object from heap (approximate, unsafe read OK for demo)
                    size_t idx = rng() % heap.size();
                    Object* target = nullptr;
                    {
                        // Non-owning read; data race acceptable for demo simplicity
                        target = heap.objects[idx];
                    }
                    parent->fields[slot_dist(rng)] = target;
                }
            } else {
                // occasionally drop references from root to simulate garbage
                root.roots[0]->fields[slot_dist(rng)] = nullptr;
            }

            // Do some work
            this_thread::sleep_for(chrono::milliseconds(2));
        }
    }
};

int main() {
    Heap heap;

    // Prepare mutator roots container for coordinator visibility
    vector<MutatorRoot*> roots;

    // Create a placeholder to pass to Coordinator; we will fill roots after mutators are created
    // We'll start with 3 mutators and 4 GC worker threads
    // We temporarily place dummy vector; we'll update after constructing mutators
    vector<Mutator*> muts;

    // We'll create dummy roots first to construct coordinator
    // (We will replace pointers after mutators are constructed)
    for (int i = 0; i < 3; ++i) roots.push_back(new MutatorRoot());

    Coordinator coord(heap, roots, /*worker_threads=*/4);

    // Now construct mutators and wire real roots
    muts.reserve(3);
    for (int i = 0; i < 3; ++i) {
        muts.push_back(new Mutator(heap, coord));
    }
    for (int i = 0; i < 3; ++i) {
        roots[i] = &muts[i]->root; // expose real root to coordinator
    }

    for (int i = 0; i < 3; ++i) muts[i]->start(i + 1);

    // GC manager thread: trigger GC when heap grows beyond threshold
    atomic<bool> running{true};
    thread gc_thread([&] {
        const size_t threshold = 2000; // objects
        size_t cycle = 0;
        while (running.load()) {
            if (heap.size() > threshold) {
                auto t0 = chrono::steady_clock::now();
                coord.gc();
                auto t1 = chrono::steady_clock::now();
                auto ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
                cout << "GC cycle " << (++cycle) << " done. Heap size: " << heap.size()
                     << ", time: " << ms << " ms\n";
            }
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    });

    // Let the system run for a while
    this_thread::sleep_for(chrono::seconds(5));

    // Stop
    running.store(false);
    if (gc_thread.joinable()) gc_thread.join();
    for (auto* m : muts) {
        m->stop();
        delete m;
    }

    // Final GC to clean remaining
    coord.gc();
    cout << "Final heap size: " << heap.size() << "\n";

    // Cleanup placeholder roots created for coordinator (structs only, not objects)
    // They point to mutators' roots; no dynamic allocation to free here beyond vector

    return 0;
}
