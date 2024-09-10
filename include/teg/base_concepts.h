// Copyright (c) 2024 Adrian & Leonardo Larrad.
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

#include <concepts>
#include <type_traits>

namespace teg {

template <typename T>
concept fundamental = std::is_fundamental_v<std::remove_cvref_t<T>>;

template <typename T>
concept is_enum = std::is_enum_v<std::remove_cvref_t<T>>;

template <typename T>
concept standard_layout = std::is_standard_layout_v<std::remove_cvref_t<T>>;

template <typename T>
concept aggregate = std::is_aggregate_v<std::remove_cvref_t<T>>;

template <typename T>
concept strict_aggregate = aggregate<T>;

template <typename T>
concept trivial = std::is_trivial_v<std::remove_cvref_t<T>>;

template <typename T>
concept trivially_copyable = std::is_trivially_copyable_v<std::remove_cvref_t<T>>;

} // namespace teg