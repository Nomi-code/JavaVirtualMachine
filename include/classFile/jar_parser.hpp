#pragma once

#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <minizip/zip.h>
#include <minizip/unzip.h>
#include <memory>

#include "java_base.hpp"

namespace raw_jvm_data {
    class JarParser {
      private:
        // unzFile is a void* pointer
        unzFile unz_file;
      public:
        JarParser(const std::filesystem::path& p);
        ~JarParser();

        void f() {
            // TODO
        }
    };
}; // namespace raw_jvm_data