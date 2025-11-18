#pragma once

#include "java_base.hpp"
#include "runtime/oop.hpp"
#include "string_pool.hpp"
#include "../classFile/class_file.hpp"
#include <cassert>

namespace rt_jvm_data {

    enum class raw_value_type { Jbyte, Jchar, Jshort, Jint, Jfloat, Jdouble, Jreference };

    static std::unordered_map<raw_value_type, raw_jvm_type::u1> TYPE_SIZE_REC{
        {raw_value_type::Jbyte, 1},     {raw_value_type::Jchar, 2},  {raw_value_type::Jshort, 2},
        {raw_value_type::Jint, 4},      {raw_value_type::Jfloat, 4}, {raw_value_type::Jdouble, 8},
        {raw_value_type::Jreference, 8}};

    static std::unordered_map<char, raw_value_type> TYPE_CHAC_REC{
        {'B', raw_value_type::Jbyte},      {'C', raw_value_type::Jchar},
        {'S', raw_value_type::Jshort},     {'I', raw_value_type::Jint},
        {'F', raw_value_type::Jfloat},     {'D', raw_value_type::Jdouble},
        {'L', raw_value_type::Jreference}, {'[', raw_value_type::Jreference}};

    [[nodiscard]] inline raw_jvm_type::u1 type_size_of(raw_value_type t) noexcept {
        using u1 = raw_jvm_type::u1;

        switch (t) {
            case raw_value_type::Jbyte:
                return u1{1};
            case raw_value_type::Jchar:
                return u1{2};
            case raw_value_type::Jshort:
                return u1{2};
            case raw_value_type::Jint:
                return u1{4};
            case raw_value_type::Jfloat:
                return u1{4};
            case raw_value_type::Jdouble:
                return u1{8};
            case raw_value_type::Jreference:
                return u1{8};
        }

        assert(false && "unknown raw_value_type");
        return u1{0};
    }

    [[nodiscard]] inline raw_value_type char_to_raw_type(char c) {
        switch (c) {
            case 'B':
                return raw_value_type::Jbyte;
            case 'C':
                return raw_value_type::Jchar;
            case 'S':
                return raw_value_type::Jshort;
            case 'I':
                return raw_value_type::Jint;
            case 'F':
                return raw_value_type::Jfloat;
            case 'D':
                return raw_value_type::Jdouble;
            case 'L':
            case '[':
                return raw_value_type::Jreference;
            default:
                assert(false && "unknown raw type char");
                return raw_value_type::Jint;
        }
    }

    [[nodiscard]] inline char raw_type_to_char(raw_value_type t) {
        switch (t) {
            case raw_value_type::Jbyte:
                return 'B';
            case raw_value_type::Jchar:
                return 'C';
            case raw_value_type::Jshort:
                return 'S';
            case raw_value_type::Jint:
                return 'I';
            case raw_value_type::Jfloat:
                return 'F';
            case raw_value_type::Jdouble:
                return 'D';
            case raw_value_type::Jreference:
                return 'L'; // 统一用 L 代表引用
            default:
                assert(false && "unknown raw_value_type");
                return 'V';
        }
    }

    struct RuntimeConstantItem;
    struct MethodWrapper;
    struct FieldWrapper;
    struct AttributeWrapper;
    class InstanceKlass;

