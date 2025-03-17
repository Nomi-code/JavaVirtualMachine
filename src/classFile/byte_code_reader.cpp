#include <cassert>
#include <cstring>
#include <spdlog/spdlog.h>

#include "classFile/byte_code_reader.hpp"

using namespace raw_jvm_data;

ByteCodeReader::ByteCodeReader(const std::filesystem::path& p) {
    open(p, std::ios::binary | std::ios::in);
    if (!is_open()) {
        spdlog::error("[classFile byte_code_reader] cant open file {}", p.c_str());
        exit(-1);
    }
    seekg(0, std::ios::end);
    this->fsize = tellg();
    this->fpos = 0;
    seekg(0, std::ios::beg);
    buffer = std::unique_ptr<char[]>(new char[this->fsize]);
    if (!read(this->buffer.get(), this->fsize)) {
        spdlog::error("read class file error");
        exit(-1);
    }
    close();
}

template <class T> inline void ByteCodeReader::read_impl(T* const addr) {
    *addr = *((T*)(buffer.get() + fpos));
    fpos += sizeof(T);
#ifdef DEBUG
    assert(fpos <= fsize);
#endif
    if constexpr (sizeof(T) > 1 && (std::endian::native == std::endian::little)) {
        *addr = BSWAP(*addr);
    }
}

void ByteCodeReader::read_multiple_bytes(raw_jvm_type::u1* const dest, const int& n) {
#ifdef DEBUG
    assert(fpos + n <= fsize);
#endif
    memcpy(dest, this->buffer.get() + this->fpos, n);
    this->fpos += n;
}

void ByteCodeReader::read_u1(raw_jvm_type::u1* const u1_addr) {
    read_impl(u1_addr);
}

void ByteCodeReader::read_u2(raw_jvm_type::u2* const u2_addr) {
    read_impl(u2_addr);
}

void ByteCodeReader::read_u4(raw_jvm_type::u4* const u4_addr) {
    read_impl(u4_addr);
}

void ByteCodeReader::read_u8(raw_jvm_type::u8* const u8_addr) {
    read_impl(u8_addr);
}