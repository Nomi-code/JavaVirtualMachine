#pragma once

#include <memory>
#include <unordered_map>
#include <filesystem>
#include <cstdlib>

#include "java_base.hpp"
#include "string_pool.hpp"
#include "classFile/class_file.hpp"

namespace rt_jvm_data {

    enum class type { Jbyte, Jchar, Jshort, Jint, Jfloat, Jdouble, Jreference };

    static std::unordered_map<type, raw_jvm_type::u1> TYPE_SIZE_REC{
        {type::Jbyte, 1},  {type::Jchar, 2},   {type::Jshort, 2},    {type::Jint, 4},
        {type::Jfloat, 4}, {type::Jdouble, 8}, {type::Jreference, 8}};

    static std::unordered_map<char, type> TYPE_CHAC_REC{
        {'B', type::Jbyte},      {'C', type::Jchar},     {'S', type::Jshort},
        {'I', type::Jint},       {'F', type::Jfloat},    {'D', type::Jdouble},
        {'L', type::Jreference}, {'[', type::Jreference}};

    struct RuntimeConstantItem;
    struct MethodWrapper;
    struct FieldWrapper;
    struct AttributeWrapper;
    class Klass;
    class KlassLoader;

    using RuntimeConstantItem_ptr = RuntimeConstantItem*;
    using MethodWrapper_ptr = MethodWrapper*;
    using FieldWrapper_ptr = FieldWrapper*;
    using AttributeWrapper_ptr = AttributeWrapper*;
    using Klass_ptr = Klass*;
    using KlassLoader_ptr = KlassLoader*;

    struct RuntimeConstantItem {};

    struct RuntimeMethodRef : public RuntimeConstantItem {};

    struct AttributeWrapper {
        raw_jvm_data::AttributeInfo_ptr aptr;
        AttributeWrapper(const raw_jvm_data::AttributeInfo_ptr);
    };

    struct MethodWrapper {
        raw_jvm_data::MethodInfo_ptr mptr;
        // late init
        raw_jvm_type::u1_ptr code;
        raw_jvm_type::u2 code_length;
        std::unordered_map<std::string, AttributeWrapper> attributes;
        MethodWrapper(const Klass&, const raw_jvm_data::MethodInfo_ptr);
    };

    struct FieldWrapper {
        raw_jvm_data::FieldInfo_ptr fptr;
        raw_jvm_type::u2 object_field_offset;
        raw_jvm_type::u2 static_field_offset;
        std::unordered_map<std::string, AttributeWrapper> attributes;
        FieldWrapper(const Klass&, const raw_jvm_data::FieldInfo_ptr, const raw_jvm_type::u2,
                     const raw_jvm_type::u2);
    };

    class KlassLoader {
      private:
        static std::filesystem::path basic_path;
        std::unordered_map<std::string, std::shared_ptr<Klass>> loaded_klasses;

      public:
        KlassLoader() = default;
        Klass_ptr load_klass(const std::string& klass_name);
    };

    class Klass : public raw_jvm_data::ClassFile {
      private:
        friend struct MethodWrapper;
        friend struct FieldWrapper;
        friend struct AttributeWrapper;

        std::string klass_name;
        KlassLoader_ptr klass_loader;

        std::unordered_map<raw_jvm_type::u2, RuntimeConstantItem_ptr> translated_constant_pool;
        std::unordered_map<std::string, MethodWrapper> rt_methods;
        std::unordered_map<std::string, FieldWrapper> rt_fields;
        std::unordered_map<std::string, AttributeWrapper> rt_attributes;

        std::string generate_function_id(raw_jvm_data::ConstantUtf8_ptr name_u8ptr,
                                         raw_jvm_data::ConstantUtf8_ptr descri_u8ptr);
        std::string generate_function_id(raw_jvm_data::ConstantNameAndType_ptr);

        type reslove_type(raw_jvm_data::ConstantUtf8_ptr);
        type reslove_type(raw_jvm_data::ConstantNameAndType_ptr);

      public:
        Klass(std::fstream& in);
        const std::string& get_klass_name() const;
    };

}; // namespace rt_jvm_data