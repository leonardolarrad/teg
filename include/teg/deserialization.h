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

#ifndef TEG_DESERIALIZATION_H
#define TEG_DESERIALIZATION_H

#include <bit>
#include <concepts>
#include <cstddef>
#include <cstring>
#include <optional>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include "buffer.h"
#include "c_array.h"
#include "container_concepts.h"
#include "core_concepts.h"
#include "error.h"
#include "members_visitor.h"
#include "serialization.h"

namespace teg::concepts {

template <class T>
concept deserializable = serializable<T>;

template <class T>
concept trivially_deserializable = trivially_serializable<T>;

} // namespace teg::concepts

namespace teg {

template <class B = byte_buffer>
    requires concepts::byte_buffer<B>
class binary_deserializer {
public:
    static constexpr bool has_resizable_buffer = concepts::resizable_container<B>;

    using span_type = decltype(std::span{std::declval<B&>()});
    using buffer_type = std::conditional_t<has_resizable_buffer, B&, span_type>;
    using byte_type = typename std::remove_reference_t<buffer_type>::value_type;

    binary_deserializer() = delete;
    binary_deserializer(binary_deserializer const&) = delete;
    binary_deserializer& operator=(binary_deserializer const&) = delete;

    constexpr explicit binary_deserializer(B & buffer) : m_buffer(buffer), m_position(0) {}
    constexpr explicit binary_deserializer(B && buffer) : m_buffer(buffer), m_position(0) {}

    template <class... T> requires (concepts::deserializable<T> && ...)
    [[nodiscard]] constexpr inline auto deserialize(T&... objs) -> error {        
        if constexpr (sizeof...(objs) == 0) {
            return {};
        }
                
        return deserialize_many(objs...);
    }

private:
    template <class T0, class... TN>
    [[nodiscard]] constexpr inline auto deserialize_many(T0& first_obj, TN&... remaining_objs) -> error {        
        if (auto result = deserialize_one(first_obj); failure(result)) [[unlikely]] {
            return result;
        }

        return deserialize_many(remaining_objs...);
    }

    [[nodiscard]] constexpr inline auto deserialize_many() -> error {
        return {};
    }
    
    template <class T>
        requires (concepts::trivially_deserializable<T>)
    [[nodiscard]] constexpr inline auto deserialize_one(T& obj) -> error {
        if (std::is_constant_evaluated()) {
            // Deserialize at compile-time.
            using type = std::remove_cvref_t<T>;
            using src_array_type = std::array<std::remove_cv_t<byte_type>, sizeof(type)>;

            src_array_type src_array;
            for (auto& src_byte : src_array) {
                src_byte = m_buffer[m_position++];
            }

            if constexpr (!concepts::c_array<type>) {
                // The object being deserialized is not a c-array and we can simply `bit-cast` its content.
                obj = std::bit_cast<type>(src_array);
                return {};
            }
            else {
                // The object being deserialized is a c-array and can have multiple dimensions (rank can be > 1).
                using c_array_type = type;
                using element_type = std::remove_cvref_t<std::remove_all_extents_t<c_array_type>>;
                using tmp_array_type = std::array<element_type, sizeof(c_array_type) / sizeof(element_type)>;

                // In C++ functions can not return c-arrays, and therefore, std::bit_cast can not be used directly.
                // Instead, we have to create a temporary array and then copy its elements one by one.
                tmp_array_type tmp_array = std::bit_cast<tmp_array_type>(src_array);
                copy_md_c_array(obj, tmp_array);
                
                return {};
            }
        }
        else {
            // Deserialize at run-time.
            auto* dst = reinterpret_cast<std::remove_cv_t<byte_type>*>(&obj);
            auto* const src = m_buffer.data() + m_position;
            auto const size = sizeof(obj);

            std::memcpy(dst, src, size);
            m_position += size;
            return {};
        }
    }

    template <class T>
        requires (concepts::aggregate<T>)
              && (!concepts::bounded_c_array<T>)
              && (!concepts::container<T>)
              && (!concepts::trivially_deserializable<T>)
    [[nodiscard]] constexpr inline auto deserialize_one(T& aggregate) -> error {
        return visit_members(
            [&](auto&... members) {
                return deserialize_many(members...);
            },
            aggregate
        );
    }

