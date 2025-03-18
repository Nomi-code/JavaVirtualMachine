#include "classFile/class_file.hpp"
#include "classFile/byte_code_reader.hpp"
#include "java_base.hpp"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>

using namespace raw_jvm_data;

ConstantUtf8::~ConstantUtf8() {
    // if (bytes != nullptr) delete[] bytes;
}

AttributeInfo::~AttributeInfo() {
    // if (info != nullptr) delete[] info;
}

FieldInfo::~FieldInfo() {
    // if (attributes != nullptr) delete[] attributes;
}

MethodInfo::~MethodInfo() {
    // if (attributes != nullptr) delete[] attributes;
}

ConstantInfo_ptr ClassFile::build_constant_info(ByteCodeReader& bcr, u1 tag) {
    ConstantInfo* info = nullptr;

    switch (tag) {
        case CONSTANT_Class: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantClass>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantClass_ptr>(info));
            break;
        }
        case CONSTANT_Fieldref: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantFieldRef>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantFieldRef_ptr>(info));
            break;
        }
        case CONSTANT_Methodref: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantMethodRef>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantMethodRef_ptr>(info));
            break;
        }
        case CONSTANT_InterfaceMethodref: {
            info =
                rt_jvm_memory::StaticSpaceAllocator<ConstantInterfaceMethodRef>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantInterfaceMethodRef_ptr>(info));
            break;
        }
        case CONSTANT_String: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantString>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantString_ptr>(info));
            break;
        }
        case CONSTANT_Integer: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantInteger>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantInteger_ptr>(info));
            break;
        }
        case CONSTANT_Float: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantFloat>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantFloat_ptr>(info));
            break;
        }
        case CONSTANT_Long: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantLong>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantLong_ptr>(info));
            break;
        }
        case CONSTANT_Double: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantDouble>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantDouble_ptr>(info));
            break;
        }
        case CONSTANT_NameAndType: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantNameAndType>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantNameAndType_ptr>(info));
            break;
        }
        case CONSTANT_Utf8: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantUtf8>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantUtf8_ptr>(info));
            break;
        }
        case CONSTANT_MethodHandle: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantMethodHandle>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantMethodHandle_ptr>(info));
            break;
        }
        case CONSTANT_MethodType: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantMethodType>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantMethodType_ptr>(info));
            break;
        }
        case CONSTANT_InvokeDynamic: {
            info = rt_jvm_memory::StaticSpaceAllocator<ConstantInvokeDynamic>::allocate_static(1);
            info->tag = tag;
            bcr >> *(static_cast<ConstantInvokeDynamic_ptr>(info));
            break;
        }
        default: {
            assert(false);
        }
    }

    assert(info != nullptr);
    return info;
}

ClassFile::ClassFile(ByteCodeReader& bcr) {
    bcr.read_u4(&this->magic);
    bcr.read_u2(&this->minor_version);
    bcr.read_u2(&this->major_version);
    bcr.read_u2(&this->constant_pool_count);

    // input constant info, index 0 unused
    this->constant_pool = new ConstantInfo_ptr[this->constant_pool_count + 1]{0};
    for (u2 index = 1; index < this->constant_pool_count; index++) {
        u1 tag = -1;
        bcr.read_u1(&tag);
        this->constant_pool[index] = this->build_constant_info(bcr, tag);

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
            bcr >> this->fields[index];
        }
    }

    bcr.read_u2(&this->methods_count);
    if (this->methods_count > 0) {
        this->methods = new MethodInfo[this->methods_count];
        for (u2 index = 0; index < this->methods_count; index++) {
            bcr >> this->methods[index];
        }
    }

    bcr.read_u2(&this->attributes_count);
    if (this->attributes_count > 0) {
        this->attributes = new AttributeInfo[this->attributes_count];
        for (u2 index = 0; index < this->attributes_count; index++) {
            bcr >> this->attributes[index];
        }
    }
}

ClassFile::~ClassFile() {
    if (this->constant_pool != nullptr) {
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
