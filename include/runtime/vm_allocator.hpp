#pragma once

#include "java_base.hpp"
#include "vm_memory.hpp"
#include "java_base.hpp"
#include <cstddef>
#include <string>
#include <type_traits>

namespace rt_jvm_memory {
    template <class T>
    requires std::derived_from<T, JvmMemory<typename T::value_type, T::size>>
    class Allocator {
      protected:
        using memory_type = T;
        typename T::value_type_ptr get_buf() const;
        std::size_t& get_pos() const;

      public:
        virtual typename T::value_type_ptr allocate(std::size_t n) = 0;
        virtual void deallocate(typename T::value_type_ptr addt, std::size_t n) = 0;
    };

    template <class T = char>
    class StringAllocator : public Allocator<StringPoolMemory<>> {
      public:
        using value_type = T;
        T* allocate(std::size_t n) override;
        void deallocate(T* addr, std::size_t n) override;
    };

    using vm_internal_string =
        std::basic_string<char, std::char_traits<char>,
                          StringAllocator<char>>;

    class MetaAllocator : public Allocator<MetaMemory<>> {
      public:
        raw_jvm_type::u1_ptr allocate(std::size_t n) override;
        void deallocate(raw_jvm_type::u1_ptr addr, std::size_t n) override;
        template <class C> C* allocate(std::size_t n = 1) {
            return reinterpret_cast<C*>(allocate(sizeof(C) * n));
        }
    };

    template <class T>
    requires std::derived_from<T, JvmMemory<typename T::value_type, T::size>>
    inline typename T::value_type_ptr Allocator<T>::get_buf() const {
        return memory_type::instance().buffer;
    }

    template <class T>
    requires std::derived_from<T, JvmMemory<typename T::value_type, T::size>>
    inline std::size_t& Allocator<T>::get_pos() const {
        return memory_type::instance().pos;
    }

    template <class T> T* StringAllocator<T>::allocate(std::size_t n) {
        const auto& buf = get_buf();
        auto& pos = get_pos();
        std::size_t require = sizeof(T) * n;
        if (pos + require <= memory_type::size) {
            const auto addr = buf + pos;
            pos += require;
            return addr;
        }
        throw std::bad_alloc();
    }

    template <class T> void StringAllocator<T>::deallocate(T* addr, std::size_t n) {
        // pass
        // the memory of string pool should never be released
    }

    raw_jvm_type::u1_ptr inline MetaAllocator::allocate(std::size_t n) {
        const auto& buf = get_buf();
        auto& pos = get_pos();
        if (pos + n <= memory_type::size) {
            const auto addr = buf + pos;
            pos += n;
            return addr;
        }
        throw std::bad_alloc();
    }

    void inline MetaAllocator::deallocate(raw_jvm_type::u1_ptr addr, std::size_t n) {
        // pass
        // the memory of meta space should never be released
    }
}; // namespace rt_jvm_memory