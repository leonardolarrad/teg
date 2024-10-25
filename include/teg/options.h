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

#ifndef TEG_OPTIONS_H
#define TEG_OPTIONS_H

#include <cstddef>
#include <cstdint>
#include <limits>
#include <bit>

namespace teg {

///  \brief Serialization options.
///  
enum class options : uint64_t {

    // An asterisk (*) indicates the default option.

    enable_format_check     = 1 << 0,   // (*) Enable format check: compile-time type code.
    enable_integrity_check  = 1 << 1,   //     Enable integrity check: data checksum.
    enable_all_checks       =           //     Enable all checks.
        enable_format_check | enable_integrity_check,

    native_endian           = 1 << 3,   //     Use native endianness for encoding/decoding data.
    little_endian           = 1 << 4,   // (*) Use little-endian for encoding/decoding data.
    big_endian              = 1 << 5,   //     Use big-endian for encoding/decoding data.

    allocation_limit_1gib   = 1 << 6,   //     Limit byte buffer allocation to 1 GiB.
    allocation_limit_2gib   = 1 << 7,   // (*) Limit byte buffer allocation to 2 GiB.
    allocation_limit_4gib   = 1 << 8,   //     Limit byte buffer allocation to 4 GiB.

    container_size_native   = 1 << 12,  //     Use native container size type (std::size_t).
    container_size_1b       = 1 << 13,  //     Use 1-byte container size type (uint8_t).
    container_size_2b       = 1 << 14,  //     Use 2-byte container size type (uint16_t).
    container_size_4b       = 1 << 15,  // (*) Use 4-byte container size type (uint32_t). 
    container_size_8b       = 1 << 16,  //     Use 8-byte container size type (uint64_t).

    variant_index_native    = 1 << 17,  //     Use native variant index type (std::size_t).
    variant_index_1b        = 1 << 18,  // (*) Use 1-byte variant index type (uint8_t).
    variant_index_2b        = 1 << 19,  //     Use 2-byte variant index type (uint16_t).
    variant_index_4b        = 1 << 20,  //     Use 4-byte variant index type (uint32_t).
    variant_index_8b        = 1 << 21,  //     Use 8-byte variant index type (uint64_t).

};

constexpr auto operator|(options a, options b) -> options { 
    return options(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}

constexpr auto operator&(options a, options b) -> bool {
    return static_cast<uint64_t>(a) & static_cast<uint64_t>(b);
}

///  \brief Default de/serialization mode.
///  
///  The default mode allows to allocate 2 GiB of memory, sets the default container size to 4 bytes 
///  and the default variant index type to 1 byte. That means that a container can hold up to 
///  2^32 (4'294'967'296) elements, and a variant can have up to 2^8 (255) alternatives.
///  
static constexpr options default_mode = 
    options::little_endian | options::allocation_limit_2gib | 
    options::container_size_4b | options::variant_index_1b;

static constexpr options compact_mode = 
    options::little_endian | options::allocation_limit_1gib | 
    options::container_size_1b | options::variant_index_1b;

///  \warning Fast mode is not portable.
static constexpr options fast_mode = 
    options::native_endian | options::allocation_limit_4gib | 
    options::container_size_native | options::variant_index_native;

static constexpr options secure_mode = default_mode | options::enable_all_checks;

///  \brief Container size type.
///  \tparam Opt Options flag.
///  
///  Returns the container size type based on the options flag.
///  
template <options Opt>
using get_container_size_type = 
    std::conditional_t<Opt & options::container_size_8b, std::uint64_t,
    std::conditional_t<Opt & options::container_size_4b, std::uint32_t,
    std::conditional_t<Opt & options::container_size_2b, std::uint16_t,
    std::conditional_t<Opt & options::container_size_1b, std::uint8_t,
    std::conditional_t<Opt & options::container_size_native, std::size_t,
    std::uint32_t // Default case.
    >>>>>;

///  \brief Variant index type.
///  \tparam Opt Options flag.
///  
///  Returns the variant index type based on the options flag.
///  
template <options Opt>
using get_variant_index_type = 
    std::conditional_t<Opt & options::variant_index_8b, std::uint64_t,
    std::conditional_t<Opt & options::variant_index_4b, std::uint32_t,
    std::conditional_t<Opt & options::variant_index_2b, std::uint16_t,
    std::conditional_t<Opt & options::variant_index_1b, std::uint8_t,
    std::conditional_t<Opt & options::variant_index_native, std::size_t,
    std::uint8_t // Default case.
    >>>>>;

///  \brief Allocation limit.
///  \tparam Opt Options flag.
///  
///  Returns the allocation limit based on the options flag.
///  
template <options Opt>
constexpr auto get_allocation_limit() -> uint64_t {
    if constexpr (Opt & options::allocation_limit_4gib) {
        return 1024ull * 1024ull * 1024ull * 4ull;
    } else if constexpr (Opt & options::allocation_limit_2gib) {
        return 1024ull * 1024ull * 1024ull * 2ull;
    } else if constexpr (Opt & options::allocation_limit_1gib) {
        return 1024ull * 1024ull * 1024ull * 1ull;;
    } else {
        return 1024ull * 1024ull * 1024ull * 2ull; // Default case.
    }
}

} // namespace teg

#endif // TEG_OPTIONS_H