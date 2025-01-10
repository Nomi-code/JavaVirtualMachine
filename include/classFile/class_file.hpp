#pragma once
#include <fstream>
#include <cassert>
#include <ostream>
#include <system_error>
#include <iostream>
#include <spdlog/spdlog.h>

#include "../java_base.hpp"
#include "byte_code_reader.hpp"

using namespace jvm_raw_type;
using std::fstream;
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

class FieldInfo;
class MethodInfo;
class AttributeInfo;
class ConstantInfo;

class ConstantInfo {
  public:
    u1 tag;
    static ConstantInfo* build();
    friend fstream& operator>>(fstream& in, ConstantInfo& ci);
};
class ConstantClass : public ConstantInfo {
    u2 name_index;
    friend fstream& operator>>(fstream& in, ConstantClass& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.name_index);
        return in;
    }
};
class ConstantFieldRef : public ConstantInfo {
    u2 name_index;
    u2 name_and_type_index;
    friend fstream& operator>>(fstream& in, ConstantFieldRef& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.name_index);
        bcr.read_u2(&ci.name_and_type_index);
        return in;
    }
};
class ConstantMethodRef : public ConstantInfo {
    u2 name_index;
    u2 name_and_type_index;
    friend fstream& operator>>(fstream& in, ConstantMethodRef& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.name_index);
        bcr.read_u2(&ci.name_and_type_index);
        return in;
    }
};
class ConstantInterfaceMethodRef : public ConstantInfo {
    u2 name_index;
    u2 name_and_type_index;
    friend fstream& operator>>(fstream& in, ConstantInterfaceMethodRef& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.name_index);
        bcr.read_u2(&ci.name_and_type_index);
        return in;
    }
};
class ConstantString : public ConstantInfo {
    u2 string_index;
    friend fstream& operator>>(fstream& in, ConstantString& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.string_index);
        return in;
    }
};
class ConstantInteger : public ConstantInfo {
    u4 bytes;
    friend fstream& operator>>(fstream& in, ConstantInteger& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u4(&ci.bytes);
        return in;
    }
};
class ConstantFloat : public ConstantInfo {
    u4 bytes;
    friend fstream& operator>>(fstream& in, ConstantFloat& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u4(&ci.bytes);
        return in;
    }
};
class ConstantLong : public ConstantInfo {
    u4 high_bytes;
    u4 low_bytes;
    friend fstream& operator>>(fstream& in, ConstantLong& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u4(&ci.high_bytes);
        bcr.read_u4(&ci.low_bytes);
        return in;
    }
};
class ConstantDouble : public ConstantInfo {
    u4 high_bytes;
    u4 low_bytes;
    friend fstream& operator>>(fstream& in, ConstantDouble& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u4(&ci.high_bytes);
        bcr.read_u4(&ci.low_bytes);
        return in;
    }
};
class ConstantNameAndType : public ConstantInfo {
    u2 name_index;
    u2 descriptor_index;
    friend fstream& operator>>(fstream& in, ConstantNameAndType& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.name_index);
        bcr.read_u2(&ci.descriptor_index);
        return in;
    }
};
class ConstantUtf8 : public ConstantInfo, public Printable {
    u2 length;
    u1* bytes;
    friend fstream& operator>>(fstream& in, ConstantUtf8& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.length);
        ci.bytes = new u1[ci.length];
        for (size_t index = 0; index < ci.length; index++) {
            bcr.read_u1(&ci.bytes[index]);
        }
        return in;
    }

    friend ostream& operator<<(ostream& out, ConstantUtf8& ci) {
        for (size_t index = 0; index < ci.length; index++) {
            out << ci.bytes[index];
        }
        return out;
    }

  public:
    void print() {
        char* tmp = new char[this->length + 1]{0};
        memcpy(tmp, this->bytes, this->length * sizeof(u1));
        spdlog::warn("read utf-8 constant info: {:}", tmp);
        delete[] tmp;
    }
};
class ConstantMethodHandle : public ConstantInfo {
    u1 reference_kind;
    u2 reference_index;
    friend fstream& operator>>(fstream& in, ConstantMethodHandle& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u1(&ci.reference_kind);
        bcr.read_u2(&ci.reference_index);
        return in;
    }
};
class ConstantMethodType : public ConstantInfo {
    u2 descriptor_index;
    friend fstream& operator>>(fstream& in, ConstantMethodType& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.descriptor_index);
        return in;
    }
};
class ConstantInvokeDynamic : public ConstantInfo {
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
    friend fstream& operator>>(fstream& in, ConstantInvokeDynamic& ci) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ci.bootstrap_method_attr_index);
        bcr.read_u2(&ci.name_and_type_index);
        return in;
    }
};

