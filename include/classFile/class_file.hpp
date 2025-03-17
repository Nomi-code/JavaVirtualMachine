#pragma once
#include <algorithm>
#include <cassert>
#include <cstring>
#include <ostream>
#include <system_error>
#include <iostream>
#include <spdlog/spdlog.h>
#include <unordered_map>

#include "../java_base.hpp"
#include "byte_code_reader.hpp"

namespace raw_jvm_data {
    using namespace raw_jvm_type;
    using std::istream;
    using std::nullptr_t;
    using std::ostream;

    constexpr u1 CONSTANT_Utf8 = 1;
    constexpr u1 CONSTANT_Integer = 3;
    constexpr u1 CONSTANT_Float = 4;
    constexpr u1 CONSTANT_Long = 5;
    constexpr u1 CONSTANT_Double = 6;
    constexpr u1 CONSTANT_Class = 7;
    constexpr u1 CONSTANT_String = 8;
    constexpr u1 CONSTANT_Fieldref = 9;
    constexpr u1 CONSTANT_Methodref = 10;
    constexpr u1 CONSTANT_InterfaceMethodref = 11;
    constexpr u1 CONSTANT_NameAndType = 12;
    constexpr u1 CONSTANT_MethodHandle = 15;
    constexpr u1 CONSTANT_MethodType = 16;
    constexpr u1 CONSTANT_InvokeDynamic = 18;

    constexpr u2 ACC_PUBLIC = 0x0001;
    constexpr u2 ACC_PRIVATE = 0x0002;
    constexpr u2 ACC_PROTECTED = 0x0004;
    constexpr u2 ACC_STATIC = 0x0008;
    constexpr u2 ACC_FINAL = 0x0010;
    constexpr u2 ACC_SYNCHRONIZED = 0x0020;
    constexpr u2 ACC_BRIDGE = 0x0040;
    constexpr u2 ACC_VOLATILE = 0x0040;
    constexpr u2 ACC_TRANSIENT = 0x0080;
    constexpr u2 ACC_NATIVE = 0x0100;
    constexpr u2 ACC_ABSTRACT = 0x0400;
    constexpr u2 ACC_STRICT = 0x0800;
    constexpr u2 ACC_SYNTHETIC = 0x1000;
    constexpr u2 ACC_ENUM = 0x4000;

    struct ConstantInfo;
    struct ConstantClass;
    struct ConstantFieldRef;
    struct ConstantMethodRef;
    struct ConstantInterfaceMethodRef;
    struct ConstantString;
    struct ConstantInteger;
    struct ConstantFloat;
    struct ConstantLong;
    struct ConstantDouble;
    struct ConstantNameAndType;
    struct ConstantUtf8;
    struct ConstantMethodHandle;
    struct ConstantMethodType;
    struct ConstantInvokeDynamic;

    struct FieldInfo;
    struct MethodInfo;
    struct AttributeInfo;

    class ClassFile;

    using ConstantInfo_ptr = ConstantInfo*;
    using ConstantClass_ptr = ConstantClass*;
    using ConstantFieldRef_ptr = ConstantFieldRef*;
    using ConstantMethodRef_ptr = ConstantMethodRef*;
    using ConstantInterfaceMethodRef_ptr = ConstantInterfaceMethodRef*;
    using ConstantString_ptr = ConstantString*;
    using ConstantInteger_ptr = ConstantInteger*;
    using ConstantFloat_ptr = ConstantFloat*;
    using ConstantLong_ptr = ConstantLong*;
    using ConstantDouble_ptr = ConstantDouble*;
    using ConstantNameAndType_ptr = ConstantNameAndType*;
    using ConstantUtf8_ptr = ConstantUtf8*;
    using ConstantMethodHandle_ptr = ConstantMethodHandle*;
    using ConstantMethodType_ptr = ConstantMethodType*;
    using ConstantInvokeDynamic_ptr = ConstantInvokeDynamic*;

    using FieldInfo_ptr = FieldInfo*;
    using MethodInfo_ptr = MethodInfo*;
    using AttributeInfo_ptr = AttributeInfo*;

