#pragma once

#include <string>
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <thread>
#include <algorithm>
#include <cassert>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <shared_mutex>

namespace raw_jvm_type {
    using u1 = unsigned char;
    using u2 = unsigned short;
    using u4 = unsigned int;
    using u8 = unsigned long long;
    using u1_ptr = u1*;
    using u2_ptr = u2*;
    using u4_ptr = u4*;
    using u8_ptr = u8*;

    constexpr u1 u1_max = (u1)-1;
    constexpr u2 u2_max = (u2)-1;
    constexpr u4 u4_max = (u4)-1;
    constexpr u8 u8_max = (u8)-1;

    template <class T>
    concept JvmWord = std::same_as<std::remove_cvref_t<T>, raw_jvm_type::u1> ||
        std::same_as<std::remove_cvref_t<T>, raw_jvm_type::u2> ||
        std::same_as<std::remove_cvref_t<T>, raw_jvm_type::u4> ||
        std::same_as<std::remove_cvref_t<T>, raw_jvm_type::u8>;
} // namespace raw_jvm_type

class Printable {
  public:
    virtual void print() const = 0;
};

inline std::atomic<bool> mark_request{false};