class AttributeInfo {
  private:
    u2 attribute_name_index;
    u4 attribute_length;
    u1* info;
    friend fstream& operator>>(fstream& in, AttributeInfo& ai) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&ai.attribute_name_index);
        bcr.read_u4(&ai.attribute_length);

        for (u4 index = 0; index < ai.attribute_length; index++) {
            bcr.read_u1(&ai.info[index]);
        }
        return in;
    }
};

class FieldInfo {
  private:
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attribute_count;
    AttributeInfo* attributes;

  public:
    friend fstream& operator>>(fstream& in, FieldInfo& fi) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&fi.access_flags);
        bcr.read_u2(&fi.name_index);
        bcr.read_u2(&fi.descriptor_index);
        bcr.read_u2(&fi.attribute_count);

        fi.attributes = new AttributeInfo[fi.attribute_count];
        for (u2 index = 0; index < fi.attribute_count; index++) {
            in >> fi.attributes[index];
        }
        return in;
    }
};

class MethodInfo {
  private:
    u2 access_flags;
    u2 name_index;
    u2 descriptor_index;
    u2 attribute_count;
    AttributeInfo* attributes;

  public:
    friend fstream& operator>>(fstream& in, MethodInfo& mi) {
        ByteCodeReader bcr(in);
        bcr.read_u2(&mi.access_flags);
        bcr.read_u2(&mi.name_index);
        bcr.read_u2(&mi.descriptor_index);
        bcr.read_u2(&mi.attribute_count);

        mi.attributes = new AttributeInfo[mi.attribute_count];
        for (u2 index = 0; index < mi.attribute_count; index++) {
            in >> mi.attributes[index];
        }
        return in;
    }
};

class ClassFileUtils {
  public:
    static ConstantInfo* build_constant_info(fstream& in, u1 tag) {
        ConstantInfo* info = nullptr;

#ifndef BUILD_CONSTANT
#define BUILD_CONSTANT(CONDITION, PTR_TYPE)                                                        \
    case CONDITION: {                                                                              \
        PTR_TYPE* ptr = new PTR_TYPE();                                                            \
        ptr->tag = tag;                                                                            \
        in >> *ptr;                                                                                \
        info = ptr;                                                                                \
        break;                                                                                     \
    }
#endif
        switch (tag) {
            BUILD_CONSTANT(CONSTANT_Class, ConstantClass);
            BUILD_CONSTANT(CONSTANT_Fieldref, ConstantFieldRef);
            BUILD_CONSTANT(CONSTANT_Methodref, ConstantMethodRef);
            BUILD_CONSTANT(CONSTANT_InterfaceMethodref, ConstantInterfaceMethodRef);
            BUILD_CONSTANT(CONSTANT_String, ConstantString);
            BUILD_CONSTANT(CONSTANT_Integer, ConstantInteger);
            BUILD_CONSTANT(CONSTANT_Float, ConstantFloat);
            BUILD_CONSTANT(CONSTANT_Long, ConstantLong);
            BUILD_CONSTANT(CONSTANT_Double, ConstantDouble);
            BUILD_CONSTANT(CONSTANT_NameAndType, ConstantNameAndType);
            BUILD_CONSTANT(CONSTANT_Utf8, ConstantUtf8);
            BUILD_CONSTANT(CONSTANT_MethodHandle, ConstantMethodHandle);
            BUILD_CONSTANT(CONSTANT_MethodType, ConstantMethodType);
            BUILD_CONSTANT(CONSTANT_InvokeDynamic, ConstantInvokeDynamic);
            default: {
                spdlog::error("cant resolve tag value: {:x}\n", tag);
                assert(false);
            }
        }

#ifdef DEBUG
        if (tag == CONSTANT_Utf8) {
            ConstantUtf8* ptr = static_cast<ConstantUtf8*>(info);
            ptr->print();
        }
#endif
        assert(info != nullptr);
        return info;
    }
};