    using RuntimeConstantItem_ptr = RuntimeConstantItem*;
    using MethodWrapper_ptr = MethodWrapper*;
    using FieldWrapper_ptr = FieldWrapper*;
    using AttributeWrapper_ptr = AttributeWrapper*;
    using Klass_ptr = InstanceKlass*;

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
        MethodWrapper(const InstanceKlass&, const raw_jvm_data::MethodInfo_ptr);
    };

    struct FieldWrapper {
        raw_jvm_data::FieldInfo_ptr fptr;
        raw_jvm_type::u2 object_field_offset;
        raw_jvm_type::u2 static_field_offset;
        std::unordered_map<std::string, AttributeWrapper> attributes;
        FieldWrapper(const InstanceKlass&, const raw_jvm_data::FieldInfo_ptr,
                     const raw_jvm_type::u2, const raw_jvm_type::u2);
    };

    enum class KlassType {
        Instance,
        Array,
        Primitive, // int double...
    };

    class RawKlass;
    class InstanceKlass;
    class ArrayKlass;
    class PrimitiveKlass;

    using RawKlass_ptr = RawKlass*;
    using InstanceKlass_ptr = InstanceKlass*;
    using ArrayKlass_ptr = ArrayKlass*;
    using PrimitiveKlass_ptr = PrimitiveKlass*;

    class RawKlass {
      protected:
        KlassType kls_type;
        oop::Ref mirror_ref;

        std::string klass_name;
        std::string wrapper_name;

      public:
        std::string get_klass_name() const noexcept {
            return klass_name;
        }

        std::string get_wrapper_name() const noexcept {
            return wrapper_name;
        }

        KlassType get_klass_type() const noexcept {
            return kls_type;
        }

        void set_ref(oop::Ref rf) noexcept {
            mirror_ref = rf;
        }

        oop::Ref get_ref() const noexcept {
            return mirror_ref;
        }
    };

    template <class T>
    concept ConstantItemPtr = std::derived_from<std::remove_pointer_t<std::remove_cvref_t<T>>,
                                                raw_jvm_data::ConstantInfo>;

    class InstanceKlass : public raw_jvm_data::ClassFile, public RawKlass {
      private:
        friend struct MethodWrapper;
        friend struct FieldWrapper;
        friend struct AttributeWrapper;
        friend class ArrayKlass;

        std::unordered_map<raw_jvm_type::u2, RuntimeConstantItem_ptr> translated_constant_pool;
        std::unordered_map<std::string, MethodWrapper> rt_methods;
        std::unordered_map<std::string, FieldWrapper> rt_fields;
        std::unordered_map<std::string, AttributeWrapper> rt_attributes;

        std::string generate_function_id(raw_jvm_data::ConstantUtf8_ptr name_u8ptr,
                                         raw_jvm_data::ConstantUtf8_ptr descri_u8ptr);
        std::string generate_function_id(raw_jvm_data::ConstantNameAndType_ptr);

        enum raw_value_type reslove_type(raw_jvm_data::ConstantUtf8_ptr);
        enum raw_value_type reslove_type(raw_jvm_data::ConstantNameAndType_ptr);

        template <ConstantItemPtr T> T get_cp_item(const int index) {
            assert(index < this->constant_pool_count);

            return static_cast<T>(this->constant_pool[index]);
        }

      public:
        InstanceKlass(std::fstream& in);

      protected:
        std::string utf8cp_to_string(raw_jvm_data::ConstantUtf8_ptr ptr);
    };

    class PrimitiveKlass : public RawKlass {
      private:
        raw_value_type type;

        std::string generate_primitive_klass_name();

      public:
        PrimitiveKlass(raw_value_type tp) noexcept : type(tp) {
            this->kls_type = KlassType::Primitive;
            this->klass_name = generate_primitive_klass_name();
        }

        raw_value_type get_raw_type() const noexcept {
            return type;
        }
    };

    class ArrayKlass : public RawKlass {
      private:
        friend class InstanceKlass;

        const RawKlass_ptr klass_ptr;
        const int dim;

        std::string wrapper_name;

        std::string generate_wrapper_name() {
            std::string wrapper = "class " + std::string('[', dim);
            char appendix = 'L';
            if (klass_ptr->get_klass_type() == KlassType::Primitive) {
                appendix = raw_type_to_char(
                    reinterpret_cast<PrimitiveKlass_ptr>(klass_ptr)->get_raw_type());
					wrapper += appendix;
			} else {
                wrapper += appendix;
				wrapper += reinterpret_cast<PrimitiveKlass_ptr>(klass_ptr)->get_klass_name();
            }
            return wrapper;
        }

      public:
        ArrayKlass(const RawKlass_ptr klass_ptr, const int dim_) noexcept
            : klass_ptr(klass_ptr), dim(dim_) {
            kls_type = KlassType::Array;
            this->klass_name = klass_ptr->get_klass_name();
        }

        std::string get_wrapper_name() noexcept {
            if (wrapper_name.empty()) {
                generate_wrapper_name();
			}
            return wrapper_name;
        }
    };
}; // namespace rt_jvm_data