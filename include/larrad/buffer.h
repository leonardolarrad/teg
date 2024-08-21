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

namespace larrad {

using buffer = std::vector<std::byte>;

namespace internal {

inline constexpr 
std::size_t compute_size_one(const auto& obj) {
    return sizeof(obj);
}

inline constexpr
std::size_t compute_size_many(const auto&... objs) {
    return (compute_size_one(objs) + ...);
}

} // namespace internal

inline constexpr 
std::size_t compute_serialized_size(const auto&... obj) {
    return internal::compute_size_many(obj...);
}

} // namespace larrad