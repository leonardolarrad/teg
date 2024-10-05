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

///  This is file is a rewrite of the md5 implementation used in Chromium.
///  https://chromium.googlesource.com/chromium/src/base/+/refs/heads/main/hash/md5_constexpr_internal.h

#ifndef TEG_MD5_H
#define TEG_MD5_H

#include <array>
#include <cassert>
#include <cstdint>
#include <string_view>

namespace teg::internal {

// The data representation at each round is a 4-tuple of uint32_t.
struct intermediate_data {
    std::uint32_t a, b, c, d;
};

// The input data for a single round consists of 16 uint32_t (64 bytes).
using round_data = std::array<std::uint32_t, 16>;

static constexpr std::array<std::uint32_t, 64> constants = {
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

static constexpr std::array<uint32_t, 16> shifts = {
    7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21
};

// The initial mid data.
static constexpr intermediate_data initial_intermediate_data = {
    0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476
};

// Given the message length, calculates the padded message length. There has
// to be room for the 1-byte end-of-message marker, plus 8 bytes for the
// uint64_t encoded message length, all rounded up to a multiple of 64 bytes.
static constexpr auto get_padded_message_length(std::uint32_t len) -> std::uint32_t {
    return (((len + 1 + 8) + 63) / 64) * 64;
}

// Extracts the |i|th byte of a uint64_t, where |i == 0| extracts the least
// significant byte. It is expected that 0 <= i < 8.
static constexpr auto extract_byte(std::uint64_t value, std::uint32_t i) -> std::uint8_t {
    assert(i < 8u);
    return static_cast<std::uint8_t>((value >> (i * 8)) & 0xff);
}

// Extracts the |i|th byte of a message of length |n|.
static constexpr auto get_padded_message_byte
    (std::string_view data, std::uint32_t m, std::uint32_t i) -> std::uint8_t {

    assert(i < m);
    assert(data.size() < m);
    assert(m % 64 == 0) ;

    if (i < data.size()) {
        // Emit the message itself...
        return static_cast<uint8_t>(data[i]);
    } else if (i == data.size()) {
        // ...followed by the end of message marker.
        return 0x80;
    } else if (i >= m - 8) {
        // The last 8 bytes encode the original message length times 8.
        return extract_byte(data.size() * 8, i - (m - 8));
    } else {
        // And everything else is just empyt padding.
        return 0;
    }
}

// Extracts the uint32_t starting at position |i| from the padded message
// generate by the provided input |data|. The bytes are treated in little
// endian order.
static constexpr auto get_padded_message_word
    (std::string_view data, std::uint32_t m, std::uint32_t i) -> std::uint32_t {

    assert(i % 4 == 0);
    assert(i < m);
    assert(data.size() < m);
    assert(m % 64 == 0) ;

    return static_cast<std::uint32_t>((get_padded_message_byte(data, m, i + 0)) <<  0) 
         | static_cast<std::uint32_t>((get_padded_message_byte(data, m, i + 1)) <<  8) 
         | static_cast<std::uint32_t>((get_padded_message_byte(data, m, i + 2)) << 16) 
         | static_cast<std::uint32_t>((get_padded_message_byte(data, m, i + 3)) << 24);
}

// Given an input buffer |data|, extracts one round worth of data starting at
// offset |i|.
static constexpr auto get_round_data
    (std::string_view data, std::uint32_t m, std::uint32_t i) -> round_data {

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

// Mixes elements |b|, |c| and |d| at round |i| of the calculation.
static constexpr auto compute_f
    (std::uint32_t i, std::uint32_t b, std::uint32_t c, std::uint32_t d) -> std::uint32_t {

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

static constexpr std::uint32_t compute_f(std::uint32_t i, intermediate_data mid) {
    return compute_f(i, mid.b, mid.c, mid.d);
}

// Calculates the indexing function at round |i|.
static constexpr std::uint32_t compute_g(std::uint32_t i) {
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

// Calculates the rotation to be applied at round |i|.
static constexpr auto get_shift(std::uint32_t i) -> std::uint32_t {
    assert(i < 64);
    return shifts[(i / 16) * 4 + (i % 4)];
}

// Rotates to the left the given |value| by the given |bits|.
static constexpr auto left_rotate(std::uint32_t value, std::uint32_t bits) -> std::uint32_t {
    assert(bits < 32);
    return (value << bits) | (value >> (32 - bits));
}

// Applies the |i|th step of mixing.
static constexpr auto apply_step
    (std::uint32_t i, round_data const& data, intermediate_data mid) -> intermediate_data {
    
    assert(i < 64);
    
    const std::uint32_t g = compute_g(i);
    assert(g < 16);
    
    const std::uint32_t f = compute_f(i, mid) + mid.a + constants[i] + data[g];
    const uint32_t s = get_shift(i);
    return intermediate_data {
        .a = mid.d,
        .b = mid.b + left_rotate(f, s),
        .c = mid.b,
        .d = mid.c
    };
}

// Adds two mid data together.
static constexpr auto add(intermediate_data mid1, intermediate_data mid2) -> intermediate_data {
    return intermediate_data {
        mid1.a + mid2.a, mid1.b + mid2.b,
        mid1.c + mid2.c, mid1.d + mid2.d
    };
}

// Processes an entire message.
static constexpr auto process_message(std::string_view message) -> intermediate_data {
    const std::uint32_t m = get_padded_message_length(static_cast<std::uint32_t>(message.size()));
    
    intermediate_data mid0 = initial_intermediate_data;
    
    for (uint32_t offset = 0; offset < m; offset += 64) {
        round_data data = get_round_data(message, m, offset);
        intermediate_data mid1 = mid0;

        for (uint32_t i = 0; i < 64; ++i) {
            mid1 = apply_step(i, data, mid1);
        }
        mid0 = add(mid0, mid1);
    }
    return mid0;
}

static constexpr auto swap_endian(std::uint32_t a) -> std::uint32_t {
    return
        ((a & 0xff) << 24) | (((a >> 8) & 0xff) << 16) |
        (((a >> 16) & 0xff) << 8) | ((a >> 24) & 0xff);
}

static constexpr auto md5_hash_64_impl(std::string_view data) -> std::uint64_t {
    intermediate_data processed_data = process_message(data);

    return (static_cast<std::uint64_t>(swap_endian(processed_data.a)) << 32) 
         | (static_cast<std::uint64_t>(swap_endian(processed_data.b)) <<  0);
}

static constexpr auto md5_hash_32_impl(std::string_view data) -> std::uint32_t {
    intermediate_data processed_data = process_message(data);
    return swap_endian(processed_data.a);
}

} // namespace teg::internal

namespace teg {

constexpr auto md5_hash_u32(std::string_view data) -> std::uint32_t {
    return internal::md5_hash_32_impl(data);
}

constexpr auto md5_hash_u64(std::string_view data) -> std::uint64_t {
    return internal::md5_hash_64_impl(data);
}

} // namespace teg

#endif // TEG_MD5_H