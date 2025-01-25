#pragma once

#include "java_base.hpp"
#include "string_pool.hpp"
#include "../classFile/class_file.hpp"

class Klass {
  private:
  public:
    void reinterpret(const raw_jvm_data::ClassFile& class_file);
};