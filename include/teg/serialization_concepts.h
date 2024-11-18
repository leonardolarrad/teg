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

#ifndef TEG_SERIALIZATION_CONCEPTS_H
#define TEG_SERIALIZATION_CONCEPTS_H

#include <type_traits>
#include <functional>

#include "core_concepts.h"
#include "def.h"
#include "error.h"
#include "options.h"

namespace teg::concepts {

///  \brief A user type with custom serialization.
///  
///  Determines whether a type has user-defined serialization or not.
///  
template <class T>
concept user_defined_serialization = 
    requires(
        T const& const_type, T & type, 
        std::function<teg::u64()> && size_func,
        std::function<teg::error()>&& serialize_func) 
    {
        { usr_serialized_size(size_func, const_type) } -> std::convertible_to<u64>;
        { usr_serialize(serialize_func, const_type) }  -> std::same_as<teg::error>;
        { usr_deserialize(serialize_func, type) }      -> std::same_as<teg::error>;
    };

///  \brief A serializable type.
///  
///  Determines whether a type is serializable or not.
///  
template <class T>
concept serializable = true;

///  \brief A type that can be deserialized.
///  
///  Determines whether a type can be deserialized or not.
///  
template <class T>
concept deserializable = serializable<T>;

template <class T>
concept serializable_builtin = fundamental<T> || is_enum<T>;

template <class T>
concept serializable_aggregate = 
        serializable<T> && aggregate<T>
    && !bounded_c_array<T> && !optional<T> && !variant<T> && !tuple<T> 
    && !container<T> && !owning_ptr<T> && !user_defined_serialization<T>;

template <class T>
concept serializable_c_array = 
        serializable<T> && bounded_c_array<T>
    && !optional<T> && !variant<T> && !tuple<T> && !container<T> 
    && !owning_ptr<T> && !user_defined_serialization<T>;

template <class T>
concept serializable_container = 
        serializable<T> && container<T> 
    && !optional<T> && !variant<T> && !owning_ptr<T> 
    && !user_defined_serialization<T>;

template <class T>
concept serializable_tuple =
        serializable<T> && tuple<T>
    && !optional<T> && !variant<T> && !container<T> 
    && !owning_ptr<T> && !user_defined_serialization<T>;

template <class T>
concept serializable_variant = 
        serializable<T> && variant<T>
    && !optional<T> && !tuple<T> && !container<T> 
    && !owning_ptr<T> && !user_defined_serialization<T>;

template <class T>
concept serializable_owning_ptr = 
        serializable<T> && owning_ptr<T>
    && !optional<T> && !variant<T> && !tuple<T> && !container<T> 
    && !user_defined_serialization<T>;

template <class T>
concept serializable_optional = 
        serializable<T> && optional<T>
    && !variant<T> && !tuple<T> && !container<T> 
    && !owning_ptr<T> && !user_defined_serialization<T>;

///  \brief A memory copyable type.
///  
///  A type if memory copyable if it is a fundamental type or a enum type, or
///  if it is trivially copyable type that does not have padding bits and does not require
///  endian-swapping.
///  
template <class T, options Opt>
concept memory_copyable = 
       (fundamental<T> || is_enum<T>)
    || (trivially_copyable<T> && packed_standard_layout<T> && !endian_swapping_required<T, Opt>);

///  \brief A type that cannot be trivially serialized.
///  
template <class T>
concept non_trivially_serializable = 
    container<T> || optional<T> || owning_ptr<T> || tuple<T> || variant<T> || user_defined_serialization<T>;

///  \brief A trivially serializable container.
///  
///  A fixed-size container that contains memory copyable elements.
///  
///  \details If endian-swapping is required a container can still be trivially serialized
///  but its elements must be endian-neutral.
///  
template <class T, options Opt>
concept trivially_serializable_container =
       (fixed_size_container<T>) 
    && (memory_copyable<typename T::value_type, Opt>)
    && (!endian_swapping_required<T, Opt>);

///  \brief A trivially serializable type.
///  
///  A type that can be memory copied directly into a buffer.
///  
template <class T, options Opt>
concept trivially_serializable = 
    memory_copyable<T, Opt> && !non_trivially_serializable<T>;

template <class T, options Opt>
concept trivially_deserializable = trivially_serializable<T, Opt>;

} // namespace teg::concepts

#endif // TEG_SERIALIZATION_CONCEPTS_H
