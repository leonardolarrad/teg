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

#include <cstddef>
#include <concepts>
#include <vector>

namespace larrad::internal {

struct struct_type {
    template <typename T>
    operator T();
};

struct vector_type {
    template <typename T>
    operator std::vector<T>();
};

template<typename T, typename ... A>
inline constexpr std::size_t get_number_of_members_impl() {
    if constexpr (std::constructible_from<T, vector_type, A...>) {
        return get_number_of_members_impl<T, A..., vector_type>();
    }
    else if constexpr (std::constructible_from<T, struct_type, A...>) {
        return get_number_of_members_impl<T, A..., struct_type>();
    }
    else {
        return sizeof...(A);
    }        
}

template<typename T>
constexpr std::size_t get_number_of_members() {
    return get_number_of_members_impl<T>();
}

} // namespace larrad::internal

namespace larrad {

template <typename T>
constexpr std::size_t number_of_members = internal::get_number_of_members<T>();

} // namespace larrad