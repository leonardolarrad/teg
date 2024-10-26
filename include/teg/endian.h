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

#ifndef TEG_ENDIAN_H
#define TEG_ENDIAN_H

#include <bit>
#include "options.h"
#include "container_concepts.h"

namespace teg {

///  \brief Determine whether or not to perform endian swapping.
///  \tparam Opt The serialization options.
///  
template <options Opt>
constexpr auto requires_endian_swap() -> bool {
    if constexpr (Opt == options::native_endian) {
        return false;
    }
    else {
        constexpr auto system_endian = std::endian::native;
        constexpr auto config_endian = (Opt & options::big_endian) ? std::endian::big : std::endian::little;

        return system_endian != config_endian;
    }
}

} // namespace teg

namespace teg::concepts {

///  \brief A type that is endian-neutral.
///
///  A type is considered endian-neutral if its size is one byte. This is because a 
///  single byte is always interpreted the same way on any platform, regardless of the
///  endianness of the system. This concept is used to determine whether or not to 
///  perform endian swapping when serializing or deserializing data.
///
template <class T>
concept endian_neutral = sizeof(T) == 1;

///  \brief An endian-neutral container.
///  A container is considered endian-neutral if its elements are endian-neutral.
///  
template <class T>
concept endian_neutral_container = container<T> && endian_neutral<typename T::value_type>;

///  \brief An endian-neutral C array.
///  A c-array is considered endian-neutral if its elements are endian-neutral.
///  
template <class T>
concept endian_neutral_c_array = bounded_c_array<T> && endian_neutral<std::remove_all_extents_t<T>>;

///  \brief Determine whether or not a tpye `T` requires endian swapping.
///  
template <class T, options Opt>
concept endian_swapping_required = requires_endian_swap<Opt>() 
    && !(endian_neutral<T> || endian_neutral_container<T> || endian_neutral_c_array<T>);

} // namespace teg::concepts

#endif // TEG_ENDIAN_H