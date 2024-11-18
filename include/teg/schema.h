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

#include "core_concepts.h"
#include "fixed_string.h"
#include "serialization_concepts.h"
#include "members_visitor.h"

namespace teg {
    
class schema_encoder {
public:
    enum class builtin_id : char {
        ch = 'c',
        u8 = 'b', u16 = 's', u32 = 'i', u64 = 'l',
        i8 = 'B', i16 = 'S', i32 = 'I', i64 = 'L',
        f32 = 'f', f64 = 'F'
    };

    enum class struct_state : char {
        begin = '{', end = '}'
    };

    enum class container_state : char {
        begin = '[', end = ']'
    };

    static constexpr char fixed_container_symbol = '#';

    template <concepts::serializable_builtin T>
    static constexpr auto get_builtin_id() -> builtin_id {
        using type = std::conditional_t<std::is_enum_v<T>, std::underlying_type<T>, T>;

        if constexpr (std::is_same_v<type, char>)                              return builtin_id::ch;
        else if constexpr (std::unsigned_integral<type> && sizeof(type) == 1)  return builtin_id::u8;
        else if constexpr (std::unsigned_integral<type> && sizeof(type) == 2)  return builtin_id::u16;
        else if constexpr (std::unsigned_integral<type> && sizeof(type) == 4)  return builtin_id::u32;
        else if constexpr (std::unsigned_integral<type> && sizeof(type) == 8)  return builtin_id::u64;
        else if constexpr (std::signed_integral<type> && sizeof(type) == 1)    return builtin_id::i8;
        else if constexpr (std::signed_integral<type> && sizeof(type) == 2)    return builtin_id::i16;
        else if constexpr (std::signed_integral<type> && sizeof(type) == 4)    return builtin_id::i32;
        else if constexpr (std::signed_integral<type> && sizeof(type) == 8)    return builtin_id::i64;
        else if constexpr (std::floating_point<type> && sizeof(type) == 4)     return builtin_id::f32;
        else if constexpr (std::floating_point<type> && sizeof(type) == 8)     return builtin_id::f64;
        else if constexpr (std::is_enum_v<T> && sizeof(T) == 1)                return builtin_id::i8;
        else if constexpr (std::is_enum_v<T> && sizeof(T) == 2)                return builtin_id::i16;
        else if constexpr (std::is_enum_v<T> && sizeof(T) == 4)                return builtin_id::i32;
        else if constexpr (std::is_enum_v<T> && sizeof(T) == 8)                return builtin_id::i64;
        else static_assert(!sizeof(type), "Unsupported builtin type");
    }

    template <concepts::serializable_builtin T>
    static constexpr auto encode() -> decltype(auto) {
        return fixed_string<1>{(char)get_builtin_id<T>()};
    }

    template <concepts::serializable_aggregate T>
    static constexpr auto encode() -> decltype(auto) {
        return visit_members(
            [](auto&&... members) {
                return fixed_string<1>{(char)struct_state::begin} 
                     + (encode<std::remove_cvref_t<decltype(members)>>() + ...)
                     + fixed_string<1>{(char)struct_state::end};
            },
            T{}
        );
    }

    template <concepts::serializable_container T>
    static constexpr auto encode() -> decltype(auto) {
        if constexpr (concepts::fixed_size_container<T>) {
            return fixed_string<1>{(char)fixed_container_symbol}
                 + fixed_string<1>{(char)container_state::begin}
                 + encode<typename T::value_type>()
                 + fixed_string<1>{(char)container_state::end};
        } else {
            return fixed_string<1>{(char)container_state::begin}
                 + encode<typename T::value_type>()
                 + fixed_string<1>{(char)container_state::end};
        }
    }
};

template <class T>
constexpr auto schema() -> decltype(auto) {
    return schema_encoder::encode<T>();
}

} // namespace teg

#endif // TEG_SCHEMA_H