    using ClassFile_ptr = ClassFile*;

    struct ConstantInfo {
        u1 tag;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantInfo& ci);
    };

    struct ConstantClass : public ConstantInfo {
        u2 name_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantClass& ci) {
            bcr.read_u2(&ci.name_index);
            return bcr;
        }
    };

    struct ConstantFieldRef : public ConstantInfo {
        u2 name_index;
        u2 name_and_type_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantFieldRef& ci) {
            bcr.read_u2(&ci.name_index);
            bcr.read_u2(&ci.name_and_type_index);
            return bcr;
        }
    };

    struct ConstantMethodRef : public ConstantInfo {
        u2 name_index;
        u2 name_and_type_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantMethodRef& ci) {
            bcr.read_u2(&ci.name_index);
            bcr.read_u2(&ci.name_and_type_index);
            return bcr;
        }
    };

    struct ConstantInterfaceMethodRef : public ConstantInfo {
        u2 name_index;
        u2 name_and_type_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantInterfaceMethodRef& ci) {
            bcr.read_u2(&ci.name_index);
            bcr.read_u2(&ci.name_and_type_index);
            return bcr;
        }
    };

    struct ConstantString : public ConstantInfo {
        u2 string_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantString& ci) {
            bcr.read_u2(&ci.string_index);
            return bcr;
        }
    };

    struct ConstantInteger : public ConstantInfo {
        u4 bytes;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantInteger& ci) {
            bcr.read_u4(&ci.bytes);
            return bcr;
        }
    };

    struct ConstantFloat : public ConstantInfo {
        u4 bytes;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantFloat& ci) {
            bcr.read_u4(&ci.bytes);
            return bcr;
        }
    };

    struct ConstantLong : public ConstantInfo {
        u4 high_bytes;
        u4 low_bytes;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantLong& ci) {
            bcr.read_u4(&ci.high_bytes);
            bcr.read_u4(&ci.low_bytes);
            return bcr;
        }
    };

    struct ConstantDouble : public ConstantInfo {
        u4 high_bytes;
        u4 low_bytes;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantDouble& ci) {
            bcr.read_u4(&ci.high_bytes);
            bcr.read_u4(&ci.low_bytes);
            return bcr;
        }
    };

    struct ConstantNameAndType : public ConstantInfo {
        u2 name_index;
        u2 descriptor_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantNameAndType& ci) {
            bcr.read_u2(&ci.name_index);
            bcr.read_u2(&ci.descriptor_index);
            return bcr;
        }
    };

    struct ConstantUtf8 : public ConstantInfo, public Printable {
        u2 length;
        u1_ptr bytes;

        ~ConstantUtf8();

        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantUtf8& ci) {
            bcr.read_u2(&ci.length);
            // to c string
            ci.bytes = new u1[ci.length + 1]{0};
            bcr.read_multiple_bytes(ci.bytes, ci.length);
            return bcr;
        }

        friend ostream& operator<<(ostream& out, ConstantUtf8& ci) {
            for (size_t index = 0; index < ci.length; index++) {
                out << ci.bytes[index];
            }
            return out;
        }

        void print() const override {
            char* tmp = new char[this->length + 1]{0};
            memcpy(tmp, this->bytes, this->length * sizeof(u1));
            spdlog::debug("read utf-8 constant info: {:}", tmp);
            delete[] tmp;
        }
    };

    struct ConstantMethodHandle : public ConstantInfo {
        u1 reference_kind;
        u2 reference_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantMethodHandle& ci) {
            bcr.read_u1(&ci.reference_kind);
            bcr.read_u2(&ci.reference_index);
            return bcr;
        }
    };

    struct ConstantMethodType : public ConstantInfo {
        u2 descriptor_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantMethodType& ci) {
            bcr.read_u2(&ci.descriptor_index);
            return bcr;
        }
    };

    struct ConstantInvokeDynamic : public ConstantInfo {
        u2 bootstrap_method_attr_index;
        u2 name_and_type_index;
        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, ConstantInvokeDynamic& ci) {
            bcr.read_u2(&ci.bootstrap_method_attr_index);
            bcr.read_u2(&ci.name_and_type_index);
            return bcr;
        }
    };

    struct AttributeInfo {
        u2 attribute_name_index;
        u4 attribute_length;
        u1_ptr info;

        ~AttributeInfo();

        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, AttributeInfo& ai) {
            bcr.read_u2(&ai.attribute_name_index);
            bcr.read_u4(&ai.attribute_length);
            // allocate new array
            ai.info = new u1[ai.attribute_length];
            bcr.read_multiple_bytes(ai.info, ai.attribute_length);
            return bcr;
        }
    };

    struct FieldInfo {
        u2 access_flags;
        u2 name_index;
        u2 descriptor_index;
        u2 attribute_count;
        AttributeInfo_ptr attributes;

        ~FieldInfo();

        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, FieldInfo& fi) {
            bcr.read_u2(&fi.access_flags);
            bcr.read_u2(&fi.name_index);
            bcr.read_u2(&fi.descriptor_index);
            bcr.read_u2(&fi.attribute_count);

            fi.attributes = new AttributeInfo[fi.attribute_count];
            for (u2 index = 0; index < fi.attribute_count; index++) {
                bcr >> fi.attributes[index];
            }
            return bcr;
        }
    };

    struct MethodInfo {
        u2 access_flags;
        u2 name_index;
        u2 descriptor_index;
        u2 attribute_count;
        AttributeInfo_ptr attributes;

        ~MethodInfo();

        friend ByteCodeReader& operator>>(ByteCodeReader& bcr, MethodInfo& mi) {
            bcr.read_u2(&mi.access_flags);
            bcr.read_u2(&mi.name_index);
            bcr.read_u2(&mi.descriptor_index);
            bcr.read_u2(&mi.attribute_count);

            mi.attributes = new AttributeInfo[mi.attribute_count];
            for (u2 index = 0; index < mi.attribute_count; index++) {
                bcr >> mi.attributes[index];
            }
            return bcr;
        }
    };

    class ClassFile : public Printable {
      protected:
        u4 magic = 0;
        u2 minor_version = 0;
        u2 major_version = 0;
        u2 constant_pool_count = 0;
        ConstantInfo_ptr* constant_pool = nullptr;
        u2 access_flags = 0;
        u2 this_class = 0;
        u2 super_class = 0;
        u2 interfaces_count = 0;
        u2_ptr interfaces = nullptr;
        u2 fields_count = 0;
        FieldInfo_ptr fields = nullptr;
        u2 methods_count = 0;
        MethodInfo_ptr methods = nullptr;
        u2 attributes_count = 0;
        AttributeInfo_ptr attributes = nullptr;

      private:
        ConstantInfo_ptr build_constant_info(ByteCodeReader& bcr, u1 tag);

      public:
        ClassFile(ByteCodeReader& bcr);
        ClassFile() = delete;
        ClassFile(const ClassFile&) = delete;
        ClassFile& operator=(const ClassFile&) = delete;
        ClassFile(ClassFile&&) = delete;
        ~ClassFile();

        void print() const override {
            spdlog::debug("############## class file start ##############");
            spdlog::debug("class file magic number : {:X}", this->magic);
            spdlog::debug("class file minor version : {:d}", this->minor_version);
            spdlog::debug("class file major version : {:d}", this->major_version);
            spdlog::debug("class file constant pool count : {:d}", this->constant_pool_count);
            spdlog::debug("class file access flags : {:X}", this->access_flags);

            spdlog::debug("class file this class : {:d}", this->this_class);
            spdlog::debug("class file super class : {:d}", this->super_class);
            spdlog::debug("class file interfaces count : {:d}", this->interfaces_count);

            spdlog::debug("class file fields count : {:d}", this->fields_count);
            spdlog::debug("class file methods count : {:d}", this->methods_count);
            spdlog::debug("class file attributes count : {:d}", this->attributes_count);

            spdlog::debug("############## class file end ##############");
        }
    };

} // namespace raw_jvm_data
