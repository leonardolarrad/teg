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

#ifndef TEG_C_ARRAY_H
#define TEG_C_ARRAY_H

#include <concepts>
#include <type_traits>
#include <tuple>

#include "def.h"
#include "core_concepts.h"
#include "container_concepts.h"

namespace teg::concepts {
    
///  \brief An c-array type.
///  \see https://en.cppreference.com/w/cpp/language/array
///  
template <class T>
concept c_array = std::is_array_v<T>;

///  \brief A bounded c-array type.
///
///  A c-array is considered bounded when its size is kwown at compile-time.
///  
///  \see https://en.cppreference.com/w/cpp/types/is_bounded_array
///  
template <class T>
concept bounded_c_array = std::is_bounded_array_v<T>;

///  \brief An unbounded c-array type.
///
///  A c-array is considered unbounded when its size is unknown at compile-time.
///  Most of the time this occurs when a c-array decays to a pointer.
///
///  \see https://en.cppreference.com/w/cpp/types/is_unbounded_array
///  \see https://en.cppreference.com/w/cpp/language/array#Arrays_of_unknown_bound
///  
template <class T>
concept unbounded_c_array = std::is_unbounded_array_v<T>;

///  \brief An aggregate type.
///
///  An aggregate is an array or a class with no user-declared or inherited constructors,
///  no private or protected direct non-static data members, no virtual functions, and
///  no virtual, private, or protected base classes. ISO/IEC 14882:2020 [dcl.init.aggr].
///
///  \see https://en.cppreference.com/w/cpp/language/aggregate_initialization
///  
template <class T>
concept aggregate = std::is_aggregate_v<T> && !unbounded_c_array<T>;

}

namespace teg::internal {

TEG_INLINE constexpr auto copy_md_c_array_impl(auto& dst, auto const& src, std::size_t& i) -> void {
    using dst_type = std::remove_cvref_t<decltype(dst)>;
    constexpr auto rank = std::rank_v<dst_type>;
    constexpr auto extent = std::extent_v<dst_type>;

    if constexpr (rank == 1) {
        for (std::size_t j = 0; j < extent; ++j) {
            dst[j] = src[i++];
        }
    }
    else {
        for (std::size_t j = 0; j < extent; ++j) {
            copy_md_c_array_impl(dst[j], src, i);
        }
    }  
}

} // namespace teg::internal

namespace teg {

///  \brief Copies a multidimensional c-array.
///
///  Copies a c-array from a source c-array or fixed-size container to a destination c-array.
///  It can be used at compile-time in a constant evalutated context.
///  
///  \tparam D Destination type.
///  \tparam S Source type.
///  \param dst The destination c-array where the data will be copied.
///  \param src The source c-array or fixed-size container from which the data will be copied.
///  
///  \details The algorithm is implemented using recursion to copy each element from every 
///  rank one by one.
///  
template <class D, class S> 
requires
       (concepts::bounded_c_array<D>)
    && (concepts::bounded_c_array<S> || concepts::fixed_size_container<S>)
    && ((!concepts::bounded_c_array<S>) || std::rank_v<S> == 1)
    && (sizeof(D) == sizeof(S))
    && (std::same_as<std::remove_all_extents_t<D>, typename S::value_type>)
TEG_INLINE constexpr auto copy_md_c_array(D & dst, S const& src) -> void {
    std::size_t i = 0;
    internal::copy_md_c_array_impl(dst, src, i);
}

} // namespace teg

#endif // TEG_C_ARRAY_H