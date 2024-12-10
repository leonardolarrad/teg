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

#ifndef TEG_DEF_H
#define TEG_DEF_H

#include <array>
#include <algorithm>
#include <bit>
#include <cassert>
#include <compare>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include <iostream>

#define TEG_VERSION_MAJOR 1
#define TEG_VERSION_MINOR 0
#define TEG_VERSION_PATCH 0
#define TEG_VERSION { TEG_VERSION_MAJOR, TEG_VERSION_MINOR, TEG_VERSION_PATCH }

#if !defined(TEG_NODISCARD)
#define TEG_NODISCARD [[nodiscard]]
#endif

#if !defined(TEG_INLINE)
#if defined(_MSC_VER)
#define TEG_INLINE [[msvc::forceinline]]
#elif defined(__GNUC__) || defined(__clang__)
#define TEG_INLINE __attribute__((always_inline))
#else
#define TEG_INLINE inline
#endif
#endif // TEG_INLINE

#if !defined(TEG_INLINE_LAMBDA)
#if defined(_MSC_VER)
#define TEG_INLINE_LAMBDA [[msvc::forceinline]]
#elif defined(__GNUC__)
#define TEG_INLINE_LAMBDA constexpr __attribute__((always_inline))
#elif defined(__clang__)
//NOLINTBEGIN
#define TEG_INLINE_LAMBDA __attribute__((always_inline)) constexpr
//NOLINTEND
#else
#define TEG_INLINE_LAMBDA constexpr
#endif
#endif // TEG_INLINE_LAMBDA

#if !defined(TEG_UNLIKELY)
#define TEG_UNLIKELY [[unlikely]]
#endif // TEG_UNLIKELY

#if !defined(TEG_LIKELY)
#define TEG_LIKELY   [[likely]]
#endif // TEG_LIKELY

namespace teg {

using u8  = std::uint8_t;            //  8-bit unsigned integer.
using u16 = std::uint16_t;           // 16-bit unsigned integer.
using u32 = std::uint32_t;           // 32-bit unsigned integer.
using u64 = std::uint64_t;           // 64-bit unsigned integer.

using i8  = std::int8_t;             //  8-bit signed integer.
using i16 = std::int16_t;            // 16-bit signed integer.
using i32 = std::int32_t;            // 32-bit signed integer.
using i64 = std::int64_t;            // 64-bit signed integer.

static_assert(sizeof(float) == 4);
static_assert(sizeof(double) == 8);
using f32 = float;                   // 32-bit floating point.
using f64 = double;                  // 64-bit floating point.

using isize = std::intptr_t;         // pointer-sized integer.
using usize = std::uintptr_t;        // pointer-sized unsigned integer.

}

#endif // TEG_DEF_H
