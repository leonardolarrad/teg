///  Copyright (c) 2024 Adrian & Leonardo Larrad.
///  
///  This software is provided 'as-is', without any express or implied warranty. 
///  In no event will the authors be held liable for any damages arising from
///  the use of this software.
///  
///  Permission is granted to anyone to use this software for any purpose,including
///  commercial applications, and to alter it and redistribute it freely, subject
///  to the following restrictions:
///  
///  1. The origin of this software must not be misrepresented; you must not claim
///     that you wrote the original software. If you use this software in a product, 
///     an acknowledgment in the product documentation would be appreciated but is 
///     not required.
///  2. Altered source versions must be plainly marked as such, and must not be
///     misrepresented as being the original software.
///  3. This notice may not be removed or altered from any source distribution.

#ifndef TEG_SCHEMA_H
#define TEG_SCHEMA_H

#include <utility>

#include "teg/core_concepts.h"
#include "teg/fixed_string.h"
#include "teg/serialization_concepts.h"
#include "teg/members_visitor.h"
#include "teg/compatible.h"

namespace teg {

class schema_analyzer {
public:
    template <concepts::serializable T>
    static constexpr auto version_count() -> u64 {
        if constexpr (concepts::compatible<T>) {
            return 0; // Compatible objects cannot be the root type.
        if constexpr (concepts::aggregate<T>) {
            return visit_members(
                [](auto&&... members) constexpr {
                    
                },
                T{}
            );
        }
        } else {
            return 1;
        }
    }
};

template <concepts::serializable T>
teg_nodiscard teg_inline constexpr auto version_count() -> u64 {
    return schema_analyzer::template version_count<T>();
}

template <concepts::serializable T>
constexpr inline u64 version_count_v = version_count<T>();

class schema_encoder {
public:
    static constexpr auto builtin_char = make_fixed_string("char");
    static constexpr auto builtin_u8 = make_fixed_string("u8");
    static constexpr auto builtin_u16 = make_fixed_string("u16");
    static constexpr auto builtin_u32 = make_fixed_string("u32");
    static constexpr auto builtin_u64 = make_fixed_string("u64");
    static constexpr auto builtin_i8 = make_fixed_string("i8");
    static constexpr auto builtin_i16 = make_fixed_string("i16");
    static constexpr auto builtin_i32 = make_fixed_string("i32");
    static constexpr auto builtin_i64 = make_fixed_string("i64");
    static constexpr auto builtin_f32 = make_fixed_string("f32");
    static constexpr auto builtin_f64 = make_fixed_string("f64");

    static constexpr auto struct_begin = make_fixed_string("{");
    static constexpr auto struct_end = make_fixed_string("}");
    static constexpr auto container_begin = make_fixed_string("[");
    static constexpr auto container_end = make_fixed_string("]");
    static constexpr auto variant_begin = make_fixed_string("<");
    static constexpr auto variant_end = make_fixed_string(">");
    
    static constexpr auto fixed_container_prefix = make_fixed_string("#");
    static constexpr auto owning_ptr_prefix = make_fixed_string("*");
    static constexpr auto optional_prefix = make_fixed_string("?");
    static constexpr auto separator = make_fixed_string(" ");
    static constexpr auto union_separator = make_fixed_string("|");

