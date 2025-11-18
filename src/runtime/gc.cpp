#include "runtime/gc.hpp"
#include <mutex>
#include <stop_token>
#include <thread>

std::list<VmThread_ptr> VmThread::registered_threads;
std::mutex VmThread::register_threads_access_mtx;

void VmThread::register_thread() noexcept {
    std::lock_guard<std::mutex> lk(register_threads_access_mtx);
    registered_threads.push_back(this);
}

void VmThread::unregister_thread() noexcept {
    std::lock_guard<std::mutex> lk(register_threads_access_mtx);
    auto iter = std::find(registered_threads.begin(), registered_threads.end(), this);
    registered_threads.erase(iter);
}

VmThread_ptr VmThread::new_vmthread() {
    VmThread_ptr vmth = new VmThread();
    vmth->state_ = State::New;
    vmth->jth = std::jthread([vmth](std::stop_token token) { vmth->run(token); });
    vmth->jth.detach();
    return vmth;
}

VmThread::State VmThread::state() const noexcept {
    return state_;
}

std::vector<VmThread_ptr> VmThread::snapshot() noexcept {
    std::unique_lock<std::mutex> lg(register_threads_access_mtx);
    std::vector<VmThread_ptr> shot;
    shot.reserve(registered_threads.size());
    for (auto th : registered_threads) {
        if (th->state() != VmThread::State::Terminated) {
            shot.emplace_back(th);
        }
    }
    return shot;
}

void VmThread::vmthread_task() {
    // TODO
    while (!token_.stop_requested()) {
    }
}

void VmThread::run(std::stop_token stoken) {
    register_thread();
    token_ = stoken;
    state_ = State::Runnable;

    vmthread_task();
    
    state_ = State::Terminated;
    unregister_thread();
}

// // JavaThread
// JavaThread::JavaThread(Entry&& e) : entry(std::move(e)) {
//     // 立即启动线程；拿到 thread id
//     thread_instance = std::make_unique<std::thread>(std::thread(entry));
//     tid = thread_instance->get_id();
// }

// void JavaThread::join() {
//     if (thread_instance && thread_instance->joinable()) {
//         thread_instance->join();
//     }
// }

// void JavaThread::detach() {
//     if (thread_instance && thread_instance->joinable()) {
//         thread_instance->detach();
//     }
// }

// std::thread::id JavaThread::thread_id() const {
//     return tid;
// }

// std::vector<Oop*>& JavaThread::thread_gc_root() {
//     return root;
// }

// // ThreadRegistry
// void ThreadRegistry::register_thread(const std::shared_ptr<JavaThread>& thread) {
//     std::lock_guard<std::mutex> lg(mtx);
//     auto tid = thread->thread_id();
//     assert(!threads.count(tid));
//     threads[tid] = thread;
// }

// void ThreadRegistry::unregister_thread(const std::shared_ptr<JavaThread>& thread) {
//     unregister_thread(thread->thread_id());
// }

// void ThreadRegistry::unregister_thread(const std::thread::id& tid) {
//     std::lock_guard<std::mutex> lg(mtx);
//     threads.erase(tid);
// }

// std::vector<std::shared_ptr<JavaThread>> ThreadRegistry::snapshot() {
//     std::lock_guard<std::mutex> lg(mtx);
//     std::vector<std::shared_ptr<JavaThread>> shot;
//     shot.reserve(threads.size());

//     for (auto it = threads.begin(); it != threads.end();) {
//         if (auto sp = it->second.lock()) {
//             shot.push_back(std::move(sp));
//             ++it;
//         } else {
//             it = threads.erase(it);
//         }
//     }
//     return shot;
// }

// // GcCoordinate
// GcCoordinate::GcCoordinate()
//     : stw_requested(false), threads_in_safepoint(0), mark_request(false) {
// }

// void GcCoordinate::request_stw() {
//     // 阻断线程创建/销毁
//     stw_requested.store(true, std::memory_order_release);

//     // 固定一个线程快照作为本轮 safepoint 目标
//     auto& tr = ThreadRegistry::instance();
//     auto shot = tr.snapshot();

//     // 归零计数，避免累加脏值
//     threads_in_safepoint.store(0, std::memory_order_relaxed);

//     // 等待所有线程进入 safepoint
//     std::unique_lock<std::mutex> ul(safepoint_mtx);
//     safepoint_cv.wait(ul, [&] {
//         return this->threads_in_safepoint.load(std::memory_order_acquire) ==
//                static_cast<int>(shot.size());
//     });
// }

// void GcCoordinate::resume_world() {
//     stw_requested.store(false, std::memory_order_release);
//     threads_in_safepoint.store(0, std::memory_order_relaxed);
//     safepoint_cv.notify_all();
//     thread_life_cv.notify_all();
// }

// void GcCoordinate::enter_safepoint() {
//     if (!stw_requested.load(std::memory_order_acquire)) return;
//     threads_in_safepoint.fetch_add(1, std::memory_order_acq_rel);
//     safepoint_cv.notify_one();
//     std::unique_lock<std::mutex> ul(safepoint_mtx);
//     safepoint_cv.wait(ul, [&] { return !stw_requested.load(std::memory_order_acquire); });
// }

// void GcCoordinate::collect_gcroots() {
//     auto& tr = ThreadRegistry::instance();
//     const auto snapshot = tr.snapshot();
//     for (auto& jthread : snapshot) {
//         for (const auto& oop : jthread->thread_gc_root()) {
//             if (!oop) continue;
//             worklist.push(oop);
//             oop->mark_grey();
//         }
//     }
// }

// void GcCoordinate::scan() {
//     while (!worklist.empty()) {
//         Oop* oop = worklist.front();
//         worklist.pop();
//         if (!oop) continue;

//         // 扫描出边：白 -> 灰（入队）
//         for (auto* child : oop->references) {
//             if (child && child->mark == OopColor::WHITE) {
//                 child->mark_grey();
//                 worklist.push(child);
//             }
//         }
//         // 当前对象完成扫描 -> 黑
//         oop->mark_black();
//     }
// }

// void GcCoordinate::enter_marking_stage() {
//     mark_request.store(true, std::memory_order_release);
// }

// void GcCoordinate::exit_marking_stage() {
//     mark_request.store(false, std::memory_order_release);
// }

// void GcCoordinate::sweep() {
//     // TODO: 与你的堆/分配器对接清扫逻辑
// }

// void GcCoordinate::gc() {
//     // Stage 1: Initial mark (STW)
//     request_stw();
//     collect_gcroots();
//     enter_marking_stage();
//     resume_world();

//     // 并发或串行扫描（当前实现是串行）
//     scan();

//     // Stage 2: Remark + Sweep (STW)
//     request_stw();
//     exit_marking_stage();
//     sweep();
//     resume_world();
// }
