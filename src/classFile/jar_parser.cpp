#include <memory>
#include <minizip/unzip.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

#include "classFile/jar_parser.hpp"

using namespace raw_jvm_data;

static bool is_directory(const std::string& file_name) {
    return !file_name.empty() && file_name[file_name.size() - 1] == '/';
}

static bool is_class_file(const std::string& file_name) {
    return file_name.size() > 6 && file_name.ends_with(".class");
}

JarParser::JarParser(const std::filesystem::path& p) {
    this->p = p;
    this->unz_file = unzOpen(p.c_str());
    if (unz_file == nullptr) {
        spdlog::error("can't open jar file {}", p.c_str());
        throw std::runtime_error("cant open jar file");
    }
}

void JarParser::parser() {
    if (unzGoToFirstFile(this->unz_file) != UNZ_OK) {
        spdlog::debug("no file in jar {}", p.c_str());
        return;
    }

    do {
        char file_name_cstr[256];
        unz_file_info unz_file_info;
        if (unzGetCurrentFileInfo(this->unz_file, &unz_file_info, file_name_cstr,
                                  sizeof(file_name_cstr), nullptr, 0, nullptr, 0) != UNZ_OK) {
            spdlog::error("can't get current file");
            break;
        }
        std::string file_name(file_name_cstr);

        if (is_directory(file_name)) {
            
        }

        if (is_class_file(file_name)) {
        }
        
    } while (unzGoToNextFile(this->unz_file) == UNZ_OK);
}

JarParser::~JarParser() {
    unzClose(this->unz_file);
}