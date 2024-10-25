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

#ifndef TEG_BUFFER_H
#define TEG_BUFFER_H

#include <vector>
#include <array>
#include <type_traits>
#include <tuple>

#include "core_concepts.h"
#include "container_concepts.h"

namespace teg::concepts {

///  \brief A byte type.
///  
///  A byte type can be represented either by a `char`, an `unsigned char` or `std::byte`.
///  However, it is recommended to use std::byte when possible.
///  
///  \see https://en.cppreference.com/w/cpp/types/byte
///  
template <class T>
concept byte = std::same_as<std::remove_cv_t<T>, char> 
            || std::same_as<std::remove_cv_t<T>, unsigned char> 
            || std::same_as<std::remove_cv_t<T>, std::byte>;

///  \brief A byte buffer type. 
///
///  A byte buffer can be either a contiguous container or a contiguous view of bytes.
///  In the first case, the buffer can be resized to fit the format of the serialized
///  data. In the second case, the buffer is fixed in size, requiring the exact size
///  of the buffer to be specified.
///  
///  \see https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
///  \see https://en.cppreference.com/w/cpp/container/span
///  
template <class T>
concept byte_buffer = (contiguous_container<T> || span<T>) && byte<typename T::value_type>;    

} // namespace teg::concepts

namespace teg {

///  \brief A contiguous and resizable container of bytes.
///  \note This is the default buffer type for run-time de/serialization.
///  
using byte_buffer = std::vector<std::byte>;

///  \brief A fixed contiguous container of bytes.
///  \tparam N The size of the buffer.
///  \note This is the default buffer type for compile-time de/serialization.
///  
template <std::size_t N>
using fixed_byte_buffer = std::array<std::byte, N>;

} // namespace teg

#endif // TEG_BUFFER_H