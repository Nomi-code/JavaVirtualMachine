#pragma once

#include "java_base.hpp"
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstddef>
#include <memory>
#include <mutex>
#include <type_traits>
#include <vector>
#include <ctime>

namespace oop {
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
    using Monitor_ptr = Monitor*;

    enum class GcColor : std::uint8_t {
        White = 0, // 未标记 / 可能回收
        Grey = 1,  // 已发现但未扫描完
        Black = 2  // 已扫描完
    };

    enum class OopType { Raw, Array, Ref };

    struct Markword {
        Monitor_ptr monitor_p;
        GcColor color;
    };

    struct BasicOop {
        Markword word;
    };

    struct Ref {
      private:
        using RawRef = BasicOop*;
        RawRef ptr;

      public:
        explicit Ref(RawRef p = nullptr) noexcept : ptr(p) {
        }

        explicit Ref(raw_jvm_type::u8_ptr addr) noexcept
            : ptr(addr ? reinterpret_cast<RawRef>(addr) : nullptr) {
        }

        static Ref null() noexcept {
            return Ref{};
        }

        static Ref from(Ref rf) noexcept {
            return Ref{rf};
        }

        bool isEmpty() const noexcept {
            return ptr == nullptr;
        }

        explicit operator bool() const noexcept {
            return !isEmpty();
        }
    };

    class Klass;
    using Klass_ptr = Klass*;

    struct InstanceOop : BasicOop {
        Klass_ptr kls_ptr;
        std::byte bytes[0];
    };

    struct ArrayOop : BasicOop {
        Klass_ptr kls_ptr;        
        int length;
        std::byte bytes[0];
    };
}; // namespace oop
