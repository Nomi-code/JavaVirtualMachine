#pragma once

#include "utils/singleton.hpp"
#include "runtime/oop.hpp"
#include "java_base.hpp"

class JavaThread;

class ThreadRegistry : public Singleton<ThreadRegistry> {
  private:
    std::unordered_map<std::thread::id, std::weak_ptr<JavaThread>> threads;
    std::mutex mtx;

  public:
    void register_thread(const std::shared_ptr<JavaThread>& thread);
    void unregister_thread(const std::shared_ptr<JavaThread>& thread);
    void unregister_thread(const std::thread::id& tid);
    std::vector<std::shared_ptr<JavaThread>> snapshot();
};

class JavaThread {
    using Entry = std::function<void()>;

  private:
    std::vector<Oop*> root;
    std::thread::id tid;
    std::unique_ptr<std::thread> thread_instance;
    Entry entry;

  public:
    explicit JavaThread(Entry&& entry);
    void join();
    void detach();
    std::thread::id thread_id() const;
    std::vector<Oop*>& thread_gc_root();
};

class GcCoordinate : public Singleton<GcCoordinate> {
  private:
    std::atomic<bool> stw_requested;
    std::atomic<int> threads_in_safepoint;
    std::condition_variable safepoint_cv;
    std::mutex safepoint_mtx;
    std::mutex thread_life_mtx;
    std::condition_variable thread_life_cv;
    std::queue<Oop*> worklist;
    std::atomic<bool> mark_request;

  public:
    GcCoordinate(); // 在 .cpp 里初始化原子量
    
    void request_stw();
    void resume_world();
    void enter_safepoint();

    void collect_gcroots();
    void scan();
    void enter_marking_stage();
    void exit_marking_stage();
    void sweep();
    void gc();
};
