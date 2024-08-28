// Copyright (c) 2024 Adrian larrad & Leonardo larrad.
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once
#include <type_traits>
#include <cstddef>
#include <concepts>
#include <vector>
#include "concepts.h"

namespace teg::internal {

template <typename T, typename... A>
concept constructible_from = requires {
    T{{A{}}...};
};

struct any_type {
    template <typename T>
    operator T();
};

template<aggregate T, typename ... A>
inline constexpr std::size_t recursive_members_count() {
    if constexpr (constructible_from<T, any_type, A...>) {
        return recursive_members_count<T, A..., any_type>();
    }
    else {
        return sizeof...(A);
    }        
}

template<aggregate T>
inline constexpr std::size_t members_count() {
    using type = std::remove_cvref_t<T>;

    if constexpr (c_array_like<type>) {
        return std::extent_v<type>;
    }
    else if constexpr (!class_like<type>) {
        return 0;
    }
    else {
        return recursive_members_count<T>();
    }
}

} // namespace teg::internal

namespace teg {

template <aggregate T>
inline constexpr std::size_t members_count = internal::members_count<T>();

} // namespace teg