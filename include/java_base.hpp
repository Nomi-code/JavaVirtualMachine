#ifndef JAVA_BASE
#define JAVA_BASE

#pragma once

namespace jvm_raw_type {
using u1 = unsigned char;
using u2 = unsigned short;
using u4 = unsigned int;
using u8 = unsigned long long;
constexpr u1 u1_max = (u1)-1;
constexpr u2 u2_max = (u2)-1;
constexpr u4 u4_max = (u4)-1;
constexpr u8 u8_max = (u8)-1;
} // namespace jvm_raw_type

class Printable {
  public:
    virtual void print() = 0;
};

#endif