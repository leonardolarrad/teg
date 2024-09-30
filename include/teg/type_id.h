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

#pragma once
#include <cstdint>
#include <concepts>

#include "concepts.h"
#include "visitor.h"
#include "fixed_string.h"
#include "declval.h"

namespace teg {

enum class type_id : char {
    id_bool = 1,
    id_carray,
    id_int8,
    id_int16,
    id_int32,
    id_int64,
    id_uint8,
    id_uint16,
    id_uint32,
    id_uint64,
    id_char8,
    id_char16,
    id_char32,
    id_float32,
    id_float64,
        
    id_optional,
    id_owning_pointer,
    id_variant,
    
    id_fixed_container,
    id_container,
    id_associative_container,
    id_map_container,

    id_class,
    id_struct      = id_class,
    id_union_t     = id_class,
    id_class_begin,
    id_class_end,

    id_separator
};

template <integral T> 
constexpr type_id int_type_id() 
{
    // Signed.
    if constexpr      (std::is_same_v<T, std::int8_t>)    return type_id::id_int8;
    else if constexpr (std::is_same_v<T, std::int16_t>)   return type_id::id_int16;
    else if constexpr (std::is_same_v<T, std::int32_t>)   return type_id::id_int32;
    else if constexpr (std::is_same_v<T, std::int64_t>)   return type_id::id_int64;
    // Unsigned. 
    else if constexpr (std::is_same_v<T, std::uint8_t>)   return type_id::id_uint8;
    else if constexpr (std::is_same_v<T, std::uint16_t>)  return type_id::id_uint16;
    else if constexpr (std::is_same_v<T, std::uint32_t>)  return type_id::id_uint32;
    else if constexpr (std::is_same_v<T, std::uint64_t>)  return type_id::id_uint64;
    // Char. 
    else if constexpr (std::is_same_v<T, char8_t>)        return type_id::id_char8;
    else if constexpr (std::is_same_v<T, char16_t>)       return type_id::id_char16;
    else if constexpr (std::is_same_v<T, char32_t>)       return type_id::id_char32;
    
    // Unreachable.
    else static_assert(!sizeof(T), "Unknown integral type.");    
}

template <floating_point T> 
constexpr type_id float_type_id() 
{
    if constexpr      (std::is_same_v<T, float>)   return type_id::id_float32;
    else if constexpr (std::is_same_v<T, double>)  return type_id::id_float64;
    
    // Unreachable.
    else static_assert(!sizeof(T), "Unknown floating point type.");    
}

template <typename T>
constexpr type_id get_type_id() 
{
    if constexpr      (std::is_integral_v<T>)         return int_type_id<T>();
    else if constexpr (std::is_floating_point_v<T>)   return float_type_id<T>();
    else if constexpr (c_array<T>)                    return type_id::id_carray;
    else if constexpr (optional<T>)                   return type_id::id_optional;
    else if constexpr (owning_pointer<T>)             return type_id::id_owning_pointer;
    else if constexpr (container<T>)                  return type_id::id_container;
    else if constexpr (variant<T>)                    return type_id::id_variant;
    else if constexpr (is_class<T>)                   return type_id::id_class;
    
    // Unreachable.
    else static_assert(!sizeof(T), "Unsupported type.");
}

template <class T>
constexpr decltype(auto) get_type_code() 
{
    using type = std::remove_cvref_t<T>;
    constexpr auto separator = fixed_string<1>{ (char)type_id::id_separator };
    constexpr auto id = get_type_id<type>();

    if constexpr (id != type_id::id_class) {
        return fixed_string<1>{ (char)id } + separator;
    }
    else {
        return 
            fixed_string<1>{ (char)type_id::id_class_begin }
            + visit_members(
                type{},
                [&](auto&&... members) {
                    return ((get_type_code<decltype(members)>()) + ...);
                }
            )
            + fixed_string<1>{ (char)type_id::id_class_end }
            + separator;            
    }
}

} // namespace teg