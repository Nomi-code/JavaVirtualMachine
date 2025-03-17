#include <spdlog/spdlog.h>
#include <string>

#include "java_base.hpp"

namespace rt_jvm_memory {
    // 32 MB
    constexpr std::size_t STATIC_ALLOCATOR_BUF_SIZE = 1024 * 1024 * 32;

    template <typename T> class StaticSpaceAllocator {
      public:
        using value_type = T;
        using value_type_ptr = value_type*;
      protected:
        static std::byte buffer[STATIC_ALLOCATOR_BUF_SIZE];
        static std::size_t pos;
        static value_type_ptr allocate_impl(const std::size_t& n) {
            if (pos + n <= STATIC_ALLOCATOR_BUF_SIZE) {
                const auto& ret_addr = reinterpret_cast<value_type_ptr>(buffer + pos);
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

    template <typename T> std::byte StaticSpaceAllocator<T>::buffer[STATIC_ALLOCATOR_BUF_SIZE];
    template <typename T> std::size_t StaticSpaceAllocator<T>::pos = 0;

    using static_string =
        std::basic_string<char, std::char_traits<char>, StaticSpaceAllocator<char>>;
}; // namespace rt_jvm_memory
