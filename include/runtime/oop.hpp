#pragma once

#include "java_base.hpp"
#include "klass.hpp"

namespace oop {
    enum class OopType {
        NormalObject,
        ArrayObject,
        MirrorObject,
    };

    class Object {
      public:
        Object() = default;
        virtual ~Object() = default;
        static OopType get_type(const Object& obj);
        static rt_jvm_data::Klass_ptr get_klass_ptr(const Object& obj);
      protected:
        OopType type;
        raw_jvm_type::u1 info;
        rt_jvm_data::Klass_ptr klass_ptr;
    };

    class ArrayObject : public Object {
      private:
        raw_jvm_type::u4 array_size;

      public:
        static raw_jvm_type::u4 get_array_size(const Object& obj);
    };

    class NormalObject : public Object {};

    class MirrorObject : public Object {};
}; // namespace Oop
