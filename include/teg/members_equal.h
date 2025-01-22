
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

#ifndef TEG_MEMBERS_EQUAL_H
#define TEG_MEMBERS_EQUAL_H

#include "teg/members_visitor.h"
#include "teg/members_tie.h"
#include "teg/members_get.h"
    
namespace teg {

template <class T1, class T2>
requires (std::equality_comparable_with<std::remove_cvref_t<T1>, std::remove_cvref_t<T2>>)
      || (concepts::accesible_aggregate<std::remove_cvref_t<T1>> && concepts::accesible_aggregate<std::remove_cvref_t<T2>>)
TEG_NODISCARD TEG_INLINE constexpr auto memberwise_equal(T1 const& left, T2 const& right) -> bool {
    using left_type = std::remove_cvref_t<T1>;
    using right_type = std::remove_cvref_t<T2>;

    if constexpr (std::equality_comparable_with<left_type, right_type>) {
        if constexpr (concepts::c_array<left_type>) {
            return std::ranges::equal(left, right);
        }
        else {
            return left == right;
        }
    }
    else if constexpr (members_count_v<left_type> != members_count_v<right_type>) {
        return false;
    }
    else {
        auto equal = [&]<std::size_t... I>(std::index_sequence<I...>) constexpr -> bool {
            return (memberwise_equal(get_member<I>(left), get_member<I>(right)) && ...);
        };
        
        return equal(std::make_index_sequence<members_count_v<left_type>>{});
    }
}

} // namespace teg

#endif // TEG_MEMBERS_EQUAL_H
