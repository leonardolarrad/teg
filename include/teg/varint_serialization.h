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

#ifndef TEG_VARINT_SERIALIZATION_H
#define TEG_VARINT_SERIALIZATION_H

#include "teg/varint.h"

namespace teg {

///  \brief Returns the number of bytes required to serialize the given variable-length integer.
///  \details User-defined serialization.
///  
template <class F, class T>
TEG_NODISCARD TEG_INLINE constexpr auto usr_serialized_size(F&& serialized_size, varint<T> var) -> u64 {
    if constexpr (std::is_signed_v<T>) {
        return uleb128::encoded_size(zigzag::encode((T)var));
    }
    else {
        return uleb128::encoded_size((T)var);
    }
}

///  \brief Serializes the given variable-length integer.
///  \details User-defined serialization.
///  
template <class F, class T>
TEG_NODISCARD TEG_INLINE constexpr auto usr_serialize(F&& encode, varint<T> var) -> error {
    
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

///  \brief Deserializes the given variable-length integer.
///  \details User-defined serialization.
///  
template <class F, class T>
TEG_NODISCARD TEG_INLINE constexpr auto usr_deserialize(F&& decode, varint<T>& var) -> error {
    
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

#endif // TEG_VARINT_SERIALIZATION_H