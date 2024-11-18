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

///  Copyright 2019 The Chromium Authors
///  Use of this source code is governed by a BSD-style license that can be
///  found in the LICENSE file.
///
///  Copyright (c) 2012 The Chromium Authors. All rights reserved.
/// 
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are
///  met:
/// 
///     * Redistributions of source code must retain the above copyright
///  notice, this list of conditions and the following disclaimer.
///     * Redistributions in binary form must reproduce the above
///  copyright notice, this list of conditions and the following disclaimer
///  in the documentation and/or other materials provided with the
///  distribution.
///     * Neither the name of Google Inc. nor the names of its
///  contributors may be used to endorse or promote products derived from
///  this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
///  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
///  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
///  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
///  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
///  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
///  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
///  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
///  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
///  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

///  This file is a rewritten version of the MD5 implementation originally used in Chromium.
///  https://chromium.googlesource.com/chromium/src/base/+/refs/heads/main/hash/md5_constexpr_internal.h

#ifndef TEG_MD5_H
#define TEG_MD5_H

#include <array>
#include <cassert>
#include <cstdint>
#include <string_view>
#include <tuple>

#include "def.h"
#include "buffer.h"
#include "fixed_string.h"

namespace teg {

struct md5_digest {
    u32 a, b, c, d;
    constexpr bool operator==(md5_digest const&) const = default;
};

namespace internal {

using round_data = std::array<u32, 16>;

static constexpr std::array<u32, 64> constants = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
    0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
    0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
    0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static constexpr std::array<u32, 16> shifts = {
    7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21
};

static constexpr md5_digest initial_digest_data = {
    0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
};

static constexpr auto get_padded_message_length(u32 len) -> u32 {
    return (((len + 1 + 8) + 63) / 64) * 64;
}

static constexpr auto extract_byte(u64 value, u32 i) -> u8 {
    assert(i < 8u);
    return static_cast<u8>((value >> (i * 8)) & 0xff);
}

template <concepts::byte T>
static constexpr auto get_padded_message_byte
    (std::span<T> data, u32 m, u32 i) -> u8 {

    assert(i < m);
    assert(data.size() < m);
    assert(m % 64 == 0) ;

    if (i < data.size()) {
        return static_cast<u8>(data[i]);
    } else if (i == data.size()) {
        return 0x80;
    } else if (i >= m - 8) {
        return extract_byte(data.size() * 8, i - (m - 8));
    } else {
        return 0;
    }
}

template <concepts::byte T>
static constexpr auto get_padded_message_word
    (std::span<T> data, u32 m, u32 i) -> u32 {

    assert(i % 4 == 0);
    assert(i < m);
    assert(data.size() < m);
    assert(m % 64 == 0) ;

    return static_cast<u32>((get_padded_message_byte(data, m, i + 0)) <<  0) 
         | static_cast<u32>((get_padded_message_byte(data, m, i + 1)) <<  8) 
         | static_cast<u32>((get_padded_message_byte(data, m, i + 2)) << 16) 
         | static_cast<u32>((get_padded_message_byte(data, m, i + 3)) << 24);
}

template <concepts::byte T>
static constexpr auto get_round_data(std::span<T> data, u32 m, u32 i) -> round_data {

    assert(i % 64 == 0);
    assert(i < m);
    assert(data.size() < m);
    assert(m % 64 == 0);

    return round_data {
        get_padded_message_word(data, m, i +  0),
        get_padded_message_word(data, m, i +  4),
        get_padded_message_word(data, m, i +  8),
        get_padded_message_word(data, m, i + 12),
        get_padded_message_word(data, m, i + 16),
        get_padded_message_word(data, m, i + 20),
        get_padded_message_word(data, m, i + 24),
        get_padded_message_word(data, m, i + 28),
        get_padded_message_word(data, m, i + 32),
        get_padded_message_word(data, m, i + 36),
        get_padded_message_word(data, m, i + 40),
        get_padded_message_word(data, m, i + 44),
        get_padded_message_word(data, m, i + 48),
        get_padded_message_word(data, m, i + 52),
        get_padded_message_word(data, m, i + 56),
        get_padded_message_word(data, m, i + 60)
    };
}

static constexpr auto compute_f
    (u32 i, u32 b, u32 c, u32 d) -> u32 {

    assert(i < 64);

    if (i < 16) {
        return d ^ (b & (c ^ d));
    } else if (i < 32) {
        return c ^ (d & (b ^ c));
    } else if (i < 48) {
        return b ^ c ^ d;
    } else {
        return c ^ (b | (~d));
    }
}

static constexpr u32 compute_f(u32 i, md5_digest mid) {
    return compute_f(i, mid.b, mid.c, mid.d);
}

static constexpr u32 compute_g(u32 i) {
    assert(i < 64);

    if (i < 16) {
        return i;
    } else if (i < 32) {
        return (5 * i + 1) % 16;
    } else if (i < 48) {
        return (3 * i + 5) % 16;
    } else {
        return (7 * i) % 16;
    }
}

static constexpr auto get_shift(u32 i) -> u32 {
    assert(i < 64);
    return shifts[(i / 16) * 4 + (i % 4)];
}

static constexpr auto left_rotate(u32 value, u32 bits) -> u32 {
    assert(bits < 32);
    return (value << bits) | (value >> (32 - bits));
}

static constexpr auto apply_step(u32 i, round_data const& data, md5_digest mid) -> md5_digest {
    assert(i < 64);
    
    const u32 g = compute_g(i);
    assert(g < 16);
    
    const u32 f = compute_f(i, mid) + mid.a + constants[i] + data[g];
    const u32 s = get_shift(i);
    return md5_digest {
        .a = mid.d,
        .b = mid.b + left_rotate(f, s),
        .c = mid.b,
        .d = mid.c
    };
}

static constexpr auto add(md5_digest d0, md5_digest d1) -> md5_digest {
    return md5_digest {
        d0.a + d1.a, d0.b + d1.b,
        d0.c + d1.c, d0.d + d1.d
    };
}

template <concepts::byte T>
static constexpr auto process_message(std::span<T> message) -> md5_digest {
    const u32 m = get_padded_message_length(static_cast<u32>(message.size()));
    
    md5_digest digest0 = initial_digest_data;
    
    for (u32 offset = 0; offset < m; offset += 64) {
        round_data data = get_round_data(message, m, offset);
        md5_digest digest1 = digest0;

        for (u32 i = 0; i < 64; ++i) {
            digest1 = apply_step(i, data, digest1);
        }
        digest0 = add(digest0, digest1);
    }
    return digest0;
}

static constexpr auto swap_endian(u32 a) -> u32 {
    return
        ((a & 0xff) << 24) | (((a >> 8) & 0xff) << 16) |
        (((a >> 16) & 0xff) << 8) | ((a >> 24) & 0xff);
}

template <concepts::byte T>
static constexpr auto md5_hash_u32_impl(std::span<T> data) -> u32 {
    md5_digest md5_digest = process_message(data);
    return swap_endian(md5_digest.a);
}

template <concepts::byte T>
static constexpr auto md5_hash_u64_impl(std::span<T> data) -> u64 {
    md5_digest md5_digest = process_message(data);

    return (static_cast<u64>(swap_endian(md5_digest.a)) << 32) 
         | (static_cast<u64>(swap_endian(md5_digest.b)) <<  0);
}

template <concepts::byte T>
static constexpr auto md5_hash_u128_impl(std::span<T> data) -> md5_digest {
    
    md5_digest digest = process_message(data);

    return { 
        swap_endian(digest.a), 
        swap_endian(digest.b), 
        swap_endian(digest.c), 
        swap_endian(digest.d) 
    };
}

static constexpr auto to_hex(u32 value) -> fixed_string<8> {
    constexpr char hex_digits[] = "0123456789abcdef";

    return {
        hex_digits[(value >> 28) & 0xf],
        hex_digits[(value >> 24) & 0xf],
        hex_digits[(value >> 20) & 0xf],
        hex_digits[(value >> 16) & 0xf],
        hex_digits[(value >> 12) & 0xf],
        hex_digits[(value >>  8) & 0xf],
        hex_digits[(value >>  4) & 0xf],
        hex_digits[(value >>  0) & 0xf]
    };
}

template <concepts::byte T>
static constexpr auto md5_hash(std::span<T> data) -> fixed_string<32> {
    md5_digest md5_digest = process_message(data);

    return to_hex(swap_endian(md5_digest.a)) 
         + to_hex(swap_endian(md5_digest.b))
         + to_hex(swap_endian(md5_digest.c)) 
         + to_hex(swap_endian(md5_digest.d));
}

} // namespace internal

/// \brief Computes first 32 bits of the MD5 hash of the given data.
///
/// \param data A string view of the data to hash.
/// \return The first 32 bits of the MD5 hash.
///
/// \example
/// \code
///    constexpr u32 hash = teg::md5_hash_u32("Hello there!");
///    static_assert(hash == 0xa77b5533ul);    
/// \endcode
///
template <concepts::byte T>
constexpr auto md5_hash_u32(std::span<T> data) -> u32 {
    return internal::md5_hash_u32_impl(data);
}

constexpr auto md5_hash_u32(std::string_view data) -> u32 {
    return md5_hash_u32(std::span(data));
}

///  \brief Computes first 64 bits of the MD5 hash of the given data.
///  
///  \param data A string view of the data to hash.
///  \return The first 64 bits of the MD5 hash.
/// 
///  \example
///  \code
///      constexpr u64 hash = teg::md5_hash_u64("Hello there!");
///      static_assert(hash == 0xa77b55332699835cul);    
///  \endcode
///  
template <concepts::byte T>
constexpr auto md5_hash_u64(std::span<T> data) -> u64 {
    return internal::md5_hash_u64_impl(data);
}

constexpr auto md5_hash_u64(std::string_view data) -> u64 {
    return md5_hash_u64(std::span(data));
}

///  \brief Md5 digest representation as a 128-bit tuple.
///
//using md5_digest = std::tuple<u32, u32, u32, u32>;

///  \brief Computes the MD5 hash of the given data.
///  
///  \param data A string view of the data to hash.
///  \return A tuple containing the 4 32-bit words of the MD5 hash.
/// 
///  \example
///  \code
///      constexpr teg::md5_digest hash = teg::md5_hash_u128("Hello there!");
///      static_assert(hash == std::make_tuple(0xa77b5533, 0x2699835c, 0x035957df, 0x17630d28));    
///  \endcode
template <concepts::byte T>
constexpr auto md5_hash_u128(std::span<T> data) -> md5_digest {
    return internal::md5_hash_u128_impl(data);
}

constexpr auto md5_hash_u128(std::string_view data) -> md5_digest {
    return md5_hash_u128(std::span(data));
}

///  \brief Computes the MD5 hash of the given data.
/// 
///  \param data A string view of the data to hash.
///  \return A fixed string containing the 32-byte hexadecimal representation of the MD5 hash.
///  
///  \example
///  \code
///      constexpr teg::fixed_string<32> hash = teg::md5_hash("Hello there!");
///      static_assert(hash == "a77b55332699835c035957df17630d28");
///  \endcode
///  
template <concepts::byte T>
constexpr auto md5_hash(std::span<T> data) -> fixed_string<32> {
    return internal::md5_hash(data);
}

constexpr auto md5_hash(std::string_view data) -> fixed_string<32> {
    return md5_hash(std::span(data));
}

} // namespace teg

#endif // TEG_MD5_H
