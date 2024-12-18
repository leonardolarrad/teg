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

#include "teg/core_concepts.h"
#include "teg/fixed_string.h"
#include "teg/serialization_concepts.h"
#include "teg/members_get.h"
#include "teg/compatible.h"
#include "teg/md5.h"

namespace teg {

class schema_analyzer {
public:

    ///  \brief Returns the version count of the given schema.
    ///  
    ///  \tparam T The type representing the schema.
    ///  \return The version count of the schema.
    ///
    template <concepts::serializable T>
    static constexpr auto version_count() -> u64 {
        if constexpr (concepts::compatible<T>) {
            return 0; // Compatible objects cannot be the root type.
        }
        else if constexpr (concepts::serializable_aggregate<T> || concepts::serializable_tuple<T>) {
            auto const get_member_version = []<std::size_t I, class M>() constexpr -> u64 {
                if constexpr (I == 0 && concepts::compatible<M>) {
                    return 0; // Compatible objects cannot be the root type.
                }
                else if constexpr (concepts::compatible<M>) {
                    return M::version;                    
                } else {
                    return 1;                                            
                }
            };

            if constexpr (concepts::serializable_aggregate<T>) {
                std::array<u64, members_count_v<T>> const version_control = 
                    [&get_member_version]<size_t... I>(std::index_sequence<I...>) constexpr {
                        return std::array<u64, members_count_v<T>>{
                            (get_member_version.template operator()
                                <I, std::remove_cvref_t<decltype(teg::get_member<I>(T{}))>>())...
                        };
                    }(std::make_index_sequence<members_count_v<T>>{});

                u64 last_version = 1;
                for (auto const version : version_control) {
                    if (version < last_version || version > last_version + 1) {
                        return 0;
                    }
                    last_version = version;
                }
                return last_version;     
            }
            else {
                std::array<u64, std::tuple_size_v<T>> const version_control = 
                    [&get_member_version]<size_t... I>(std::index_sequence<I...>) constexpr {
                        return std::array<u64, std::tuple_size_v<T>>{
                            (get_member_version.template operator()
                                <I, std::remove_cvref_t<std::tuple_element_t<I, T>>>())...
                        };
                    }(std::make_index_sequence<std::tuple_size_v<T>>{});
                
                u64 last_version = 1;
                for (auto const version : version_control) {
                    if (version < last_version || version > last_version + 1) {
                        return 0;
                    }
                    last_version = version;
                }
                return last_version;
            }
        } 
        else {
            return 1;
        }
    }
};

///  \brief Returns the version count of the given schema.
///  
///  \tparam T The type that represents the schema.
///  \return The version count of the schema.
///
template <concepts::serializable ...T>
requires (sizeof...(T) > 0)
TEG_NODISCARD TEG_INLINE constexpr auto version_count() -> u64 {
    
    if constexpr (sizeof...(T) == 1) {
        return schema_analyzer::template version_count<T...>();
    }
    else {
        return schema_analyzer::template version_count<std::tuple<T...>>();
    }

}

///  \brief Number of versions represented in the type schema.
///
///  A serializable type can be composed of multiple compatibles objects, forming
///  multiple versions of one schema. This constant specifies the number of versions
///  of the given type.
///
template <concepts::serializable... T>
constexpr u64 version_count_v = version_count<T...>();

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
    static constexpr auto tuple_begin = make_fixed_string("(");
    static constexpr auto tuple_end = make_fixed_string(")");
    static constexpr auto variant_begin = make_fixed_string("<");
    static constexpr auto variant_end = make_fixed_string(">");
    
    static constexpr auto fixed_container_prefix = make_fixed_string("#");
    static constexpr auto owning_ptr_prefix = make_fixed_string("*");
    static constexpr auto optional_prefix = make_fixed_string("?");
    static constexpr auto compatible_prefix = make_fixed_string("@");
    static constexpr auto separator = make_fixed_string(" ");
    static constexpr auto union_separator = make_fixed_string("|");

    ///  \brief Returns the token that represents the given built-in type.
    ///
    template <concepts::builtin T>
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
    
