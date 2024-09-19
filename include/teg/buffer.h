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
#include <vector>

#include "concepts.h"
#include "visitor.h"

namespace teg::internal {

inline constexpr 
std::size_t buffer_size_one(fixed_size_container auto const& obj);

inline constexpr 
std::size_t buffer_size_one(container auto const& obj);

inline constexpr 
std::size_t buffer_size_one(auto const& obj);

inline constexpr
std::size_t buffer_size_many() {
    return 0;
}

inline constexpr
std::size_t buffer_size_many(auto const& first_obj, auto const&... remaining_objs) {
    return buffer_size_one(first_obj) + buffer_size_many(remaining_objs...);
}


inline constexpr 
std::size_t buffer_size_one(fixed_size_container auto const& obj) {    
    std::size_t size = 0;
    for (auto const& elem : obj) {
        size += buffer_size_one(elem);
    }
    return size;
}

inline constexpr 
std::size_t buffer_size_one(container auto const& obj) {
    using type = std::remove_cvref_t<decltype(obj)>;
    using size_type = typename type::size_type;
    
    auto size = sizeof(size_type);
    for (auto const& elem : obj) {
        size += buffer_size_one(elem);
    }
    return size;
}

inline constexpr 
std::size_t buffer_size_one(auto const& obj) {
    using type = std::remove_cvref_t<decltype(obj)>;
    
    if constexpr (fundamental<type> || is_enum<type>) {
        return sizeof(type);
    }
    else {
        return visit_members(
            obj, 
            [](auto&&... objs) constexpr {
                return buffer_size_many(objs...);
            }
        );
    }
}



} // namespace teg::internal

namespace teg {

using buffer = std::vector<std::byte>;

[[nodiscard]] inline constexpr 
std::size_t buffer_size(const auto&... obj) {
    return internal::buffer_size_many(obj...);
}

} // namespace teg