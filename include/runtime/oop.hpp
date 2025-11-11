#pragma once

#include "java_base.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <ctime>

enum class OopColor { WHITE, GREY, BLACK };

class Monitor {
  private:
    std::recursive_mutex rmtx;
    std::condition_variable cv;

  public:
    void enter() {
        rmtx.lock();
    }
    void exit() {
        rmtx.unlock();
    }
    void wait() {
        std::unique_lock<std::recursive_mutex> rul(rmtx);
    }
    void wait(std::chrono::milliseconds duration) {
        std::unique_lock<std::recursive_mutex> rul(rmtx);
    }
    void notify_one() {
        cv.notify_one();
    }
    void notify_all() {
        cv.notify_all();
    }
};

struct Oop {
    OopColor mark;
    std::vector<Oop*> references;
    std::vector<raw_jvm_type::u8> data;

    Oop() {
        if (mark_request.load(std::memory_order_acquire)) {
            mark = OopColor::BLACK;
        } else {
            mark = OopColor::WHITE;
        }
    }

    void mark_white() {
        mark = OopColor::WHITE;
    }

    void mark_black() {
        mark = OopColor::BLACK;
    }

    void mark_grey() {
        mark = OopColor::GREY;
    }
};
