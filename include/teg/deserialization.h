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
#include "def.h"
#include "error.h"
#include "members_visitor.h"
#include "options.h"
#include "serialization.h"
#include "serialization_concepts.h"

namespace teg {

///  \brief A binary deserializer.
///  
///  Decodes data from a given buffer using a binary format.
///  
///  \tparam  Buf  The type of the buffer to deserialize from.
///  \tparam  Opt  The options for deserialization.
///  
template <options Opt = default_mode, class Buf = byte_buffer>
    requires concepts::byte_buffer<Buf>
class binary_deserializer {
public:
    ///  \brief A type used to represent a portion of the buffer or the entire buffer itself.
    ///  
    using span_type = decltype(std::span{std::declval<Buf&>()});
    
    ///  \brief The type used to represent the buffer in which the data will be serialized.
    ///  \see `teg::concepts::byte_buffer`
    ///
    using buffer_type = std::conditional_t<concepts::resizable_container<Buf>, Buf&, span_type>;
    
    ///  \brief The type used to represent a byte.
    ///  \see `teg::concepts::byte`
    ///  
    using byte_type = std::remove_const_t<typename std::remove_reference_t<buffer_type>::value_type>;
    
    ///  \brief The type used to represent the size of the container being serialized.
    ///  
    using container_size_type = get_container_size_type<Opt>;

    ///  \brief The type used to represent the index of the variant being serialized.
    ///  
    using variant_index_type = get_variant_index_type<Opt>;

    ///  \brief Defines whether the buffer is resizable or not.
    ///  
    static constexpr bool has_resizable_buffer = concepts::resizable_container<Buf>;

    ///  \brief The buffer's maximum in-memory size.
    ///
    static constexpr u64 allocation_limit = get_allocation_limit<Opt>();

    ///  \brief The maximum size of elements a container can have.
    ///  
    static constexpr u64 max_container_size = std::numeric_limits<container_size_type>::max();

    ///  \brief The maximum number of alternatives a variant can have.
    ///  
    static constexpr u64 max_variant_index = std::numeric_limits<variant_index_type>::max();

    // Non-default-initializable and non-copyable.
    binary_deserializer() = delete;
    binary_deserializer(binary_deserializer const&) = delete;
    binary_deserializer& operator=(binary_deserializer const&) = delete;

    ///  \brief Construct a new binary deserializer.
    ///  \param buffer The buffer to deserialize from.
    ///  
    constexpr explicit binary_deserializer(Buf & buffer)  : m_buffer(buffer), m_position(0) {}
    constexpr explicit binary_deserializer(Buf && buffer) : m_buffer(buffer), m_position(0) {}

    template <class... T> requires (concepts::deserializable<T> && ...)
    teg_nodiscard teg_inline constexpr auto deserialize(T&... objs) -> error {        
        if constexpr (sizeof...(objs) == 0) {
            return {};
        }        
        return deserialize_many(objs...);
    }

private:
    ///  \brief Deserializes the given objects.
    ///  
    template <class T0, class... TN>
    teg_nodiscard teg_inline constexpr auto deserialize_many(T0& first_obj, TN&... remaining_objs) -> error {        
        if (auto const result = deserialize_one(first_obj); failure(result)) [[unlikely]] {
            return result;
        }

        return deserialize_many(remaining_objs...);
    }

    ///  \brief Case where there is no more objects to deserialize.
    ///  
    teg_nodiscard teg_inline constexpr auto deserialize_many() -> error {
        return {};
    }
    
    ///  \brief Deserializes the given trivially-serializable object.
    ///  
    template <class T>
        requires (concepts::trivially_deserializable<T, Opt>)
              || (concepts::trivially_serializable_container<T, Opt>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& trivial) -> error {
        return read_bytes(trivial);
    }

    ///  \brief Deserializes the given aggregate.
    ///  
    template <class T>
        requires (concepts::aggregate<T>)
              && (!concepts::bounded_c_array<T>)
              && (!concepts::container<T>)
              && (!concepts::tuple<T>)
              && (!concepts::trivially_deserializable<T, Opt>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& aggregate) -> error {
        return visit_members(
            [&](auto&... members) {
                return deserialize_many(members...);
            },
            aggregate
        );
    }

