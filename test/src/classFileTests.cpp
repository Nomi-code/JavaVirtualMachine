#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <filesystem>

#include "../../include/runtime/klass.hpp"


static const std::string test_class_file =
    "/workspace/JavaVirtualMachine/resource/FakePhoenixApplication.class";

static const std::string text_file = "/workspace/JavaVirtualMachine/resource/Demo.txt";

static const std::string test_class_file_dir = "/workspace/JavaVirtualMachine/resource";

TEST(CLASS_FILE_TEST, CLASS_FILE_INPUT_TEST) {
    namespace fs = std::filesystem;
    using std::cout;
    using std::fstream;
    using std::ifstream;
    using std::string;

    auto pass = true;

    fs::path dir = test_class_file_dir;
    if (!fs::exists(dir) || !fs::is_directory(dir)) {
        spdlog::error("no such dir: {}", test_class_file_dir);
        pass = false;
    }
    for (const auto& entry : fs::directory_iterator(dir)) {
        auto p = entry.path();
        if (string(p).ends_with(".class")) {
            fstream ifs(p, std::ios::binary | std::ios::in);
            spdlog::warn({"start read class file: {}"}, p.c_str());
            rt_jvm_data::InstanceKlass kls(ifs);

            // kls.print();
            ifs.close();
        }
    }

    EXPECT_TRUE(pass);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}