class ClassFile : public Printable {
  private:
    u4 magic = 0;
    u2 minor_version = 0;
    u2 major_version = 0;
    u2 constant_pool_count = 0;
    ConstantInfo** constant_pool = nullptr;
    u2 access_flags = 0;
    u2 this_class = 0;
    u2 super_class = 0;
    u2 interfaces_count = 0;
    u2* interfaces = nullptr;
    u2 fields_count = 0;
    FieldInfo* fields = nullptr;
    u2 methods_count = 0;
    MethodInfo* methods = nullptr;
    u2 attributes_count = 0;
    AttributeInfo* attributes = nullptr;

  public:
    ClassFile() = default;

    void print() {
        spdlog::info("############## class file start ##############");
        spdlog::info("class file magic number : {:X}", this->magic);
        spdlog::info("class file minor version : {:d}", this->minor_version);
        spdlog::info("class file major version : {:d}", this->major_version);
        spdlog::info("class file constant pool count : {:d}", this->constant_pool_count);
        spdlog::info("class file access flags : {:X}", this->access_flags);

        spdlog::info("class file this class : {:d}", this->this_class);
        spdlog::info("class file super class : {:d}", this->super_class);
        spdlog::info("class file interfaces count : {:d}", this->interfaces_count);

        spdlog::info("class file fields count : {:d}", this->fields_count);
        spdlog::info("class file methods count : {:d}", this->methods_count);
        spdlog::info("class file attributes count : {:d}", this->attributes_count);

        spdlog::info("############## class file end ##############");
    }

    friend fstream& operator>>(fstream& in, ClassFile& cf) {
        ByteCodeReader bcr(in);
        bcr.read_u4(&cf.magic);
        bcr.read_u2(&cf.major_version);
        bcr.read_u2(&cf.major_version);
        bcr.read_u2(&cf.constant_pool_count);

        // input constant info, index 0 unused
        cf.constant_pool = new ConstantInfo*[cf.constant_pool_count + 1]();
        for (u2 index = 1; index < cf.constant_pool_count; index++) {
            u1 tag = -1;
            bcr.read_u1(&tag);
#ifdef DEBUG
            spdlog::info("read tag value from constant pool [{0:}]: {1:02x}", index, tag);
#endif
            cf.constant_pool[index] = ClassFileUtils::build_constant_info(in, tag);
            if (cf.constant_pool[index]->tag == CONSTANT_Double ||
                cf.constant_pool[index]->tag == CONSTANT_Long) {
                // Long and double occupy two constant pool positions
                index += 1;
            }
        }

        bcr.read_u2(&cf.access_flags);
        bcr.read_u2(&cf.this_class);
        bcr.read_u2(&cf.super_class);
        bcr.read_u2(&cf.interfaces_count);

        if (cf.interfaces_count > 0) {
            cf.interfaces = new u2[cf.interfaces_count]{0};
            for (u2 index = 0; index < cf.interfaces_count; index++) {
                bcr.read_u2(&cf.interfaces[index]);
            }
        }

        if (cf.fields_count > 0) {
            bcr.read_u2(&cf.fields_count);
            cf.fields = new FieldInfo[cf.fields_count];
            for (u2 index = 0; index < cf.fields_count; index++) {
                in >> cf.fields[index];
            }
        }

        if (cf.methods_count > 0) {
            bcr.read_u2(&cf.methods_count);
            cf.methods = new MethodInfo[cf.methods_count];
            for (u2 index = 0; index < cf.methods_count; index++) {
                in >> cf.methods[index];
            }
        }

        if (cf.attributes_count > 0) {
            bcr.read_u2(&cf.attributes_count);
            cf.attributes = new AttributeInfo[cf.attributes_count];
            for (u2 index = 0; index < cf.attributes_count; index++) {
                in >> cf.attributes[index];
            }
        }

        return in;
    }
};