    template <concepts::serializable_builtin T>
    static constexpr auto get_builtin_token() -> decltype(auto) {
        using type = std::conditional_t<std::is_enum_v<T>, std::underlying_type<T>, T>;

        if constexpr (std::is_same_v<type, char>)                              return builtin_char;
        else if constexpr (std::unsigned_integral<type> && sizeof(type) == 1)  return builtin_u8;  
        else if constexpr (std::unsigned_integral<type> && sizeof(type) == 2)  return builtin_u16;
        else if constexpr (std::unsigned_integral<type> && sizeof(type) == 4)  return builtin_u32;
        else if constexpr (std::unsigned_integral<type> && sizeof(type) == 8)  return builtin_u64;
        else if constexpr (std::signed_integral<type> && sizeof(type) == 1)    return builtin_i8;
        else if constexpr (std::signed_integral<type> && sizeof(type) == 2)    return builtin_i16;
        else if constexpr (std::signed_integral<type> && sizeof(type) == 4)    return builtin_i32;
        else if constexpr (std::signed_integral<type> && sizeof(type) == 8)    return builtin_i64;
        else if constexpr (std::floating_point<type> && sizeof(type) == 4)     return builtin_f32;
        else if constexpr (std::floating_point<type> && sizeof(type) == 8)     return builtin_f64;
        else if constexpr (std::is_enum_v<T> && sizeof(T) == 1)                return builtin_i8;
        else if constexpr (std::is_enum_v<T> && sizeof(T) == 2)                return builtin_i16;
        else if constexpr (std::is_enum_v<T> && sizeof(T) == 4)                return builtin_i32;
        else if constexpr (std::is_enum_v<T> && sizeof(T) == 8)                return builtin_i64;
        else static_assert(!sizeof(type), "Unsupported builtin type");
    }

    template <concepts::serializable_builtin T>
    static constexpr auto encode() -> decltype(auto)    {
        return get_builtin_token<T>();
    }

    template <concepts::serializable_aggregate T>
    static constexpr auto encode() -> decltype(auto) {
        return visit_members(
            [](auto&&... members) constexpr {
                auto aggregate_schema = struct_begin + (
                    (encode<std::remove_cvref_t<decltype(members)>>() 
                    + separator) + ...
                );
                
                aggregate_schema[aggregate_schema.size() - 1] = struct_end[0];
                return aggregate_schema;
            },
            T{}
        );
    }

    template <concepts::serializable_c_array T>
    static constexpr auto encode() -> decltype(auto) {
        return fixed_container_prefix
             + container_begin
             + encode<std::remove_all_extents_t<T>>()
             + container_end;
    }

    template <concepts::serializable_container T>
    static constexpr auto encode() -> decltype(auto) {
        if constexpr (concepts::fixed_size_container<T>) {
            return fixed_container_prefix
                 + container_begin
                 + encode<typename T::value_type>()
                 + container_end;
        } else {
            return container_begin
                 + encode<typename T::value_type>()
                 + container_end;
        }
    }

    template <concepts::serializable_optional T>
    static constexpr auto encode() -> decltype(auto) {
        return optional_prefix + encode<typename T::value_type>();
    }

    template <concepts::serializable_owning_ptr T>
    static constexpr auto encode() -> decltype(auto) {
        return owning_ptr_prefix + encode<typename T::element_type>();
    }

    template <concepts::serializable_variant T>
    static constexpr auto encode() -> decltype(auto) {

        return variant_begin + 
            []<std::size_t... I>(std::index_sequence<I...>) constexpr {
               auto variant_schema = 
                   ((encode<std::variant_alternative_t<I, T>>() + union_separator) + ...);

               variant_schema[variant_schema.size() - 1] = variant_end[0];
               return variant_schema;

            }(std::make_index_sequence<std::variant_size_v<T>>{}); 
    }

    template <concepts::serializable_tuple T>
    static constexpr auto encode() -> decltype(auto) {
        return struct_begin + 
            []<std::size_t... I>(std::index_sequence<I...>) constexpr {
                auto tuple_schema = 
                    ((encode<std::tuple_element_t<I, T>>() + separator) + ...);
                tuple_schema[tuple_schema.size() - 1] = struct_end[0];
                return tuple_schema;
            }(std::make_index_sequence<std::tuple_size_v<T>>{}); 
    }
};

template <class T>
teg_nodiscard teg_inline constexpr auto schema() -> decltype(auto) {
    return make_fixed_string("teg:") + schema_encoder::encode<T>();
}

} // namespace teg

#endif // TEG_SCHEMA_H
