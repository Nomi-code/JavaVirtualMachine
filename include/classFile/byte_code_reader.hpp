#pragma once

#include <fstream>
#include <filesystem>
#include <type_traits>
#include <bit>

#include "../java_base.hpp"

class ByteCodeReader {
  private:
    std::fstream& file_binary_resource;

  public:
    ByteCodeReader() = delete;
    ByteCodeReader(const ByteCodeReader&) = delete;
    ByteCodeReader& operator=(const ByteCodeReader&) = delete;
    ~ByteCodeReader() = default;
    explicit ByteCodeReader(std::fstream& fms) : file_binary_resource(fms) {
    }

    void read_u1(raw_jvm_type::u1* const);
    void read_u2(raw_jvm_type::u2* const);
    void read_u4(raw_jvm_type::u4* const);
    void read_u8(raw_jvm_type::u8* const);
};

inline void ByteCodeReader::read_u1(raw_jvm_type::u1* const u1_addr) {
    this->file_binary_resource.read(reinterpret_cast<char*>(u1_addr), sizeof(raw_jvm_type::u1));
}

inline void ByteCodeReader::read_u2(raw_jvm_type::u2* const u2_addr) {
    this->file_binary_resource.read(reinterpret_cast<char*>(u2_addr), sizeof(raw_jvm_type::u2));
    if constexpr (std::endian::native == std::endian::little) {
        *u2_addr = __builtin_bswap16(*u2_addr); 
    }
}

inline void ByteCodeReader::read_u4(raw_jvm_type::u4* const u4_addr) {
    this->file_binary_resource.read(reinterpret_cast<char*>(u4_addr), sizeof(raw_jvm_type::u4));
    if constexpr (std::endian::native == std::endian::little) {
        *u4_addr = __builtin_bswap32(*u4_addr);
    }
}

inline void ByteCodeReader::read_u8(raw_jvm_type::u8* const u8_addr) {
    this->file_binary_resource.read(reinterpret_cast<char*>(u8_addr), sizeof(raw_jvm_type::u8));
    if constexpr (std::endian::native == std::endian::little) {
        *u8_addr = __builtin_bswap64(*u8_addr);
    }
}
