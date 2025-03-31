#pragma once

#include "java_base.hpp"
#include "utils/singleton.hpp"
#include "utils/spin_lock.hpp"

#include <string_view>
#include <type_traits>
#include <unordered_set>

namespace rt_jvm_memory {
    namespace default_config {
        constexpr std::size_t STRING_POOL_SIZE = 1024 * 1024 * 64;
        constexpr std::size_t META_SPACE_SIZE = 1024 * 1024 * 64;
    }; // namespace default_config

    template <typename T, std::size_t N>
    requires std::is_trivial_v<T>
    class JvmMemory : public Singleton<JvmMemory<T, N>> {
        template <class U>
        requires std::derived_from<U, JvmMemory<typename U::value_type, U::size>>
        friend class Allocator;

        friend class Singleton<JvmMemory<T, N>>;

      public:
        static constexpr std::size_t size = N;
        using value_type = T;
        using value_type_ptr = T*;

        JvmMemory(JvmMemory&& other) = delete;
        JvmMemory(const JvmMemory& other) = delete;
        JvmMemory<T, N> operator=(const JvmMemory<T, N>& other) = delete;

      protected:
        std::size_t pos;
        value_type buffer[N];

        JvmMemory() = default;
        ~JvmMemory() = default;
    };

    template <std::size_t N = default_config::STRING_POOL_SIZE>
    class StringPoolMemory : public JvmMemory<char, N> {
        template <class U>
        requires std::derived_from<U, JvmMemory<typename U::value_type, U::size>>
        friend class Allocator;

      private:
        std::unordered_set<std::string_view> strings_rec;
        mutable SpinLock lock;
      public:
        void add_record(const std::string_view&);
        bool contains(const std::string_view&) const;
    };

    template <std::size_t N = default_config::META_SPACE_SIZE>
    class MetaMemory : public JvmMemory<raw_jvm_type::u1, N> {
        template <class U>
        requires std::derived_from<U, JvmMemory<typename U::value_type, U::size>>
        friend class Allocator;

      private:
        
      public:
    };

}; // namespace rt_jvm_memory