    template <class T>
        requires (concepts::bounded_c_array<T> || concepts::fixed_size_container<T>) 
              && (!concepts::trivially_deserializable<T>)
    [[nodiscard]] inline constexpr auto deserialize_one(T& array) -> error {
        // Deserialize the elements.            
        for (auto& element : array) {
            if (auto result = deserialize_one(element); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};        
    }

    template <class T> 
        requires (concepts::container<T>)
              && (!concepts::fixed_size_container<T>)
              && (!concepts::trivially_deserializable<T>)
    [[nodiscard]] inline constexpr auto deserialize_one(T& container) -> error {
        using type = std::remove_reference_t<T>;
        using value_type = typename type::value_type;
        using size_type = typename type::size_type;

        if constexpr (concepts::clearable_container<type>) {
            // Pre-condition: the container should be empty.
            container.clear();
        }

        // Deserialize the size.
        size_type size;
        if (auto result = deserialize_one(size); failure(result)) [[unlikely]] {
            return result;
        }

        // Deserialize the elements.
        if constexpr (
               concepts::contiguous_container<type>
            && concepts::resizable_container<type> 
            && concepts::trivially_serializable<value_type>
        ) {
            // Optimized path: memory copy elements.
            container.resize(size);

            auto* dst = reinterpret_cast<byte_type*>(container.data());
            auto const* src = m_buffer.data() + m_position;
            auto const size = container.size() * sizeof(value_type);

            std::memcpy(dst, src, size);
            m_position += size;

            return {};
        }
        else {
            // Unoptimized path: deserialize elements one by one.
            // Select the most suitable method to emplace the elements based on the container type.

            if constexpr (concepts::reservable_container<type>) {
                // Optimization: pre-allocate uninitialized memory whenever possible.
                container.reserve(size);
            }

            if constexpr (concepts::map_container<type>) {
                // By default, `std::map` uses `std::pair<const key_type, mapped_type>` as its value_type.
                // We cannot deserialize const-qualified types, so we need to remove the const qualifiers.
                using key_value_element = std::pair<typename type::key_type, typename type::mapped_type>;
                key_value_element element;

                for (size_type i = 0; i < size; ++i) {                    
                    if (auto result = deserialize_one(element); failure(result)) [[unlikely]] {
                        return result;
                    }
                    container.emplace(std::move(element));
                }
                return {};
            }
            else if constexpr (concepts::inplace_constructing_container<type>) {
                // Construct the elements and then move them into the container.
                value_type element;
                for (size_type i = 0; i < size; ++i) {
                    if (auto result = deserialize_one(element); failure(result)) [[unlikely]] {
                        return result;
                    }
                    container.emplace(std::move(element));
                }
                return {};
            }
            else if constexpr (concepts::back_inplace_constructing_container<type>) {
                // Emplace the elements at the back of the container.
                for (size_type i = 0; i < size; ++i) {                
                    if (auto result = deserialize_one(container.emplace_back()); failure(result)) [[unlikely]] {
                        return result;
                    }
                }
                return {};
            }
            else if constexpr (concepts::front_inplace_constructing_container<type>) {
                // Worst-case scenario: the container constructs its elements at the front of its storage.
                // We may need to reverse the container after deserialization.
                for (size_type i = 0; i < size; ++i) {
                    if (auto result = deserialize_one(container.emplace_front()); failure(result)) [[unlikely]] {
                        return result;
                    }
                }

                if constexpr (concepts::invertible_container<type>) {
                    container.reverse();
                }
                return {};
            }
            else {
                static_assert(!sizeof(T), "Unsupported container type.");
            }
        }
    }

    template <class T> requires (concepts::owning_ptr<T>)
    [[nodiscard]] inline constexpr auto deserialize_one(T& pointer) -> error {
        using type = std::remove_reference_t<T>;
        using element_type = typename type::element_type;

        // Deserialize the element.
        auto data = std::make_unique<element_type>();
        if (auto result = deserialize_one(*data); failure(result)) [[unlikely]] {
            return result;
        }

        // Transfer ownership.
        pointer.reset(data.release());
        return {};
    }

    template <class T> requires (concepts::optional<T>)
    [[nodiscard]] inline constexpr auto deserialize_one(T& optional) -> error {
        using type = std::remove_reference_t<T>;
        using value_type = typename type::value_type;
        
        // Deserialize the `has_value` flag.
        byte_type has_value;    
        if (auto result = deserialize_one(has_value); failure(result)) [[unlikely]] {
            return result;
        }

        // Check if there is a value to deserialize.
        if (!bool(has_value)) {
            optional = std::nullopt;
            return {};
        }

        // Deserialize the value.
        if (!optional.has_value()) {
            // The optional may have a default value. If not, we default-construct one.
            optional = value_type{};
        }
        return deserialize_one(*optional);
    }

    template <class T> 
        requires (concepts::tuple<T>) 
              && (!concepts::container<T>)
              && (!concepts::trivially_deserializable<T>)
    [[nodiscard]] inline constexpr auto deserialize_one(T& tuple) -> error {
        return std::apply(
            [&](auto&&... elements) constexpr {
                return deserialize_many(elements...);
            },
            tuple
        );
    }

    template <class T> requires (concepts::variant<T>)
    [[nodiscard]] inline constexpr auto deserialize_one(T& variant) -> error {
        using variant_type = std::remove_reference_t<T>;

        // Deserialize the index.
        std::size_t runtime_index;
        if (auto result = deserialize_one(runtime_index); failure(result)) [[unlikely]] {
            return result;
        }

        // Deserialize the element.
        constexpr std::size_t table_size = std::variant_size_v<variant_type>;

        if (runtime_index >= table_size) [[unlikely]] {
            return error { std::errc::invalid_argument };
        }
        
        return index_table_lookup<table_size>(runtime_index, [&](auto comptime_index) {
            // Using an index table we can transform a runtime index into a compile-time index. 
            // With this technique we can then deserialize the variant alternative (based on the index)
            // at run-time.
            std::variant_alternative_t<comptime_index, variant_type> element;        
            if (auto result = deserialize_one(element); failure(result)) [[unlikely]] {
                return result;
            }

            variant.template emplace<comptime_index>(std::move(element));
            return teg::error {};
        });
    }

private:
    buffer_type m_buffer = {};
    std::size_t m_position = 0;
};

template <class B, class... T>
    requires (concepts::byte_buffer<B>) && (concepts::deserializable<T> && ...)
constexpr inline auto deserialize(B & input_buffer, T&... objs) -> error {
    return binary_deserializer<B>{input_buffer}.deserialize(objs...);
}

} // namespace teg

#endif // TEG_DESERIALIZATION_H 