#include "classFile/class_file.hpp"
#include "runtime/gc.hpp"
#include "runtime/string_pool.hpp"
#include <limits>
#include <memory>
#include <numeric>
#include <thread>

void thread_task() {
    auto& tr = ThreadRegistry::instance();
    auto& gr = GcCoordinate::instance();
    gr.create_thread();
    for (int i = 0; i < std::numeric_limits<int>::max(); i++) {
        if (i % std::numeric_limits<int>::max() / 2 == 0) {
            auto& gr = GcCoordinate::instance();
            gr.enter_safepoint();
        }
    }
    gr.destroy_thread();
}

void gc_task() {
    auto& gr = GcCoordinate::instance();
    gr.gc();
}

int main(void) {
    // JavaThread jth(thread_task);
    JavaThread gc(gc_task);
    gc.join();
    return 0;
}