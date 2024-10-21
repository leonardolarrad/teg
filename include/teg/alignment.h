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

#ifndef TEG_ALIGNMENT_H
#define TEG_ALIGNMENT_H

#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>

#include "core_concepts.h"
#include "members_visitor.h"

namespace teg {

template <class T>
    requires (concepts::trivially_copyable<T>) 
          && (!concepts::aggregate<T>)
consteval auto packed_sizeof() -> std::size_t {
    return sizeof(T);
}

template <class T>
    requires (concepts::trivially_copyable<T>) 
          && (concepts::aggregate<T>)
consteval auto packed_sizeof() -> std::size_t {
    return visit_members(
        [](auto&&... members) constexpr {
            return (packed_sizeof<std::remove_cvref_t<decltype(members)>>() + ...);
        },
        T{}
    );
}

template <class T>
    requires (concepts::trivially_copyable<T>)
constexpr bool has_padding() {
    return !std::has_unique_object_representations_v<T>;
}

template <class T>
    requires (concepts::trivially_copyable<T>)
          && (concepts::fundamental<T> || concepts::is_enum<T>)
constexpr bool has_padding() {
    return false;
}

template <class T>
    requires (concepts::trivially_copyable<T>) 
          && (concepts::c_array<T>)
constexpr bool has_padding() {
    return has_padding<std::remove_cvref_t<std::remove_all_extents_t<T>>>();
}

template <class T>
    requires (concepts::trivially_copyable<T>) 
          && (concepts::aggregate<T>)
          && (!concepts::c_array<T>)
constexpr bool has_padding() {
    constexpr auto compiler_size = sizeof(T);
    constexpr auto packed_size = packed_sizeof<T>();
    constexpr auto has_padded_members = visit_members(
        [](auto&&... members) constexpr {
            return (has_padding<std::remove_cvref_t<decltype(members)>>() || ...);
        },
        T{}
    );       

    return packed_size != compiler_size || has_padded_members;
}

} // namespace teg 

namespace teg::concepts {

template <class T>
concept padded_layout = concepts::trivially_copyable<T> && has_padding<T>();

template <class T>
concept packed_layout = concepts::trivially_copyable<T> && !has_padding<T>();

} // namespace teg::concepts

#endif // TEG_ALIGNMENT_H