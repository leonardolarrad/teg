#ifndef TEG_MEMBERS_GET_H
#define TEG_MEMBERS_GET_H

#include "teg/members_tie.h"

namespace teg {

template <std::size_t I, class T>
teg_inline constexpr auto get_member(T&& t) -> decltype(auto) {
    return std::get<I>(teg::tie_members(t));
}

} // namespace teg

#endif
