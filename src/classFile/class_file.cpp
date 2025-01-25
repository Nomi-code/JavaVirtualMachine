#include "../../include/classFile/class_file.hpp"
#include "../../include/runtime/string_pool.hpp"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

using namespace raw_jvm_data;
using namespace rt_jvm_data;
using std::string;
using std::unordered_map;

ConstantInfo_ptr ClassFile::build_constant_info(fstream& in, u1 tag) {
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
    this->constant_pool = new ConstantInfo_ptr[this->constant_pool_count + 1];
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

MethodWrapper::MethodWrapper(MethodInfo_ptr mptr) : mptr(mptr) {
}

Klass::Klass(std::fstream& in) : raw_jvm_data::ClassFile(in) {
    unordered_map<string, MethodWrapper> method_wrapper_rec;
    unordered_map<string, FieldWrapper> field_wrapper_rec;
    spdlog::error("start validate {:d}", this->methods_count);
    for (size_t index = 0; index < this->methods_count; index++) {
        MethodInfo_ptr mptr = &this->methods[index];
        ConstantNameAndType_ptr natptr =
            static_cast<ConstantNameAndType_ptr>(this->constant_pool[mptr->descriptor_index]);

        ConstantUtf8_ptr name_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[mptr->name_index]);
        ConstantUtf8_ptr nat_name_u8_ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[natptr->name_index]);
        ConstantUtf8_ptr nat_des_u8_ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[natptr->descriptor_index]);

        assert(natptr->tag == CONSTANT_NameAndType);
        assert(name_u8ptr->tag == CONSTANT_Utf8);
        assert(nat_name_u8_ptr->tag == CONSTANT_Utf8);
        assert(nat_des_u8_ptr->tag == CONSTANT_Utf8);

        spdlog::info("reslove method {}.{}:{}", (char*)name_u8ptr->bytes,
                     (char*)nat_name_u8_ptr->bytes, (char*)nat_des_u8_ptr->bytes);
    }
}

// void ClassFile::reinterpret() {
//     unordered_map<string, MethodInfo_ptr> method_rec;
//     unordered_map<string, FieldInfo_ptr> field_rec;

//     for (size_t index = 0; index < this->fields_count; index++) {

//     }

//     for (size_t index = 0; index < this->constant_pool_count; index++) {
//         ConstantInfo_ptr cptr = this->constant_pool[index];
//         switch (cptr->tag) {
//             case CONSTANT_Utf8: {
//                 ConstantUtf8_ptr u8ptr = static_cast<ConstantUtf8_ptr>(cptr);
//                 break;
//             }
//             default: {
//                 spdlog::error("No such constant type: {:X}", cptr->tag);
//             }
//         }
//     }
// }
