#pragma once

#include <spdlog/spdlog.h>
#include <string>

#include "java_base.hpp"

namespace rt_jvm_memory {
    // 32 MB
    constexpr std::size_t STATIC_ALLOCATOR_META_SPACE_SIZE = 1024 * 1024 * 64;

    template <typename T> class StaticSpaceAllocator {
      public:
        using value_type = T;
        using value_type_ptr = value_type*;

      protected:
        static std::byte meta_space[STATIC_ALLOCATOR_META_SPACE_SIZE];
        static std::size_t pos;
        static value_type_ptr allocate_impl(std::size_t n) {
            n = n * sizeof(value_type);
            if (pos + n <= STATIC_ALLOCATOR_META_SPACE_SIZE) {
                const auto& ret_addr = reinterpret_cast<value_type_ptr>(meta_space + pos);
                pos += n;
                return ret_addr;
            }
            throw std::bad_alloc();
        }

      public:
        StaticSpaceAllocator() = default;

        static value_type_ptr allocate_static(std::size_t n) {
            return allocate_impl(n);
        }

        value_type_ptr allocate(std::size_t n) {
            return allocate_impl(n);
        }

        void deallocate(value_type_ptr addr, std::size_t n) {
            // pass
        }
    };

    template <typename T> std::byte StaticSpaceAllocator<T>::meta_space[STATIC_ALLOCATOR_META_SPACE_SIZE] = {};
    template <typename T> std::size_t StaticSpaceAllocator<T>::pos = 0;

    using static_string =
        std::basic_string<char, std::char_traits<char>, StaticSpaceAllocator<char>>;
}; // namespace rt_jvm_memory
