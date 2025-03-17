#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <filesystem>
#include <stack>

#include "../../include/runtime/klass.hpp"
#include "classFile/byte_code_reader.hpp"
#include "classFile/class_file.hpp"

namespace fs = std::filesystem;

static const std::string test_class_file_directory = "/workspace/JavaVirtualMachine/resource/";

static void try_parse_class_file(const fs::path& p) {
    // spdlog::set_level(spdlog::level::level_enum::debug);

    if (!std::string(p).ends_with(".class")) {
        std::string file_name(p);
        spdlog::error("file {} is not a class file", file_name);
        return;
    }
    raw_jvm_data::ByteCodeReader bcr(p);
    spdlog::debug("start read class file: {}", p.c_str());
    rt_jvm_data::Klass kls(bcr);
    // only test class file reading
    // raw_jvm_data::ClassFile clf(bcr);
    spdlog::debug("end read class file {}", p.c_str());
    // kls.print();
}

TEST(CLASS_FILE_TEST, CLASS_FILE_INPUT_TEST) {
    using std::cout;
    using std::fstream;
    using std::ifstream;
    using std::stack;
    using std::string;
    spdlog::debug("start class file resloving test...");
    auto pass = true;

    stack<fs::path> paths;
    paths.emplace(fs::path(test_class_file_directory));

    while (!paths.empty()) {
        const auto p = paths.top();
        paths.pop();

        if (fs::is_directory(p)) {
            for (const auto& entry : fs::directory_iterator(p)) {
                const auto& sub_p = entry.path();
                if (fs::is_directory(sub_p)) {
                    spdlog::debug("reading dir {}", sub_p.c_str());
                    paths.emplace(sub_p);
                } else {
                    try_parse_class_file(sub_p);
                }
            }
        } else {
            try_parse_class_file(p);
        }
    }

    EXPECT_TRUE(pass);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}