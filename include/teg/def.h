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

#include <cstdint>

#define TEG_VERSION 1

#if !defined(teg_nodiscard)
#define teg_nodiscard [[nodiscard]]
#endif

#if !defined(teg_inline)
#if defined(_MSC_VER)
#define teg_inline [[msvc::forceinline]]
#elif defined(__GNUC__) || defined(__clang__)
#define teg_inline __attribute__((always_inline))
#else
#define teg_inline inline
#endif
#endif // teg_inline

#if !defined(teg_inline_lambda)
#if defined(_MSC_VER)
#define teg_inline_lambda [[msvc::forceinline]]
#elif defined(__GNUC__)
#define teg_inline_lambda constexpr __attribute__((always_inline))
#elif defined(__clang__)
#define teg_inline_lambda __attribute__((always_inline)) constexpr
#else
#define teg_inline_lambda constexpr
#endif
#endif // teg_inline_lambda

#if !defined(teg_unlikely)
#define teg_unlikely [[unlikely]]
#endif // teg_unlikely

#if !defined(teg_likely)
#define teg_likely   [[likely]]
#endif // teg_likely

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
