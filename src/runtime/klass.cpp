#include "runtime/klass.hpp"
#include "classFile/byte_code_reader.hpp"
#include "classFile/class_file.hpp"
#include "runtime/vm_allocator.hpp"
#include <cstdlib>
#include <filesystem>
#include <string>

using namespace rt_jvm_data;
using namespace rt_jvm_memory;
using namespace raw_jvm_data;

AttributeWrapper::AttributeWrapper(const raw_jvm_data::AttributeInfo_ptr aaptr) : aptr(aaptr) {
    // pass
}

MethodWrapper::MethodWrapper(const Klass& kls, const MethodInfo_ptr mptr)
    : mptr(mptr), code(nullptr), code_length(0) {
    for (size_t index = 0; index < mptr->attribute_count; index++) {
        const auto& aptr = &mptr->attributes[index];
        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(kls.constant_pool[aptr->attribute_name_index]);

        assert(name_u8ptr->tag == CONSTANT_Utf8);
        const auto& attr_name =
            std::string_view(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length);

        // spdlog::debug("attribute name: {}", attr_name);

        this->attributes.emplace(std::move(attr_name), aptr);
    }
    if (this->attributes.contains("Code")) {
        const auto& code_attr = this->attributes.at("Code");
        this->code = code_attr.aptr->info;
        this->code_length = code_attr.aptr->attribute_length;
    }
}

FieldWrapper::FieldWrapper(const Klass& kls, const raw_jvm_data::FieldInfo_ptr fptr,
                           const u2 object_field_size, const u2 static_field_offset)
    : fptr(fptr), object_field_offset(object_field_size), static_field_offset(static_field_offset) {
    for (size_t index = 0; index < fptr->attribute_count; index++) {
        const auto& aptr = &fptr->attributes[index];
        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(kls.constant_pool[aptr->attribute_name_index]);

        assert(name_u8ptr->tag == CONSTANT_Utf8);

        const auto& attr_name =
            std::string_view(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length);
        this->attributes.emplace(std::move(attr_name), aptr);
    }
}

Klass_ptr KlassLoader::load_klass(const std::string_view& klass_name) {
    std::filesystem::path p = klass_name;
#ifdef DEBUG
#endif
    // TODO
    return nullptr;
}

std::string Klass::generate_function_id(raw_jvm_data::ConstantUtf8_ptr name_u8ptr,
                                        raw_jvm_data::ConstantUtf8_ptr descriptor_u8ptr) const {
    std::string result(name_u8ptr->length + descriptor_u8ptr->length + 2, '\0');
    result.append(reinterpret_cast<char*>(name_u8ptr->bytes),
                                                    name_u8ptr->length);
    result.push_back(':');
    result.append(reinterpret_cast<char*>(descriptor_u8ptr->bytes), descriptor_u8ptr->length);
    return result;
}

std::string Klass::generate_function_id(raw_jvm_data::ConstantNameAndType_ptr p) const {
    const auto& name_u8ptr = static_cast<ConstantUtf8_ptr>(this->constant_pool[p->name_index]);
    const auto& descriptor_u8ptr =
        static_cast<ConstantUtf8_ptr>(this->constant_pool[p->descriptor_index]);

    return this->generate_function_id(name_u8ptr, descriptor_u8ptr);
}

type Klass::reslove_type(raw_jvm_data::ConstantUtf8_ptr u8ptr) const {
    assert(u8ptr->length > 0);

    const auto& first_ch = u8ptr->bytes[0];

    if (!TYPE_CHAC_REC.contains(first_ch)) {
        spdlog::error("can't reslove type {}",
                      std::string(reinterpret_cast<char*>(u8ptr->bytes), u8ptr->length));
        assert(false);
    }
    return TYPE_CHAC_REC[first_ch];
}

type Klass::reslove_type(raw_jvm_data::ConstantNameAndType_ptr nat_ptr) const {
    return this->reslove_type(
        static_cast<ConstantUtf8_ptr>(this->constant_pool[nat_ptr->descriptor_index]));
}

Klass::Klass(ByteCodeReader& bcr) : raw_jvm_data::ClassFile(bcr) {
    const auto& this_class = static_cast<ConstantClass_ptr>(this->constant_pool[this->this_class]);
    const auto& this_class_name =
        static_cast<ConstantUtf8_ptr>(this->constant_pool[this_class->name_index]);
    this->klass_name =
        std::string_view(reinterpret_cast<char*>(this_class_name->bytes), this_class_name->length);

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
        // spdlog::debug("reslove method {}", function_id);
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
            std::string_view(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length);

        const auto& field_type = this->reslove_type(descriptor_u8ptr);
        const auto& field_size = TYPE_SIZE_REC[field_type];

        this->rt_fields.emplace(std::move(field_id), FieldWrapper(*this, fptr, static_field_offset,
                                                                  object_field_offset));
        fptr->access_flags& ACC_STATIC ? static_field_offset += field_size
                                       : object_field_offset += field_size;
        // spdlog::debug("reslove field {}", field_id);
    }

    for (size_t index = 0; index < this->attributes_count; index++) {
        const auto& aptr = &this->attributes[index];

        const auto& name_u8ptr =
            static_cast<ConstantUtf8_ptr>(this->constant_pool[aptr->attribute_name_index]);

        std::string_view attr_name(reinterpret_cast<char*>(name_u8ptr->bytes), name_u8ptr->length);
        // spdlog::debug("resolve attribute name {}", attr_name);

        this->rt_attributes.emplace(std::move(attr_name), aptr);
    }
}

const std::string_view& Klass::get_klass_name() const {
    return this->klass_name;
}