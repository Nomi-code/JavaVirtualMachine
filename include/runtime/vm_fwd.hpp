#pragma once

#include "oop.hpp"
#include "klass.hpp"
#include <cassert>
#include <type_traits>

namespace vm::memory {
    template <class T>
    concept Field = std::same_as<std::remove_cvref_t<T>, raw_jvm_type::u1> ||
        std::same_as<std::remove_cvref_t<T>, raw_jvm_type::u2> ||
        std::same_as<std::remove_cvref_t<T>, raw_jvm_type::u4> ||
        std::same_as<std::remove_cvref_t<T>, raw_jvm_type::u8> ||
        std::same_as<std::remove_cvref_t<T>, oop::Ref>;

    template <class T>
    concept RWoop = std::same_as<std::remove_cvref_t<T>, oop::InstanceOop> ||
        std::same_as<std::remove_cvref_t<T>, oop::ArrayOop>;

    template <Field T, RWoop Q> void write(Q& oop, const T& var, const std::size_t off) {
        using U = std::remove_cvref_t<T>;
        std::size_t offset = off;

        if constexpr (std::same_as<std::remove_cvref_t<Q>, oop::ArrayOop>) {
            auto kls_ptr = static_cast<rt_jvm_data::PrimitiveKlass_ptr>(oop.kls_ptr);
            offset *= rt_jvm_data::type_size_of(kls_ptr->get_raw_type());
        }

        auto* bytes = reinterpret_cast<std::byte*>(oop.bytes);
        auto* addr = bytes + offset;
        *reinterpret_cast<U*>(addr) = var;
    }

    template <Field T, RWoop Q> T read(Q& oop, const std::size_t off) {
        using U = std::remove_cvref_t<T>;

        std::size_t offset = off;

        if constexpr (std::same_as<std::remove_cvref_t<Q>, oop::ArrayOop>) {
            auto kls_ptr = static_cast<rt_jvm_data::PrimitiveKlass_ptr>(oop.kls_ptr);
            offset *= rt_jvm_data::type_size_of(kls_ptr->get_raw_type());
        }

        auto* bytes = reinterpret_cast<std::byte*>(oop.bytes);
        auto* addr = bytes + offset;
        return *reinterpret_cast<U*>(addr);
    }
}; // namespace vm::memory