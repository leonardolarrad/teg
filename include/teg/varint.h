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

#include <tuple>
#include <limits>
#include <span>

#include "def.h"
#include "error.h"
#include "serialization_concepts.h"

namespace teg {

class uleb128 {
public:
    ///  \brief Constructs an uninitialized ULEB-128 value.
    ///  
    teg_inline constexpr uleb128() = default;

    ///  \brief Constructs a ULEB-128 value from a given 64-bit unsigned integer.
    ///  
    teg_inline constexpr uleb128(u64 value) : m_value(value) {}

    ///  \brief Converts the ULEB-128 value to a 64-bit unsigned integer.
    ///  \details Implicit conversion.
    ///  
    teg_inline constexpr operator u64() const { return m_value; }

    ///  \brief Converts the ULEB-128 value to a 64-bit unsigned integer reference.
    ///  \details Implicit conversion.
    ///  
    teg_inline constexpr operator u64&() & { return m_value; }

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
    teg_nodiscard teg_inline static constexpr auto size(u64 value) -> u8 {
        u8 size = 0;

        do {
            value >>= 7;
            size += sizeof(u8);
        } while (value != 0);

        return size;
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

private:
    u64 m_value;
};

class ileb128 {

    teg_nodiscard teg_inline static constexpr auto size(i64 value) -> u8 {
        u8 size = 0;
        i8 const sign = value >> (sizeof(i64) * 8 - 1);
        bool more = false;

        do {
            u8 byte = value & 0b01111111;
            value >>= 7;
            more = value != sign || ((byte ^ sign) & 0b01000000) != 0;
            size += sizeof(u8);
        } while (more);

        return size;
    }

};

using varint = ileb128;
using uvarint = uleb128;

} // namespace teg


#endif // TEG_VARINT_H