#include "classFile/jar_parser.hpp"
#include <memory>
#include <minizip/unzip.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

using namespace raw_jvm_data;

JarParser::JarParser(const std::filesystem::path& p)  {
    this->unz_file = unzOpen(p.c_str());
    if (unz_file == nullptr) {
        spdlog::error("can't open jar file {}", p.c_str());
        throw std::runtime_error("cant open jar file");
    }
}

JarParser::~JarParser() {
    unzClose(this->unz_file);
}