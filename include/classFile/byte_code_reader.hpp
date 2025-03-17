#pragma once

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <memory>
#include <type_traits>

#include "../java_base.hpp"

namespace raw_jvm_data {
    class ByteCodeReader : protected std::fstream {
      private:
        std::unique_ptr<char[]> buffer;
        size_t fsize = -1;
        size_t fpos = -1;

      public:
        ByteCodeReader() = delete;
        ByteCodeReader(const ByteCodeReader&) = delete;
        ByteCodeReader& operator=(const ByteCodeReader&) = delete;
        ~ByteCodeReader() = default;
        explicit ByteCodeReader(const std::filesystem::path&);

        void read_u1(raw_jvm_type::u1* const);
        void read_u2(raw_jvm_type::u2* const);
        void read_u4(raw_jvm_type::u4* const);
        void read_u8(raw_jvm_type::u8* const);
        void read_multiple_bytes(raw_jvm_type::u1* const dest, const int& n);

#define BSWAP(value)                                                                               \
    (sizeof(value) == 2                                                                            \
         ? __builtin_bswap16(value)                                                                \
         : (sizeof(value) == 4 ? __builtin_bswap32(value)                                          \
                               : (sizeof(value) == 8 ? __builtin_bswap64(value) : value)))

        template <class T> void read_impl(T* const);
    };

}; // namespace raw_jvm_data
