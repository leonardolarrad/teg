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

#ifndef TEG_COMPATIBLE_H
#define TEG_COMPATIBLE_H

#include <optional>
#include <type_traits>
#include <concepts>

#include "teg/def.h"

namespace teg {

template <class T, u64 V>
class compatible : public std::optional<T> {
public:
    static constexpr u64 version = V;

    using std::optional<T>::optional;
        
    teg_inline constexpr compatible() = default;
    teg_inline constexpr compatible(T const& value) : std::optional<T>(value) {}
    teg_inline constexpr compatible(T&& value) : std::optional<T>(std::move(value)) {}
    teg_inline constexpr compatible(std::nullopt_t) : std::optional<T>(std::nullopt) {}
    teg_inline constexpr compatible(const compatible &other) = default;
    teg_inline constexpr compatible(compatible &&other) = default;
    teg_inline constexpr compatible(std::optional<T> &&other)
        : std::optional<T>(std::move(other)){};
    teg_inline constexpr compatible(const std::optional<T> &other)
        : std::optional<T>(other){};
    teg_inline constexpr compatible &operator=(const compatible &other) = default;
    teg_inline constexpr compatible &operator=(compatible &&other) = default;
};

template <class T, u64 V1, u64 V2>
teg_inline constexpr bool operator==(compatible<T, V1> const& l, compatible<T, V2> const& r) {
    if (V1 != V2) {
        return false;
    }
    else {
        return 
            static_cast<bool>(l) == static_cast<bool>(r) 
            && (!l || *l == *r);
    }
}

namespace concepts {

namespace internal {

template <class T>
constexpr inline bool is_compatible_v = false;

template <class T, u64 V>
constexpr inline bool is_compatible_v<compatible<T, V>> = true;

} // namespace internal

template <class T>
concept compatible = internal::is_compatible_v<T>; 

} // namespace concepts

/*///  \brief Returns the number of bytes required to encode the given */
/*///  compatible object.*/
/*///*/
/*///  \details User-defined serialization.*/
/*///  */
/*template <class F, class T, u64 V>*/
/*teg_nodiscard teg_inline auto usr_serialized_size(F&& serialized_size, compatible<T, V>& comp) -> u64 {*/
/*    return serialized_size(static_cast<std::optional<T>>(comp));*/
/*}*/
/**/
/*///  \brief Serializes a compatible object.*/
/*///  \details User-defined serialization.*/
/*///  */
/*template <class F, class T, u64 V>*/
/*teg_nodiscard teg_inline auto usr_serialize(F&& encode, compatible<T, V>& comp) -> error {*/
/*    auto const result = encode(static_cast<std::optional<T>>(comp));*/
/**/
/*    if (failure(result)) teg_unlikely {*/
/*        return result;*/
/*    }*/
/**/
/*    return {};*/
/*}*/
/**/
/*///  \brief Deserializes a compatible object.*/
/*///  \details User-defined serialization.*/
/*///  */
/*template <class F, class T, u64 V>*/
/*teg_nodiscard teg_inline auto usr_deserialize(F&& decode, compatible<T, V>& comp) -> error {*/
/*    std::optional<T> value;*/
/**/
/*    auto const result = decode(value);*/
/*    if (failure(result)) teg_unlikely {*/
/*        value = std::nullopt;*/
/*    }*/
/**/
/*    comp = value;*/
/*    return {};*/
/*}*/

} // namespace teg

#endif // TEG_COMPATIBLE_H
