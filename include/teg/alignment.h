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

#include "teg/def.h"
#include "teg/core_concepts.h"
#include "teg/members_equal.h"

namespace teg::internal {

template <class T>
    requires (concepts::standard_layout<T>) 
          && (!concepts::accesible_aggregate<T>)
consteval auto packed_sizeof_impl() -> std::size_t {
    return sizeof(T);
}

template <class T>
    requires (concepts::standard_layout<T>) 
          && (concepts::accesible_aggregate<T>)
consteval auto packed_sizeof_impl() -> std::size_t {
    return teg::visit_members(
        [](auto&&... members) constexpr {
            return (packed_sizeof_impl<std::remove_cvref_t<decltype(members)>>() + ...);
        },
        T{}
    );
}

template <class T>
    requires (concepts::standard_layout<T>)
constexpr bool has_padding_bits_impl() {
    return !std::has_unique_object_representations_v<T>;
}

template <class T>
    requires (concepts::standard_layout<T>)
          && (concepts::fundamental<T> || concepts::is_enum<T>)
constexpr bool has_padding_bits_impl() {
    return false;
}

template <class T>
    requires (concepts::standard_layout<T>) 
          && (concepts::c_array<T>)
constexpr bool has_padding_bits_impl() {
    return has_padding_bits_impl<std::remove_cvref_t<std::remove_all_extents_t<T>>>();
}

template <class T>
    requires (concepts::standard_layout<T>)
          && (concepts::accesible_aggregate<T>) 
          && (!concepts::c_array<T>)
constexpr bool has_padding_bits_impl() {
    constexpr auto compiler_size = sizeof(T);
    constexpr auto packed_size = packed_sizeof_impl<T>();
    constexpr auto has_padded_members = teg::visit_members(
        [](auto&&... members) constexpr {
            return (has_padding_bits_impl<std::remove_cvref_t<decltype(members)>>() || ...);
        },
        T{}
    );       

    return packed_size != compiler_size || has_padded_members;
}

} // namespace teg::internal

namespace teg {

///  \brief Returns the size of a type `T` in bytes as if the type was packed.
///  \tparam T A standard layout type.
///  
///  If `T` is a standard layout type, counts the size in bytes of `T` 
///  excluding the padding bits.
///  
///  \example
///  \code
///      struct padded {
///          char a;     // 1 byte
///                      // 3 bytes padding
///          int32_t b;  // 4 bytes
///      };
///      static_assert(sizeof(padded) == 8);
///      static_assert(teg::packed_sizeof<padded>() == 5);
///  \endcode
///  
template <class T> requires (concepts::standard_layout<T>)
constexpr auto packed_sizeof() -> std::size_t {
    return internal::packed_sizeof_impl<T>();
}

///  \brief Returns `true` if the type `T` has padding bits, otherwise `false`.
///  \tparam T A standard layout type.
///  
///  If `T` is a standard layout type, checks if `T` has padding bits.
///
///  \example
///  \code
///      struct padded {
///          char a;     // 1 byte
///                      // 3 bytes padding
///          int32_t b;  // 4 bytes
///      };
///      #pragma pack(push, 1)
///      struct packed {
///          char a;     // 1 byte
///          int32_t b;  // 4 bytes
///      };
///      #pragma pack(pop)
///      static_assert(teg::has_padding_bits<padded>());
///      static_assert(!teg::has_padding_bits<packed>());
///  \endcode
template <class T> requires (concepts::standard_layout<T>)
constexpr auto has_padding_bits() -> bool {
    return internal::has_padding_bits_impl<T>();
}

template <class T>
consteval auto has_packed_layout() -> bool {
    return false;
}

template <class T>
requires concepts::fundamental<T> || concepts::is_enum<T>
consteval auto has_packed_layout() -> bool {
    return true;
}

template <class T>
requires concepts::bit_castable<T> && concepts::accesible_aggregate<T>
consteval auto has_packed_layout() -> bool {
    
    auto bytes = std::array<u8, sizeof(T)>{};
    const T reference = std::bit_cast<T>(std::array<u8, sizeof(T)>{});

    for (std::size_t i = 0; i < sizeof(T); ++i) {
        bytes[i] = 1u;  // Perturb the object representation.
        const T instance = std::bit_cast<T>(bytes);

        if (memberwise_equal(instance, reference)) {
            return false;
        }

        bytes[i] = 0u;  // Restore the object representation.
    }
    return true;    

}

template <class T>
requires concepts::bounded_c_array<T>
consteval auto has_packed_layout() -> bool {
    return has_packed_layout<std::remove_cvref_t<std::remove_all_extents_t<T>>>();
}


} // namespace teg

namespace teg::concepts {

///  \brief A type that has standard layout and has no padding bits.
///  \tparam T A standard layout type.
///  \see https://en.cppreference.com/w/c/language/object
///
template <class T>
concept packed_standard_layout = standard_layout<T> && !has_padding_bits<T>();

///  \brief A type that has no padding bits.
///  \tparam T A standard layout type.
///  \see https://en.cppreference.com/w/c/language/object
///
template <class T>
concept packed_layout = has_packed_layout<T>();

} // namespace teg::concepts

#endif // TEG_ALIGNMENT_H