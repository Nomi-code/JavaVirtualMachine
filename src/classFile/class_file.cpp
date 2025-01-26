#include "../../include/classFile/class_file.hpp"
#include "../../include/runtime/string_pool.hpp"
#include "java_base.hpp"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>

using namespace raw_jvm_data;
using namespace rt_jvm_data;
using std::string;

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

MethodWrapper::MethodWrapper(MethodInfo_ptr mptr) : mptr(mptr), code(nullptr) {
    // TODO
}

FieldWrapper::FieldWrapper(raw_jvm_data::FieldInfo_ptr fptr, u2 object_field_size,
                           u2 static_field_offset)
    : fptr(fptr), object_field_offset(object_field_size), static_field_offset(static_field_offset) {
    // TODO
}

std::string Klass::generate_function_id(raw_jvm_data::ConstantUtf8_ptr name_u8ptr,
                                        raw_jvm_data::ConstantUtf8_ptr descriptor_u8ptr) {
    return std::string(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length) + ':' +
           std::string(reinterpret_cast<char*>(descriptor_u8ptr->bytes), descriptor_u8ptr->length);
}

std::string Klass::generate_function_id(raw_jvm_data::ConstantNameAndType_ptr p) {
    const auto& name_u8ptr = static_cast<ConstantUtf8_ptr>(this->constant_pool[p->name_index]);
    const auto& descriptor_u8ptr =
        static_cast<ConstantUtf8_ptr>(this->constant_pool[p->descriptor_index]);

    return this->generate_function_id(name_u8ptr, descriptor_u8ptr);
}

type Klass::reslove_type(raw_jvm_data::ConstantUtf8_ptr u8ptr) {
    assert(u8ptr->length > 0);

    const auto& first_ch = u8ptr->bytes[0];

    if (!TYPE_CHAC_REC.contains(first_ch)) {
        spdlog::error("can't reslove type {}",
                      std::string(reinterpret_cast<char*>(u8ptr->bytes), u8ptr->length));
        assert(false);
    }
    return TYPE_CHAC_REC[first_ch];
}

type Klass::reslove_type(raw_jvm_data::ConstantNameAndType_ptr nat_ptr) {
    return this->reslove_type(
        static_cast<ConstantUtf8_ptr>(this->constant_pool[nat_ptr->descriptor_index]));
}

Klass::Klass(std::fstream& in) : raw_jvm_data::ClassFile(in) {
    for (size_t index = 0; index < this->methods_count; index++) {
        const auto& mptr = &this->methods[index];

        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[mptr->name_index]);
        const auto& descriptor_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[mptr->descriptor_index]);

        assert(name_u8ptr->tag == CONSTANT_Utf8);
        assert(descriptor_u8ptr->tag == CONSTANT_Utf8);

        const auto& function_id = generate_function_id(name_u8ptr, descriptor_u8ptr);

        this->rt_methods.emplace(function_id, mptr);
        spdlog::info("reslove method {}", function_id);
    }

    u2 object_field_offset = 0, static_field_offset = 0;
    for (size_t index = 0; index < this->fields_count; index++) {
        const auto& fptr = &this->fields[index];

        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[fptr->name_index]);
        const auto& descriptor_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[fptr->descriptor_index]);

        assert(name_u8ptr->tag == CONSTANT_Utf8);
        assert(descriptor_u8ptr->tag == CONSTANT_Utf8);

        const auto& field_id =
            std::string(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length);

        const auto& field_type = this->reslove_type(descriptor_u8ptr);
        const auto& field_size = TYPE_SIZE_REC[field_type];

        this->rt_fields.emplace(field_id,
                                FieldWrapper(fptr, static_field_offset, object_field_offset));
        fptr->access_flags& ACC_STATIC ? static_field_offset += field_size
                                       : object_field_offset += field_size;
        spdlog::info("reslove field {}", field_id);
    }
}