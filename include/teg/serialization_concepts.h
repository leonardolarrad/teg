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

#include "teg/def.h"
#include "teg/alignment.h"
#include "teg/core_concepts.h"
#include "teg/container_concepts.h"
#include "teg/error.h"
#include "teg/compatible.h"
#include "teg/options.h"
#include "teg/endian.h"
#include "teg/members_get.h"

namespace teg::concepts {

///  \brief A user type with custom serialization.
///  
///  Determines whether a type has user-defined serialization or not.
///  
template <class T>
concept user_defined_encoding = 
    requires(
        T const& const_type, T & type, 
        std::function<teg::u64()> && size_func,
        std::function<teg::error()>&& serialize_func) 
    {
        { encoded_size(size_func, const_type) } -> std::convertible_to<u64>;
        { encode(serialize_func, const_type) }  -> std::same_as<teg::error>;
        { decode(serialize_func, type) }        -> std::same_as<teg::error>;
    };

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
        { usr_serialized_size(size_func, const_type) } -> std::convertible_to<teg::u64>;
        { usr_serialize(serialize_func, const_type) }  -> std::same_as<teg::error>;
        { usr_deserialize(serialize_func, type) }      -> std::same_as<teg::error>;
    };

template <class T>
concept has_valid_compatibility_compoisiton = true;

template <class T>
concept builtin = fundamental<T> || is_enum<T>;

template <class T>
concept match_aggregate = 
        aggregate<T>
    && !bounded_c_array<T> && !optional<T> && !variant<T> && !tuple<T> 
    && !container<T> && !owning_ptr<T> && !user_defined_serialization<T>;

template <class T>
concept match_c_array = 
        bounded_c_array<T>
    && !optional<T> && !variant<T> && !tuple<T> && !container<T> 
    && !owning_ptr<T> && !user_defined_serialization<T>;

template <class T>
concept match_container = 
        container<T> 
    && !optional<T> && !variant<T> && !owning_ptr<T> 
    && !user_defined_serialization<T>;

template <class T>
concept match_tuple =
        tuple<T>
    && !optional<T> && !variant<T> && !container<T> 
    && !owning_ptr<T> && !user_defined_serialization<T>;

template <class T>
concept match_variant = 
        variant<T>
    && !optional<T> && !tuple<T> && !container<T> 
    && !owning_ptr<T> && !user_defined_serialization<T>;

template <class T>
concept match_owning_ptr = 
        owning_ptr<T>
    && !optional<T> && !variant<T> && !tuple<T> && !container<T> 
    && !user_defined_serialization<T>;

template <class T>
concept match_optional = 
        optional<T>
    && !variant<T> && !tuple<T> && !container<T> && !owning_ptr<T> 
    && !compatible<T> && !user_defined_serialization<T>;

template <class T>
concept match_compatible = compatible<T>;

///  \brief A memory copyable type.
///  
///  A type if memory copyable if it is a fundamental type or a enum type, or
///  if it is trivially copyable type that does not have padding bits and does not require
///  endian-swapping.
///  
template <class T, options Opt>
concept safe_memory_copyable = (fundamental<T> || is_enum<T>)
    || (packed_layout<T> && !endian_swapping_required<T, Opt> && !is_varint_forced<Opt>());

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
    && (safe_memory_copyable<typename T::value_type, Opt>)
    && (!endian_swapping_required<T, Opt>);

///  \brief A trivially serializable type.
///  
///  A type that can be memory copied directly into a buffer.
///  
template <class T, options Opt>
concept trivially_serializable = 
    safe_memory_copyable<T, Opt> && !non_trivially_serializable<T>;

template <class T, options Opt>
concept trivially_deserializable = trivially_serializable<T, Opt>;

} // namespace teg::concepts

namespace teg::concepts::internal {

class type_analyzer {
public:
    static constexpr u32 max_recursion_depth = 100;

    template <class T, u32 Depth = 0>
    static constexpr auto is_serializable() -> bool {
        return is_serializable_impl<T, Depth>(); 
    }

private:    
    template <class T, u32 Depth>
    static constexpr auto is_serializable_impl() -> bool { 
        return false; // Max recursion depth reached or invalid type.
    }
    
    template <class T, u32 Depth>
    requires builtin<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool { 
        return true;
    }
    
    template <class T, u32 Depth>
    requires match_aggregate<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool {
        return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> bool {
            return (
                (is_serializable_impl<
                    std::remove_cvref_t<decltype(get_member<I>(T{}))>, Depth + 1>()
                ) && ...
            );
        }(std::make_index_sequence<members_count_v<T>>{});
    }
    
    template <class T, u32 Depth>
    requires match_c_array<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool { 
        return is_serializable<std::remove_all_extents_t<T>, Depth + 1>();
    }
    
    template <class T, u32 Depth>
    requires match_container<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool { 
        return is_serializable<typename T::value_type, Depth + 1>();
    }
    
    template <class T, u32 Depth>
    requires match_tuple<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool { 
        return std::apply(
            [](auto&&... members) { 
                return (is_serializable<std::remove_cvref_t<decltype(members)>, Depth + 1>() && ...); 
            },
            T{});
    }
    
    template <class T, u32 Depth>
    requires match_variant<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool {
        return []<size_t... I>(std::index_sequence<I...>) {
            return ((is_serializable<std::remove_cvref_t<std::variant_alternative_t<I, T>>, Depth + 1>()) && ...);
        }(std::make_index_sequence<std::variant_size_v<T>>{});
    }
    
    template <class T, u32 Depth>
    requires match_owning_ptr<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool { 
        return is_serializable<std::remove_cvref_t<typename T::element_type>, Depth + 1>(); 
    }
    
    template <class T, u32 Depth>
    requires match_optional<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool { 
        return is_serializable<std::remove_cvref_t<typename T::value_type>, Depth + 1>(); 
    }
    
    template <class T, u32 Depth>
    requires match_compatible<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool { 
        return is_serializable<std::remove_cvref_t<typename T::value_type>, Depth + 1>(); 
    }
    
    template <class T, u32 Depth>
    requires user_defined_serialization<T> && (Depth < max_recursion_depth)
    static constexpr auto is_serializable_impl() -> bool { 
        return true; 
    }
};

} // namespace teg::internal

namespace teg::concepts {

///  \brief A serializable type.
///  
///  Determines whether a type is serializable or not.
///  
template <class T>
concept serializable = internal::type_analyzer::is_serializable<T>();

///  \brief A type that can be deserialized.
///  
///  Determines whether a type can be deserialized or not.
///  
template <class T>
concept deserializable = serializable<T>;

} // namespace teg::concepts

#endif // TEG_SERIALIZATION_CONCEPTS_H
