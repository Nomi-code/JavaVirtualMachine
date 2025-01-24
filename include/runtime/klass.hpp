#pragma once
#include <cstddef>
#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "java_base.hpp"
#include "string_pool.hpp"
#include "../classFile/class_file.hpp"

class ConstantItem {};

class Field {};

struct Attribute {
    jvm_raw_type::u4 attribute_length;
    jvm_raw_type::u1 info;
};

struct Method {
    jvm_raw_type::u2 access_flag;
    jvm_raw_type::u1 code[1];
    std::string& name_index;
    std::string& descriptor;
};

struct MethodReference {
  public:
};

class Klass {
  private:
    static std::unordered_map<std::string, Klass*> klass_container;

    std::vector<std::unique_ptr<ConstantItem>> constant_items;
    std::unordered_map<std::unique_ptr<std::string>, Method> methods;
    std::unordered_map<std::unique_ptr<std::string>, Field> fields;
  public:
    void reinterpret(const raw_jvm_data::ClassFile& class_file);
};