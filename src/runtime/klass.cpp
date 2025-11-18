#include "runtime/klass.hpp"
#include "classFile/class_file.hpp"
#include <cassert>
#include <spdlog/spdlog.h>

using namespace rt_jvm_data;
using namespace raw_jvm_data;

AttributeWrapper::AttributeWrapper(const raw_jvm_data::AttributeInfo_ptr aaptr) : aptr(aaptr) {
    // TODO
}

MethodWrapper::MethodWrapper(const InstanceKlass& kls, const MethodInfo_ptr mptr)
    : mptr(mptr), code(nullptr), code_length(0) {
    for (size_t index = 0; index < mptr->attribute_count; index++) {
        const auto& aptr = &mptr->attributes[index];
        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(kls.constant_pool[aptr->attribute_name_index]);

        assert(name_u8ptr->tag == CONSTANT_Utf8);

        std::string attr_name(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length);
        // spdlog::info("attribute name: {}", attr_name);
        this->attributes.emplace(std::move(attr_name), aptr);
    }
    if (this->attributes.contains("Code")) {
        const auto& code_attr = this->attributes.at("Code");
        this->code = code_attr.aptr->info;
        this->code_length = code_attr.aptr->attribute_length;
    }
}

FieldWrapper::FieldWrapper(const InstanceKlass& kls, const raw_jvm_data::FieldInfo_ptr fptr,
                           const u2 object_field_size, const u2 static_field_offset)
    : fptr(fptr), object_field_offset(object_field_size), static_field_offset(static_field_offset) {
    for (size_t index = 0; index < fptr->attribute_count; index++) {
        const auto& aptr = &fptr->attributes[index];
        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(kls.constant_pool[aptr->attribute_name_index]);

        assert(name_u8ptr->tag == CONSTANT_Utf8);

        std::string attr_name(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length);
        this->attributes.emplace(std::move(attr_name), aptr);
    }
}

std::string InstanceKlass::generate_function_id(raw_jvm_data::ConstantUtf8_ptr name_u8ptr,
                                                raw_jvm_data::ConstantUtf8_ptr descriptor_u8ptr) {
    return std::string(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length) + ':' +
           std::string(reinterpret_cast<char*>(descriptor_u8ptr->bytes), descriptor_u8ptr->length);
}

std::string InstanceKlass::generate_function_id(raw_jvm_data::ConstantNameAndType_ptr p) {
    const auto& name_u8ptr = static_cast<ConstantUtf8_ptr>(this->constant_pool[p->name_index]);
    const auto& descriptor_u8ptr =
        static_cast<ConstantUtf8_ptr>(this->constant_pool[p->descriptor_index]);

    return this->generate_function_id(name_u8ptr, descriptor_u8ptr);
}

raw_value_type InstanceKlass::reslove_type(raw_jvm_data::ConstantUtf8_ptr u8ptr) {
    assert(u8ptr->length > 0);

    const auto& first_ch = u8ptr->bytes[0];

    if (!TYPE_CHAC_REC.contains(first_ch)) {
        spdlog::error("can't reslove type {}",
                      std::string(reinterpret_cast<char*>(u8ptr->bytes), u8ptr->length));
        assert(false);
    }
    return TYPE_CHAC_REC[first_ch];
}

raw_value_type InstanceKlass::reslove_type(raw_jvm_data::ConstantNameAndType_ptr nat_ptr) {
    return this->reslove_type(
        static_cast<ConstantUtf8_ptr>(this->constant_pool[nat_ptr->descriptor_index]));
}

InstanceKlass::InstanceKlass(std::fstream& in) : raw_jvm_data::ClassFile(in) {
    for (size_t index = 0; index < this->methods_count; index++) {
        const auto& mptr = &this->methods[index];

        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[mptr->name_index]);
        const auto& descriptor_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[mptr->descriptor_index]);

        assert(name_u8ptr->tag == CONSTANT_Utf8);
        assert(descriptor_u8ptr->tag == CONSTANT_Utf8);

        const auto& function_id = generate_function_id(name_u8ptr, descriptor_u8ptr);

        this->rt_methods.emplace(function_id, MethodWrapper(*this, mptr));
        // spdlog::info("reslove method {}", function_id);
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

        this->rt_fields.emplace(
            field_id, FieldWrapper(*this, fptr, static_field_offset, object_field_offset));
        fptr->access_flags& ACC_STATIC ? static_field_offset += field_size
                                       : object_field_offset += field_size;
        // spdlog::info("reslove field {}", field_id);
    }

    for (size_t index = 0; index < this->attributes_count; index++) {
        const auto& aptr = &this->attributes[index];

        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[aptr->attribute_name_index]);

        std::string attr_name(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length);

        // spdlog::info("resolve attribute name {}", attr_name);

        this->rt_attributes.emplace(std::move(attr_name), aptr);
    }

    ConstantClass_ptr this_kls = get_cp_item<ConstantClass_ptr>(this->this_class);
    ConstantUtf8_ptr this_kls_name = get_cp_item<ConstantUtf8_ptr>(this_kls->name_index);
    this->klass_name = utf8cp_to_string(this_kls_name);
}

std::string InstanceKlass::utf8cp_to_string(raw_jvm_data::ConstantUtf8_ptr ptr) {
    return std::string(reinterpret_cast<char*>(ptr->bytes), ptr->length);
}

std::string PrimitiveKlass::generate_primitive_klass_name() {
    switch (type) {
        case raw_value_type::Jbyte:
            return "byte";
        case raw_value_type::Jchar:
            return "char";
        case raw_value_type::Jshort:
            return "int";
        case raw_value_type::Jint:
            return "int";
        case raw_value_type::Jfloat:
            return "float";
        case raw_value_type::Jdouble:
            return "double";
        default: {
            spdlog::error("can't handle type: {}", raw_type_to_char(type));
            assert(false);
        }
    }
}

