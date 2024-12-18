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

namespace teg {

///  \brief Determines whether the type has a packed layout.
///
///  \details Unable to determine whether the type has a packed layout.
///
template <class T>
consteval auto has_packed_layout() -> bool {
    return false;
}

///  \brief Determines whether the type has a packed layout.
///
///  \details A fundamental type always has a packed layout.
///
template <class T>
requires concepts::fundamental<T> || concepts::is_enum<T>
consteval auto has_packed_layout() -> bool {
    return true;
}

///  \brief Determines whether the type has a packed layout.
///
///  \details A packed layout is a layout that does not have padding bits, to detect whether
///  a type has a packed layout, we perturb the object representation and check if the perturbed
///  object representation is equal to the original object representation.
///
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

///  \brief Determines whether the type has a packed layout.
///
///  \details A c-array has a packed layout if its element type has a packed layout.
///
template <class T>
requires concepts::bounded_c_array<T>
consteval auto has_packed_layout() -> bool {
    return has_packed_layout<std::remove_cvref_t<std::remove_all_extents_t<T>>>();
}


} // namespace teg

namespace teg::concepts {

///  \brief A type that has no padding bits.
///
template <class T>
concept packed_layout = has_packed_layout<T>();

} // namespace teg::concepts

#endif // TEG_ALIGNMENT_H
