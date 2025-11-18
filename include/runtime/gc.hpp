#pragma once

#include "runtime/klass.hpp"
#include "utils/singleton.hpp"
#include "runtime/oop.hpp"
#include "java_base.hpp"
#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <mutex>
#include <stop_token>
#include <thread>
#include <type_traits>
#include <list>

class ThreadRegistry;
class StackFrame;
class JavaThread;
class VmThread;

using VmThread_ptr = VmThread*;
using jthread_ptr = std::jthread*;

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

struct Slot {
    raw_jvm_type::u4 raw{0};

    Slot(raw_jvm_type::u4 v = 0) : raw(v) {
    }

    template <class T> void write(const T& v) noexcept {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_trivially_copyable_v<U>, "T must be trivially copyable");

        if constexpr (std::is_same_v<U, raw_jvm_type::u4>) {
            raw = std::bit_cast<raw_jvm_type::u4>(v);
        } else if constexpr (std::is_same_v<U, raw_jvm_type::u2>) {
            raw = static_cast<raw_jvm_type::u4>(std::bit_cast<raw_jvm_type::u2>(v));
        } else if constexpr (std::is_same_v<U, raw_jvm_type::u1>) {
            raw = static_cast<raw_jvm_type::u4>(std::bit_cast<raw_jvm_type::u1>(v));
        } else {
            static_assert(!sizeof(U), "Slot::write only supports u1/u2/u4");
        }
    }

    template <class T> constexpr T read() const noexcept {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_trivially_copyable_v<U>, "T must be trivially copyable");

        if constexpr (std::is_same_v<U, raw_jvm_type::u4>) {
            return std::bit_cast<U>(raw);
        } else if constexpr (std::is_same_v<U, raw_jvm_type::u2>) {
            return std::bit_cast<U>(static_cast<raw_jvm_type::u2>(raw & 0xFFFFu));
        } else if constexpr (std::is_same_v<U, raw_jvm_type::u1>) {
            return std::bit_cast<U>(static_cast<raw_jvm_type::u1>(raw & 0xFFu));
        } else {
            static_assert(!sizeof(U), "Slot::read only supports u1/u2/u4");
        }
    }
};

class StackFrame {
  private:
    struct OperandStack {
        explicit OperandStack(int sz_bytes) : size(sz_bytes), top(0) {
            // 用 max_align_t 保证对齐
            data = static_cast<std::byte*>(
                ::operator new (sz_bytes, std::align_val_t{alignof(std::max_align_t)}));
        }

        ~OperandStack() {
            ::operator delete (data, std::align_val_t{alignof(std::max_align_t)});
        }

        OperandStack(const OperandStack&) = delete;
        OperandStack& operator=(const OperandStack&) = delete;

        template <class T> void push(T v) {
            static_assert(std::is_trivially_copyable_v<T>);
            std::size_t need = sizeof(T);
            assert(top + need <= size);

            std::memcpy(data + top, &v, need);
            top += need;
        }

        template <class T> T pop() {
            static_assert(std::is_trivially_copyable_v<T>);
            std::size_t need = sizeof(T);
            assert(top >= need);

            top -= need;
            T v{};
            std::memcpy(&v, data + top, need);
            return v;
        }

      private:
        int size;
        int top;
        std::byte* data;
    };

    raw_jvm_type::u4 pc{};
    raw_jvm_type::u2 max_locals;
    raw_jvm_type::u2 max_stack;
    std::vector<Slot> slots;

    OperandStack op_stack;
    const rt_jvm_data::InstanceKlass& kls;
    oop::Ref jvm_thread;

  public:
    explicit StackFrame(const raw_jvm_type::u2 max_locals_, const raw_jvm_type::u2 max_stacks_,
                        const rt_jvm_data::InstanceKlass& kls_, oop::Ref jvm_thread_)
        : pc(0), max_locals(max_locals_), max_stack(max_stacks_),
          slots(static_cast<int>(max_locals_)), op_stack(max_stacks_), kls(kls_),
          jvm_thread(jvm_thread_) {
    }

    template <raw_jvm_type::JvmWord T> T read(int index) const noexcept {
        using U = std::remove_cvref_t<T>;
        if constexpr (std::is_same_v<U, raw_jvm_type::u8>) {
            const auto& lo = slots[index + 0];
            const auto& hi = slots[index + 1];
            raw_jvm_type::u8 lo32 = lo.read<raw_jvm_type::u4>();
            raw_jvm_type::u8 hi32 = hi.read<raw_jvm_type::u4>();
            return static_cast<U>((hi32 << 32) | lo32);
        } else {
            return slots[index].template read<U>();
        }
    }

    template <raw_jvm_type::JvmWord T> void write(const T& value, int index) noexcept {
        using U = std::remove_cvref_t<T>;
        if constexpr (std::is_same_v<U, raw_jvm_type::u8>) {
            raw_jvm_type::u4 lo32 = static_cast<raw_jvm_type::u4>(value & 0xFFFFFFFFull);
            raw_jvm_type::u4 hi32 = static_cast<raw_jvm_type::u4>((value >> 32) & 0xFFFFFFFFull);
            slots[index + 0].write<raw_jvm_type::u4>(lo32);
            slots[index + 1].write<raw_jvm_type::u4>(hi32);
        } else {
            slots[index].template write<U>(value);
        }
    }

    template <raw_jvm_type::JvmWord T> void push(const T t) {
        op_stack.push(t);
    }

    template <raw_jvm_type::JvmWord T> T pop() {
        return op_stack.pop<T>();
    }
};

class VmThread {
    enum class State { New, Runnable, Blocked, Terminated };

  private:
    std::vector<StackFrame> stack_;
    std::jthread jth;
    oop::InstanceOop mirror_oop;
    State state_;

    void register_thread() noexcept;
    void unregister_thread() noexcept;
    void vmthread_task();

  public:
    static std::list<VmThread_ptr> registered_threads;
    static std::mutex register_threads_access_mtx;
    static VmThread_ptr new_vmthread();

    static std::vector<VmThread_ptr> snapshot() noexcept;
    
    std::stop_token token_;

    State state() const noexcept;
    void run(std::stop_token stoken);
};

// class GcCoordinate : public Singleton<GcCoordinate> {
//   private:
//     std::atomic<bool> stw_requested;
//     std::atomic<int> threads_in_safepoint;
//     std::condition_variable safepoint_cv;
//     std::mutex safepoint_mtx;
//     std::mutex thread_life_mtx;
//     std::condition_variable thread_life_cv;
//     std::queue<oop::Ref> worklist;
//     std::atomic<bool> mark_request;

//   public:
//     GcCoordinate();

//     void request_stw();
//     void resume_world();
//     void enter_safepoint();

//     void collect_gcroots();
//     void scan();
//     void enter_marking_stage();
//     void exit_marking_stage();
//     void sweep();
//     void gc();
// };
