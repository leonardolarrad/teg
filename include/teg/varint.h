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

#ifndef TEG_VARINT_H
#define TEG_VARINT_H

#include <limits>
#include <span>
#include <type_traits>
#include <concepts>

#include "def.h"
#include "error.h"
#include "serialization_concepts.h"

namespace teg {

class zigzag {
public:
    ///  \brief Encodes a signed integer to an unsigned integer using the zigzag encoding.
    ///
    template <class T> requires (std::signed_integral<T>)
    teg_inline static constexpr auto encode(T value) -> std::make_unsigned_t<T> {
        return static_cast<std::make_unsigned_t<T>>(
            (static_cast<std::make_unsigned_t<T>>(value) << 1) ^ (value >> (sizeof(T) * 8 - 1))
        );
    }

    ///  \brief Decodes an unsigned integer to a signed integer using the zigzag encoding.
    ///
    template <class T> requires (std::unsigned_integral<T>)
    teg_inline static constexpr auto decode(T value) -> std::make_signed_t<T> {
        return static_cast<std::make_signed_t<T>>((value >> 1) ^ -(value & 1));
    }
};

class uleb128 {
public:
    ///  \brief Calculates the size in bytes needed to encode the given value 
    ///  to ULEB-128 format.
    ///  
    ///  \param value The value to encode.
    ///  \return The size in bytes needed to encode the given value to ULEB-128 format.
    ///  
    ///  \example
    ///  \code
    ///      constexpr auto value = 777;
    ///      constexpr auto size = teg::uleb128::size(value);
    ///      static_assert(size == 2);
    ///  \endcode
    ///  
    teg_nodiscard teg_inline static constexpr auto encoded_size(u64 value) -> u8 {
        u8 size = 0;

        do {
            value >>= 7;
            size += sizeof(u8);
        } while (value != 0);

        return size;
    }

    ///  \brief Returns the maximum size in bytes that an encoded ULEB-128 value can have.
    ///  
    template <class T> requires std::integral<T>
    teg_nodiscard teg_inline static constexpr auto max_encoded_size() -> u8 {
        return encoded_size(std::numeric_limits<T>::max());
    }

    ///  \brief Returns the minimum size in bytes that an encoded ULEB-128 value can have.
    ///  
    template <class T> requires std::integral<T>
    teg_nodiscard teg_inline static constexpr auto min_encoded_size() -> u8 {
        return encoded_size(std::numeric_limits<T>::min());
    }

    ///  \brief Encodes the given value to ULEB-128 format.
    ///  
    ///  \param data The buffer to store the encoded value.
    ///  \param value The value to encode.
    ///  \return The number of bytes stored in the buffer. In case of failure, returns 0.
    ///  
    ///  \example
    ///  \code
    ///      constexpr auto value = 777;
    ///      teg::u8 data[2];
    ///      teg::uleb128::encode(data, value);
    ///      assert(data[0] == 0b10001001 && data[1] == 0b00000110);
    ///  \endcode
    ///  
    teg_inline static constexpr auto encode(std::span<u8> data, u64 value) -> u8 {
        u8 size = 0;

        do {
            if (size >= data.size()) teg_unlikely {                
                return 0;
            }

            u8 byte = value & 0b01111111;
            value >>= 7;

            if (value != 0) {
                byte = byte | 0b10000000;
            }
            data[size++] = byte;
        } 
        while (value != 0);

        return size;
    }

    ///  \brief Decodes the given value from ULEB-128 format.
    ///  
    ///  \param data The buffer containing the encoded value.
    ///  \param value The value to decode.
    ///  \return The number of bytes read from the buffer. In case of failure, returns 0.
    ///  
    ///  \example
    ///  \code
    ///      teg::u8 data[2] = { 0b10001001, 0b00000110 };
    ///      teg::u64 value;
    ///      teg::uleb128::decode(data, value);
    ///      assert(value == 777);
    ///  \endcode
    ///  
    teg_inline static constexpr auto decode(std::span<u8 const> data, u64& value) -> u8 {
        value = 0;
        u64 shift = 0;
        u8 size = 0;

        while (true) {
            if (size >= data.size()) teg_unlikely {
                value = 0;
                return 0;
            }

            u8 byte = data[size++];
            value |= u64(byte & 0b01111111) << shift;

            if ((byte & 0b10000000) == 0) {
                break;
            }

            shift += 7;
        }

        return size;
    }
};

template <class T = usize> 
    requires (std::integral<T>) && (sizeof(T) >= 4 && sizeof(T) <= 8)
class varint {
public:
    ///  \brief The underlying integral type.
    ///
    using value_type = T;

    ///  \brief Constructs an uninitialized varint value.
    ///
    teg_inline constexpr varint() = default;

    ///  \brief Constructs a varint with the given value.
    ///
    teg_inline constexpr varint(value_type value) : m_value(value) {}    

    ///  \brief Converts the varint value to the underlying integral type.
    ///  \details Implicit conversion.
    ///
    teg_inline constexpr operator value_type() const { return m_value; }

    ///  \brief Converts the varint value to the underlying integral type reference.
    ///  \details Implicit conversion.
    ///
    teg_inline constexpr operator value_type&() & { return m_value; }

private:
    T m_value;
};

///  \brief A signed variable-length integer (32-bit memory representation).
///  
using vint32 = varint<i32>;

///  \brief A signed variable-length integer (64-bit memory representation).
///
using vint64 = varint<i64>;

///  \brief An unsigned variable-length integer (32-bit memory representation).
///  
using vuint32 = varint<u32>;

///  \brief An unsigned variable-length integer (64-bit memory representation).
///  
using vuint64 = varint<u64>;

template <class F, class T>
teg_nodiscard teg_inline auto usr_encoding_size(F&& encoding_size, varint<T> var) -> u64 {
    if constexpr (std::is_signed_v<T>) {
        return uleb128::encoded_size(zigzag::encode((T)var));
    }
    else {
        return uleb128::encoded_size((T)var);
    }
}

template <class F, class T>
teg_nodiscard teg_inline auto usr_encode(F&& encode, varint<T> var) -> error {
    
    using value_type = std::make_unsigned_t<typename varint<T>::value_type>;
    value_type value = [var]() constexpr {
            if constexpr (std::is_signed_v<T>) {
                return zigzag::encode((T)var);
            }
            else {
                return var;
            }
        }();

    do {
        u8 byte = value & 0b01111111;
        value >>= 7;

        if (value != 0) {
            byte = byte | 0b10000000;
        }

        if (auto const result = encode(byte); failure(result)) teg_unlikely {
            return result;
        }
    }
    while (value != 0);

    return {};
}

template <class F, class T>
teg_nodiscard teg_inline auto usr_decode(F&& decode, varint<T>& var) -> error {
    
    using value_type = std::make_unsigned_t<typename varint<T>::value_type>;
        
    value_type value = 0;
    u64 shift = 0;
    u8 size = 0;

    while (size <= uleb128::max_encoded_size<value_type>()) {        
        u8 byte;     
        if (auto const result = decode(byte); failure(result)) teg_unlikely {
            return result;
        }
        
        value |= u64(byte & 0b01111111) << shift;
        
        if ((byte & 0b10000000) == 0) {
            if constexpr (std::is_signed_v<T>) {
                var = zigzag::decode(value);
                return {};
            }
            else {
                var = value;
                return {};
            }
        }

        shift += 7;
        size += 1;
    }

    return error { std::errc::value_too_large };
}

} // namespace teg


#endif // TEG_VARINT_H