    ///  \brief Constructs a string representation of the schema for the given type.
    ///  
    ///  \tparam T The type to construct the schema for.
    ///  \tparam V The version of the type to construct the schema for.
    ///  
    ///  \return A fixed string containing the schema for the given type.
    ///  
    ///  \details Constructs a string representation of the schema for the given type in 
    ///  a generic way, but also takes into account the version of the type.
    ///
    template <concepts::serializable T, u64 V>
    static constexpr auto schema() -> decltype(auto) {
        if constexpr(concepts::serializable_aggregate<T> || concepts::serializable_tuple<T>) {
            auto member_encoder = []<std::size_t I, class M>() constexpr -> decltype(auto) {
                if constexpr (concepts::compatible<M>) {
                    if constexpr (M::version == V) {
                        return encode<M>();
                    }
                    else {
                        return make_fixed_string("");
                    }
                }
                else {
                    if constexpr (V == 1) {
                        return encode<M>();
                    }
                    else {
                        return make_fixed_string("");
                    }
                }
            };
            
            auto encode_next = [&member_encoder]<std::size_t I, class M>() constexpr -> decltype(auto) {
                const auto encode = member_encoder.template operator()<I, M>();
                
                if constexpr (encode == make_fixed_string("")) {
                    return encode;
                }
                else {
                    return encode + separator;
                }
            };

            if constexpr (concepts::serializable_tuple<T>) {
                auto root_schema = tuple_begin +  
                    [&encode_next]<size_t... I>(std::index_sequence<I...>) constexpr {
                        return (
                            (encode_next.template operator()
                                <I, std::remove_cvref_t<std::tuple_element_t<I, T>>>()
                            ) + ...
                        );
                    }(std::make_index_sequence<std::tuple_size_v<T>>{});

                root_schema[root_schema.size() - 1] = tuple_end[0];
                return root_schema;
            }
            else {
                auto root_schema = struct_begin +  
                    [&encode_next]<size_t... I>(std::index_sequence<I...>) constexpr {
                        return (
                            (encode_next.template operator()
                                <I, std::remove_cvref_t<decltype(get_member<I>(T{}))>>()
                            ) + ...
                        );
                    }(std::make_index_sequence<members_count_v<T>>{});

                root_schema[root_schema.size() - 1] = struct_end[0];
                return root_schema;
            }
        }
        else {
            return encode<T>();
        }
    }

    ///  \brief Encodes the schema for a builtin type.
    ///  
    template <concepts::builtin T>
    static constexpr auto encode() -> decltype(auto)    {
        return get_builtin_token<T>();
    }

    ///  \brief Encodes the schema for an aggregate type.
    ///
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

    ///  \brief Encodes the schema for a c-array type.
    ///
    template <concepts::serializable_c_array T>
    static constexpr auto encode() -> decltype(auto) {
        return fixed_container_prefix
             + container_begin
             + encode<std::remove_extent_t<T>>()
             + container_end;
    }

    ///  \brief Encodes the schema for a container type.
    ///
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

    ///  \brief Encodes the schema for an optional type.
    ///  
    template <concepts::serializable_optional T>
    static constexpr auto encode() -> decltype(auto) {
        return optional_prefix + encode<typename T::value_type>();
    }

    ///  \brief Encodes the schema for an owning pointer type.
    ///
    template <concepts::serializable_owning_ptr T>
    static constexpr auto encode() -> decltype(auto) {
        return owning_ptr_prefix + encode<typename T::element_type>();
    }

    ///  \brief Encodes the schema for a variant type.
    ///  
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

    ///  \brief Encodes the schema for a tuple type.
    ///
    template <concepts::serializable_tuple T> 
    static constexpr auto encode() -> decltype(auto) {
        return tuple_begin + 
            []<std::size_t... I>(std::index_sequence<I...>) constexpr {
                auto tuple_schema = 
                    ((encode<std::tuple_element_t<I, T>>() + separator) + ...);
                tuple_schema[tuple_schema.size() - 1] = tuple_end[0];
                return tuple_schema;
            }(std::make_index_sequence<std::tuple_size_v<T>>{}); 
    }

    ///  \brief Encodes the schema for a compatible type.
    ///
    template <concepts::serializable_compatible T>
    static constexpr auto encode() -> decltype(auto) {
        return compatible_prefix + encode<typename T::value_type>();
    }

    ///  \brief Encodes the schema for a user-defined type.
    ///  
    template <concepts::user_defined_serialization T>
    static constexpr auto  encode() -> decltype(auto) {
        return usr_schema(T{});
    }
};

///  \brief Constructs a string representation of the schema for the given type.
///
///  \tparam V The version of the schema.
///  \tparam T The type to construct the schema for.
///
///  \return A fixed string containing the schema for the given type.
///
template <u64 V, class... T>
requires (sizeof...(T) > 0) && (V >= 1)
TEG_NODISCARD TEG_INLINE constexpr auto schema() -> decltype(auto) {
    if constexpr (sizeof...(T) == 1) {
        return schema_encoder::schema<T..., V>();
    }
    else {
        return schema_encoder::schema<std::tuple<T...>, V>();
    }
}

///  \brief Returns the hash table of the schema of the provided types.
///
template <class... T>
TEG_NODISCARD TEG_INLINE constexpr auto schema_hash_table() -> decltype(auto) {

    return []<std::size_t... I>(std::index_sequence<I...>) constexpr {
        return std::array<u32, sizeof...(I)>{
            (md5::hash_u32(schema<I + 1, T...>()))...
        };
    }(std::make_index_sequence<version_count_v<T...>>{});

}

} // namespace teg

#endif // TEG_SCHEMA_H
