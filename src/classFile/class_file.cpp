#include "classFile/class_file.hpp"
#include "runtime/string_pool.hpp"
#include "java_base.hpp"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>
#include <utility>

using namespace raw_jvm_data;

ConstantUtf8::~ConstantUtf8() {
    if (bytes != nullptr) delete[] bytes;
}

AttributeInfo::~AttributeInfo() {
    if (info != nullptr) delete[] info;
}

FieldInfo::~FieldInfo() {
    if (attributes != nullptr) delete[] attributes;
}

MethodInfo::~MethodInfo() {
    if (attributes != nullptr) delete[] attributes;
}

ConstantInfo_ptr ClassFile::build_constant_info(fstream& in, u1 tag) {
    ConstantInfo* info = nullptr;

#define BUILD_CONSTANT(CONDITION, PTR_TYPE)                                                        \
    case CONDITION: {                                                                              \
        PTR_TYPE* ptr = new PTR_TYPE();                                                            \
        ptr->tag = tag;                                                                            \
        in >> *ptr;                                                                                \
        info = ptr;                                                                                \
        break;                                                                                     \
    }

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

    assert(info != nullptr);
    return info;
}

ClassFile::ClassFile(std::fstream& in) {
    ByteCodeReader bcr(in);

    bcr.read_u4(&this->magic);
    bcr.read_u2(&this->major_version);
    bcr.read_u2(&this->major_version);
    bcr.read_u2(&this->constant_pool_count);

    // input constant info, index 0 unused
    this->constant_pool = new ConstantInfo_ptr[this->constant_pool_count + 1]{0};
    for (u2 index = 1; index < this->constant_pool_count; index++) {
        u1 tag = -1;
        bcr.read_u1(&tag);
        this->constant_pool[index] = this->build_constant_info(in, tag);

        if (this->constant_pool[index]->tag == CONSTANT_Double ||
            this->constant_pool[index]->tag == CONSTANT_Long) {
            // Long and double occupy two constant pool positions
            index += 1;
        }
    }

    bcr.read_u2(&this->access_flags);
    bcr.read_u2(&this->this_class);
    bcr.read_u2(&this->super_class);
    bcr.read_u2(&this->interfaces_count);

    if (this->interfaces_count > 0) {
        this->interfaces = new u2[this->interfaces_count]{0};
        for (u2 index = 0; index < this->interfaces_count; index++) {
            bcr.read_u2(&this->interfaces[index]);
        }
    }

    bcr.read_u2(&this->fields_count);
    if (this->fields_count > 0) {
        this->fields = new FieldInfo[this->fields_count];
        for (u2 index = 0; index < this->fields_count; index++) {
            in >> this->fields[index];
        }
    }

    bcr.read_u2(&this->methods_count);
    if (this->methods_count > 0) {
        this->methods = new MethodInfo[this->methods_count];
        for (u2 index = 0; index < this->methods_count; index++) {
            in >> this->methods[index];
        }
    }

    bcr.read_u2(&this->attributes_count);
    if (this->attributes_count > 0) {
        this->attributes = new AttributeInfo[this->attributes_count];
        for (u2 index = 0; index < this->attributes_count; index++) {
            in >> this->attributes[index];
        }
    }
}

ClassFile::~ClassFile() {
    if (this->constant_pool != nullptr) {

        for (size_t index = 0; index < this->constant_pool_count; index++) {
            auto ptr = this->constant_pool[index];
            if (ptr != nullptr) {
                switch (ptr->tag) {
                    case CONSTANT_Utf8: {
                        auto u8ptr = static_cast<ConstantUtf8_ptr>(ptr);
                        delete u8ptr;
                        break;
                    }
                    default: {
                        delete ptr;
                    }
                }
                this->constant_pool[index] = nullptr;
            }
        }
        delete[] this->constant_pool;
        this->constant_pool = nullptr;
    }

    if (this->interfaces != nullptr) {
        delete[] this->interfaces;
        this->interfaces = nullptr;
    }

    if (this->fields != nullptr) {
        delete[] this->fields;
        this->fields = nullptr;
    }

    if (this->methods != nullptr) {
        delete[] this->methods;
        this->methods = nullptr;
    }

    if (this->attributes != nullptr) {
        delete[] this->attributes;
        this->attributes = nullptr;
    }
}

