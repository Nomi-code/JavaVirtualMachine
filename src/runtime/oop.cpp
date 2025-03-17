#include "runtime/oop.hpp"
#include <spdlog/spdlog.h>

using namespace oop;

raw_jvm_type::u4 ArrayObject::get_array_size(const Object& obj) {
    return static_cast<const ArrayObject&>(obj).array_size;
}

OopType Object::get_type(const Object& obj) {
    return obj.type;
}

rt_jvm_data::Klass_ptr Object::get_klass_ptr(const Object& obj) {
    return obj.klass_ptr;
}