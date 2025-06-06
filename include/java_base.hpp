#pragma once

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
} // namespace raw_jvm_type

class Printable {
  public:
    virtual void print() const = 0;
};