    ///  \brief Deserializes a bounded c-array or a fixed-size container.
    ///  
    ///  \details This functions handles the deserialization only if they are not 
    ///  trivially serializable. Otherwise they get deserialized using the trivial deserialization.
    ///  
    template <class T>
        requires (concepts::bounded_c_array<T> || concepts::fixed_size_container<T>)
              && (!concepts::trivially_deserializable<T, Opt>)
              && (!concepts::trivially_serializable_container<T, Opt>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& array) -> error {
        // Deserialize the array's elements.       
        for (auto& element : array) {
            if (auto const result = deserialize_one(element); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }

    ///  \brief Deserialize a container.
    ///  
    ///  \details This function handles the deserialization of most container types, 
    ///  including contiguous containers and associative containers with a generic approach.
    ///  
    template <class T> 
        requires (concepts::container<T>)
              && (!concepts::fixed_size_container<T>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& container) -> error {
        
        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;

        if constexpr (concepts::clearable_container<container_type>) {
            // Pre-condition: the container should be empty.
            container.clear();
        }

        // Deserialize the size.
        container_size_type size;
        if (auto const result = deserialize_one(size); failure(result)) [[unlikely]] {
            return result;
        }

        // Deserialize the elements.
        if constexpr (
               concepts::contiguous_container<container_type>
            && concepts::resizable_container<container_type> 
            && concepts::trivially_serializable<element_type, Opt>
        ) {
            // Optimized path: memory copy elements.
            container.resize(size);
            return read_bytes(container);
        }
        else {
            // Unoptimized path: deserialize elements one by one.
            // Select the most suitable method to emplace the elements based on the container type.

            if constexpr (concepts::reservable_container<container_type>) {
                // Optimization: pre-allocate uninitialized memory whenever possible.
                container.reserve(size);
            }

            if constexpr (concepts::map_container<container_type>) {
                // By default, `std::map` uses `std::pair<const key_type, mapped_type>` as its element_type.
                // We cannot deserialize const-qualified types, so we need to remove the const qualifiers.
                using key_value_element = std::pair<typename container_type::key_type, typename container_type::mapped_type>;
                key_value_element element;

                for (container_size_type i = 0; i < size; ++i) {                    
                    if (auto const result = deserialize_one(element); failure(result)) [[unlikely]] {
                        return result;
                    }
                    container.emplace(std::move(element));
                }
                return {};
            }
            else if constexpr (concepts::inplace_constructing_container<container_type>) {
                // Construct the elements and then move them into the container.
                element_type element;
                for (container_size_type i = 0; i < size; ++i) {
                    if (auto const result = deserialize_one(element); failure(result)) [[unlikely]] {
                        return result;
                    }
                    container.emplace(std::move(element));
                }
                return {};
            }
            else if constexpr (concepts::back_inplace_constructing_container<container_type>) {
                // Emplace the elements at the back of the container.
                for (container_size_type i = 0; i < size; ++i) {                
                    if (auto const result = deserialize_one(container.emplace_back()); failure(result)) [[unlikely]] {
                        return result;
                    }
                }
                return {};
            }
            else if constexpr (concepts::front_inplace_constructing_container<container_type>) {
                // Worst-case scenario: the container constructs its elements at the front of its storage.
                // We may need to reverse the container after deserialization.
                for (container_size_type i = 0; i < size; ++i) {
                    if (auto const result = deserialize_one(container.emplace_front()); failure(result)) [[unlikely]] {
                        return result;
                    }
                }

                if constexpr (concepts::invertible_container<container_type>) {
                    container.reverse();
                }
                return {};
            }
            else {
                static_assert(!sizeof(T), "Unsupported container container_type.");
            }
        }
    }

    ///  \brief Deserializes the given owning pointer.
    ///  
    template <class T> requires (concepts::owning_ptr<T>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& pointer) -> error {
        using type = std::remove_reference_t<T>;
        using element_type = typename type::element_type;

        // Deserialize the element.
        auto data = std::make_unique<element_type>();
        if (auto const result = deserialize_one(*data); failure(result)) [[unlikely]] {
            return result;
        }

        // Transfer ownership.
        pointer.reset(data.release());
        return {};
    }

    ///  \brief Deserializes the given optional.
    ///  
    template <class T> requires (concepts::optional<T>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& optional) -> error {
        using type = std::remove_reference_t<T>;
        using value_type = typename type::value_type;
        
        // Deserialize the `has_value` flag.
        byte_type has_value;    
        if (auto const result = deserialize_one(has_value); failure(result)) [[unlikely]] {
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

    ///  \brief Deserializes the given tuple-like object. 
    ///
    template <class T> requires (concepts::tuple<T>) && (!concepts::container<T>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& tuple) -> error {
        return std::apply(
            [&](auto&&... elements) constexpr {
                return deserialize_many(elements...);
            },
            tuple
        );
    }

    ///  \brief Deserializes the given variant.
    ///  
    template <class T> requires (concepts::variant<T>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& variant) -> error {
        using variant_type = std::remove_reference_t<T>;

        // Deserialize the index.
        variant_index_type runtime_index;
        if (auto const result = deserialize_one(runtime_index); failure(result)) [[unlikely]] {
            return result;
        }

        // Deserialize the alternative.
        constexpr std::size_t table_size = std::variant_size_v<variant_type>;

        if (runtime_index >= table_size) [[unlikely]] {
            return error { std::errc::invalid_argument };
        }
        
        return index_table_lookup<table_size>(runtime_index, [&](auto comptime_index) {
            // Using an index table we can transform a runtime index into a compile-time index. 
            // With this technique we can then deserialize the variant alternative (based on the index)
            // at run-time.
            std::variant_alternative_t<comptime_index, variant_type> element;        
            if (auto const result = deserialize_one(element); failure(result)) [[unlikely]] {
                return result;
            }

            variant.template emplace<comptime_index>(std::move(element));
            return teg::error {};
        });
    }

    ///  \brief Deserializes the given user-defined serializable object.
    ///  
    template <class T> requires (concepts::user_defined_serialization<T>)
    teg_nodiscard teg_inline constexpr auto deserialize_one(T& usr_obj) -> error {
        return usr_deserialize(
            [&](auto&&... objs) constexpr {
                return deserialize_many(objs...);
            }, usr_obj);
    }

    ///  \brief Copies the underlying bytes of the given trivially serializable object
    ///  directly from the buffer.
    ///  
    ///  \details Performs endian-swapping if needed.
    ///  
    template <class T>
        requires (concepts::trivially_deserializable<T, Opt>)
              || (concepts::trivially_serializable_container<T, Opt>)
    teg_nodiscard teg_inline constexpr auto read_bytes(T& obj) -> error {        

        auto constexpr size = []() constexpr { // Calculate the object's size.
            if constexpr (concepts::trivially_serializable_container<T, Opt>) {
                return T{}.size() * sizeof(typename T::value_type);
            }
            else {
                return sizeof(T);
            }}();

        if (std::is_constant_evaluated()) {
            // Compile-time deserialization.
            using type = std::remove_cvref_t<T>;
            using src_array_type = std::array<std::remove_cv_t<byte_type>, sizeof(type)>;
            src_array_type src_array{};

            if constexpr (!concepts::endian_swapping_required<T, Opt>) {
                for (std::size_t i = 0; i < size; ++i) {
                    src_array[i] = m_buffer[m_position + i];
                }
            }
            else {
                for (std::size_t i = 0; i < size; ++i) {
                    src_array[size - 1 - i] = m_buffer[m_position + i];
                }
            }
            m_position += size;

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
            // Run-time deserialization.
            if (m_position + size > m_buffer.size()) {
                return error { std::errc::value_too_large };
            }

            auto* dst = reinterpret_cast<std::remove_cv_t<byte_type>*>(&obj);
            auto* const src = m_buffer.data() + m_position;            
            m_position += size;

            if constexpr (!concepts::endian_swapping_required<T, Opt>) {
                std::memcpy(dst, src, size);
                return {};
            }
            else {
                std::reverse_copy(src, src + size, dst);
                return {};
            }
        }
    }

    ///  \brief Copies the underlying bytes of the given contiguous container
    ///  directly from the buffer.
    ///  
    template <class T> 
        requires (concepts::contiguous_container<T>)
              && (concepts::trivially_serializable<typename T::value_type, Opt>)
              && (!concepts::trivially_serializable_container<T, Opt>)
    teg_nodiscard teg_inline constexpr auto read_bytes(T& container) -> error {
        // Deserialization at compile-time is not possible in this case.
        // Deserialize at run-time.
        auto* dst = reinterpret_cast<byte_type*>(container.data());
        auto const* src = m_buffer.data() + m_position;
        auto const size = container.size() * sizeof(typename T::value_type);

        if (m_position + size > m_buffer.size()) {
            return error { std::errc::value_too_large };
        }

        std::memcpy(dst, src, size);
        m_position += size;

        return {};
    }

private:
    buffer_type m_buffer = {};  // A reference to a contiguous container or span of data.
    u64 m_position = 0;         // The current position in the buffer.
};

template <options Opt = default_mode, class Buf, class... T>
    requires (concepts::byte_buffer<Buf>) && (concepts::deserializable<T> && ...)
teg_nodiscard teg_inline constexpr auto deserialize(Buf& input_buffer, T&... objs) -> error {
    
    // Create a binary deserializer and deserialize the given objects.
    return binary_deserializer<Opt, Buf>{input_buffer}.deserialize(objs...);
}

} // namespace teg

#endif // TEG_DESERIALIZATION_H 