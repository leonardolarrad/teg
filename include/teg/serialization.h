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

#ifndef TEG_SERIALIZATION_H
#define TEG_SERIALIZATION_H

#include <algorithm>
#include <bit>
#include <concepts>
#include <cstddef>
#include <cstring>
#include <span>
#include <tuple>
#include <type_traits>
#include <utility>

#include "alignment.h"
#include "buffer.h"
#include "c_array.h"
#include "container_concepts.h"
#include "core_concepts.h"
#include "def.h"
#include "endian.h"
#include "error.h"
#include "members_visitor.h"
#include "options.h"
#include "serialization_concepts.h"

namespace teg {

///  \brief A binary serializer.
///  
///  Encodes data into a given buffer using a binary format.
///  
///  \tparam  Buf  The type of the buffer to serialize into.
///  \tparam  Opt  The options for serialization.
///  
template <options Opt = default_mode, class Buf = byte_buffer>
    requires concepts::byte_buffer<Buf>
class binary_serializer {
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
    binary_serializer() = delete;
    binary_serializer(binary_serializer const&) = delete;
    binary_serializer& operator=(binary_serializer const&) = delete;

    ///  \brief Construct a new binary serializer.
    ///  \param buffer The buffer to serialize into.
    ///  
    constexpr explicit binary_serializer(Buf & buffer) : m_buffer(buffer), m_position(0) {}
    constexpr explicit binary_serializer(Buf && buffer) : m_buffer(buffer), m_position(0) {}

    ///  \brief Calculates the number of bytes needed to serialize the given objects.
    ///  
    ///  \tparam ...T The types of the objects to serialize.
    ///  \param ...objs The objects to serialize.
    ///  \return The number of bytes needed to serialize the given objects.
    ///  
    ///  \example
    ///  \code
    ///      constexpr auto serialized_size = 
    ///         teg::binary_serializer::serialized_size("A string!", 55, 9.99f);
    ///      
    ///      static_assert(serialized_size == 22);
    ///  \endcode
    ///  
    template <class... T> requires (concepts::serializable<T> && ...)
    teg_nodiscard teg_inline static constexpr auto serialized_size(T const&... objs) -> u64 {
        return serialized_size_many(objs...);
    }

    ///  \brief Binary serialization of the given objects.
    ///  
    ///  Performs a binary serialization of the given objects using the provided buffer 
    ///  and returns an error code indicating whether the serialization was successful.
    ///  
    ///  \tparam ...T The types of the objects to serialize.    
    ///  \param ...objs The objects to serialize.
    ///  \return An error code indicating whether the serialization was successful.
    ///  
    ///  \example
    ///  \code
    ///      auto buffer = teg::byte_buffer{};
    ///      auto serializer = teg::binary_serializer{buffer};
    ///      auto const result = serializer.serialize("A string!", 55, 9.99f);
    ///      
    ///      if (success(result)) {
    ///          for (auto const& byte : buffer) {
    ///              std::cout << static_cast<int>(byte);
    ///          }
    ///      } else {
    ///          std::cout << result.message();
    ///      }
    ///  \endcode
    ///  
    ///  \details The binary serialization algorithm is implemented as a series of calls 
    ///  to `serialize_many` and `serialize_one` using overload resolution through concepts.
    ///  
    template <class... T> requires (concepts::serializable<T> && ...)
    teg_nodiscard teg_inline constexpr auto serialize(T const&... objs) -> error {        
        if constexpr (sizeof...(objs) == 0) {
            return {};
        }

        if constexpr (has_resizable_buffer) {
            u64 const new_size = m_position + serialized_size(objs...);

            // Check allocation limit.
            if (new_size > allocation_limit) [[unlikely]] {
                return error { std::errc::value_too_large };
            }

            // Resize the buffer.
            using buffer_size_type = std::remove_cvref_t<buffer_type>::size_type;
            m_buffer.resize(static_cast<buffer_size_type>(new_size));

            if (auto const result = serialize_many(objs...); failure(result)) [[unlikely]] {                
                clear();
                return result;
            }
            return {};
        }
        else {
            // Check buffer space.
            if (m_buffer.size() < m_position + serialized_size(objs...)) [[unlikely]] {
                clear();
                return error { std::errc::no_buffer_space };
            }

            return serialize_many(objs...);
        }
    }

    ///  \brief Binary serialization of the given objects.
    ///  \see `binary_serilizer::serialize`
    ///  
    template <class... T> requires (concepts::serializable<T> && ...)
    teg_nodiscard teg_inline constexpr auto operator()(T const&... objs) -> error { 
        return serialize(objs...); 
    }

    teg_nodiscard teg_inline constexpr auto position() const -> u64 { 
        return m_position; 
    }

    teg_nodiscard teg_inline constexpr auto position() -> u64& {
        return m_position;
    }

    teg_nodiscard teg_inline constexpr auto data() const -> buffer_type const& {
        return m_buffer;
    }

    teg_nodiscard teg_inline constexpr auto size() const -> u64 {
        return m_buffer.size();
    }

    teg_inline constexpr auto reset() -> void {
        m_position = 0;
    }

    teg_inline constexpr auto clear() -> void {
        reset();
        
        if constexpr (teg::concepts::clearable_container<Buf>) {
            m_buffer.clear();
        }
    }

private:
    ///  \brief Calculates the encoding size of the given objects.
    ///  
    template <class T0, class... TN> 
    teg_nodiscard teg_inline static constexpr auto 
        serialized_size_many(T0 const& first_obj, TN const&... remaining_objs) -> u64 
    {
        return serialized_size_one(first_obj) + serialized_size_many(remaining_objs...);
    }

    ///  \brief Case where theres no objects to encode.
    ///  
    teg_nodiscard teg_inline static constexpr auto serialized_size_many() -> u64 {
        return 0;
    }

    ///  \brief Calculates the encoding size of the given trivially serializable type.
    ///  
    template <class T> requires (concepts::trivially_serializable<T, Opt>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& trivial) -> u64 {        
        return sizeof(trivial);
    }

    ///  \brief Calculates the encoding size of the given aggregate.
    ///  
    template <class T> 
        requires (concepts::aggregate<T>) 
              && (!concepts::bounded_c_array<T>)
              && (!concepts::container<T>)
              && (!concepts::tuple<T>)
              && (!concepts::trivially_serializable<T, Opt>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& aggregate) -> u64 {
        return visit_members(
            [&](auto&&... member) constexpr {
                return serialized_size_many(member...);
            },
            aggregate
        );
    }

    ///  \brief Calculates the encoding size of the given c-array.
    ///  
    template <class T> requires
           (concepts::bounded_c_array<T>) 
        && (!concepts::trivially_serializable<T, Opt>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& c_array) -> u64 {
        return std::size(c_array) * serialized_size_one(c_array[0]);
    }

    ///  \brief Calculates the encoding size of the given fixed-size container.
    ///  
    template <class T> requires (concepts::fixed_size_container<T>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& fixed_container) -> u64 {

        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;

        if constexpr (concepts::trivially_serializable<element_type, Opt>) {
            return sizeof(element_type) * fixed_container.size();
        } 
        else {
            u64 result = 0;
            for (auto const& element : fixed_container) {
                result += serialized_size_one(element);
            }
            return result;
        }
    }

    ///  \brief Calculates the encoding size of the given container.
    ///  
    template <class T> requires (concepts::container<T>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& container) -> u64 {

        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;
        
        u64 const container_size = [&]() constexpr {
            if constexpr (concepts::sized_container<container_type>) {
                return static_cast<u64>(container.size());
            } else {
                return static_cast<u64>(std::distance(container.begin(), container.end()));
            }
        }();

        u64 result = serialized_size_one(static_cast<container_size_type>(container_size));

        if constexpr (concepts::trivially_serializable<element_type, Opt>) {
            result += sizeof(element_type) * container_size;
            return result;
        }
        else {
            for (auto const& element : container) {
                result += serialized_size_one(element);
            }
            return result;
        }
        
        /*
        if constexpr (
            concepts::sized_container<container_type> &&
            concepts::trivially_serializable<element_type, Opt>
        ) {
            u64 const container_size = std::min(
                static_cast<u64>(container.size()), max_container_size);
            serialized_size += sizeof(element_type) * container_size;
            return serialized_size;
        } 
        else if constexpr (
            concepts::trivially_serializable<element_type, Opt>
        ) {
            u64 const container_size = std::min(
                static_cast<u64>(std::distance(container.begin(), container.end())), max_container_size);
            serialized_size += sizeof(element_type) * container_size;
            return serialized_size;        
        } else {
            for (auto const& element : container) {
                serialized_size += serialized_size_one(element);
            }
            return serialized_size;
        }
        */
    }

    ///  \brief Calculates the encoding size of the given owning pointer.
    ///  
    template <class T> requires (concepts::owning_ptr<T>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& ptr) -> u64 {
        if (ptr == nullptr) {
            return 0;
        }    
        return serialized_size_one(*ptr);
    }

    ///  \brief Calculates the encoding size of the given optional.
    ///  
    template <class T> requires (concepts::optional<T>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& optional) -> u64 {    
        if (optional.has_value()) {
            return sizeof(byte_type) + serialized_size_one(optional.value());
        }
        else {
            return sizeof(byte_type);
        }
    }

    ///  \brief Calculates the encoding size of the given tuple-like object.
    ///  
    template <class T> requires (concepts::tuple<T>) && (!concepts::container<T>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& tuple) -> u64 {    
        return std::apply(
            [](auto&&... elements) constexpr {
                return serialized_size_many(elements...);
            },
            tuple
        );
    }

    ///  \brief Calculates the encoding size of the given variant.
    ///  
    template <class T> requires (concepts::variant<T>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& variant) -> u64 {
        const u64 index_size = sizeof(variant_index_type);
        const u64 element_size = std::visit(
            [](auto&& value) constexpr {
                return serialized_size_one(value);
            },
            variant
        );
        return index_size + element_size;
    }

    ///  \brief Calculates the encoding size of a user-defined serializable type.
    ///  
    template <class T> requires (concepts::user_defined_serialization<T>)
    teg_nodiscard teg_inline static constexpr auto serialized_size_one(T const& usr_obj) -> u64 {
        return usr_serialized_size(
            [&](auto&&... objs) constexpr {
                return serialized_size_many(objs...);
            }, usr_obj);
    }

    ///  \brief Serializes the given objects.
    ///  
    template <class T0, class... TN> 
    teg_nodiscard teg_inline 
    constexpr auto serialize_many(T0 const& first_obj, TN const&... remaining_objs) -> error {        
        if (auto const result = serialize_one(first_obj); failure(result)) [[unlikely]] {
            return result;
        }

        return serialize_many(remaining_objs...);
    }

    ///  \brief Serialize zero objects.
    ///  
    teg_nodiscard teg_inline constexpr auto serialize_many() -> error {
        return {};
    }

    ///  \brief Serialize a trivially serializable object.
    ///  
    template <class T>
        requires (concepts::trivially_serializable<T, Opt>)
              || (concepts::trivially_serializable_container<T, Opt>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& trivial) -> error {
        return write_bytes(trivial);
    }

    ///  \brief Serialize the given aggregate.
    ///  
    template <class T>
        requires (concepts::aggregate<T>)
              && (!concepts::bounded_c_array<T>) 
              && (!concepts::container<T>)
              && (!concepts::tuple<T>)
              && (!concepts::trivially_serializable<T, Opt>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& aggregate) -> error {
        return visit_members(
            [&](auto&&... members) {
                return serialize_many(members...);
            },
            aggregate
        );
    }

    ///  \brief Serialize the given bounded c-array.
    ///  
    template <class T> 
        requires (concepts::bounded_c_array<T>) 
              && (!concepts::trivially_serializable<T, Opt>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& c_array) -> error {
        // Serialize elements.            
        for (auto const& element : c_array) {
            if (auto const result = serialize_one(element); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }

    ///  \brief Serialize a container.
    ///  
    ///  \details This function handles the serialization all container types, 
    ///  including fixed-size containers, contiguous containers, and associative containers, 
    ///  in a generic way.
    ///  
    template <class T> 
        requires (concepts::container<T>)
              && (!concepts::trivially_serializable_container<T, Opt>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& container) -> error {

        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;
        using native_size_type = typename container_type::size_type;
        
        // Serialize the container's size if needed.
        if constexpr (!concepts::fixed_size_container<container_type>) {
            // For fixed-size containers, the size is known at compile-time; thus, no serialization
            // is needed. Otherwise, serialize the container's size.
            
            if constexpr (concepts::sized_container<container_type>) {
                // Optimized path: we don't need to compute the size; 
                // it is already known at run-time.
                native_size_type const size = container.size();

                if (size > max_container_size) [[unlikely]] {
                    return error { std::errc::value_too_large };
                }

                auto const result = serialize_one(static_cast<container_size_type>(size));
                if (failure(result)) [[unlikely]] {
                    return result;
                }
            }
            else {
                // Non-optimized path: some containers (like `std::forward_list`) 
                // don't have a `size()` observer.
                native_size_type const size = std::distance(container.begin(), container.end());
                
                if (size > max_container_size) [[unlikely]] {
                    return error { std::errc::value_too_large };
                }

                auto const result = serialize_one(static_cast<container_size_type>(size));
                if (failure(result)) [[unlikely]] {
                    return result;
                }
            }
        }

        // Serialize the container's elements.
        if constexpr (
            concepts::contiguous_container<container_type> &&
            concepts::trivially_serializable<element_type, Opt>
        ) {
            // Optimized path: memory copy elements.            
            return write_bytes(container);   
        }
        else {
            // Non-optimized path: serialize each element one by one.
            for (auto const& element : container) {
                if (auto const result = serialize_one(element); failure(result)) [[unlikely]] {
                    return result;
                }
            }
            return {};
        }
    }

    ///  \brief Serializes the given owning pointer.
    ///  
    template <class T> requires (concepts::owning_ptr<T>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& ptr) -> error  {
        if (ptr == nullptr) [[unlikely]] {
            return std::errc::invalid_argument;
        }

        return serialize_one(*ptr);
    }

    ///  \brief Serializes the given optional.
    ///  
    template <class T> requires (concepts::optional<T>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& optional) -> error {
        if (!optional.has_value()) [[unlikely]] {
            return serialize_one(byte_type(false));
        } else {
            return serialize_many(byte_type(true), *optional);
        }
    }

    ///  \brief Serializes the given tuple-like object. 
    ///  
    template <class T> requires (concepts::tuple<T>) && (!concepts::container<T>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& tuple) -> error {    
        return std::apply(
            [&](auto&&... elements) constexpr {
                return serialize_many(elements...);
            },
            tuple
        );
    }

    ///  \brief Serializes the given variant.
    ///  
    template <class T> requires (concepts::variant<T>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& variant) -> error {
        // Check valueless by exception.
        if (variant.valueless_by_exception()) [[unlikely]] {
            return error { std::errc::invalid_argument };
        }

        // Serialize the index.
        auto const result = serialize_one(static_cast<variant_index_type>(variant.index()));
        if (failure(result)) [[unlikely]] {
            return result;        
        }    

        // Serialize the value.
        return std::visit(
            [&](auto&& value) constexpr {
                return serialize_one(value);
            },
            variant
        );
    }

    ///  \brief Serializes the given user-defined serializable object.
    ///  
    template <class T> requires (concepts::user_defined_serialization<T>)
    teg_nodiscard teg_inline constexpr auto serialize_one(T const& usr_obj) -> error {
        return usr_serialize(
            [&](auto&&... objs) constexpr {
                return serialize_many(objs...);
            }, usr_obj);
    }

    ///  \brief Copies the underlying bytes of the given trivially serializable object
    ///  directly into the buffer.
    ///  
    ///  \details Performs endian-swapping if needed.
    ///  
    template <class T> 
        requires (concepts::trivially_serializable<T, Opt>)
              || (concepts::trivially_serializable_container<T, Opt>)
    teg_nodiscard teg_inline constexpr auto write_bytes(T const& obj) -> error {

        auto constexpr size = []() constexpr { // Calculate the object's size.
            if constexpr (concepts::trivially_serializable_container<T, Opt>) {
                return T{}.size() * sizeof(typename T::value_type);
            }
            else {
                return sizeof(T);
            }}();

        if (std::is_constant_evaluated()) {
            // Compile-time serialization.
            using src_array_type = std::array<byte_type, sizeof(T)>;
            auto src_array = std::bit_cast<src_array_type>(obj);

            for (std::size_t i = 0; i < size; ++i) {
                if constexpr (!concepts::endian_swapping_required<T, Opt>) {
                    m_buffer[m_position + i] = src_array[i];
                }
                else {
                    m_buffer[m_position + i] = src_array[size - 1 - i];
                }
            }

            m_position += size;
            return {};
        }
        else {
            // Run-time serialization.
            auto* dst = m_buffer.data() + m_position;
            auto* const src = reinterpret_cast<byte_type const*>(&obj);
            
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
    ///  directly into the buffer.
    ///  
    template <class T>
        requires (concepts::contiguous_container<T>)
              && (concepts::trivially_serializable<typename T::value_type, Opt>)
              && (!concepts::trivially_serializable_container<T, Opt>)
    teg_nodiscard teg_inline constexpr auto write_bytes(T const& container) -> error {

        auto* dst = m_buffer.data() + m_position;
        auto const* src = reinterpret_cast<byte_type const*>(container.data());
        auto const size = container.size() * sizeof(typename T::value_type);
        
        std::memcpy(dst, src, size);
        m_position += size;
        
        return {};
    }

private:
    buffer_type m_buffer = {}; // A reference to a contiguous container or a span of data.
    u64 m_position = 0;   // The current position in the buffer.
};

///  \brief Serializes the provided objects into the specified buffer using a binary format.
///  
///  \tparam  Opt   The options for serialization.
///  \tparam  ...T  The types of the objects to serialize.
///  \tparam  Buf   The type of the buffer to serialize into.
///  
///  \param  output_buffer  The buffer to serialize the data into.
///  \param  objs...        The objects to be serialized.
///  
///  \return An error code indicating the success or failure of the serialization process.
///  
///  \example 
///  \code
///      teg::byte_buffer buffer{};
///      auto const result = teg::serialize(buffer, "A string!", 55, 9.99f);
///      
///      if (success(result)) { 
///          // Use the serialized bytes
///          for (auto const& byte : buffer) {
///              std::cout << static_cast<int>(byte); 
///          }
///      }
///      else { 
///          // Handle the error.
///          std::cout << result.message(); 
///      }
///  \endcode
///  
template <options Opt = default_mode, class Buf, class... T>
    requires (concepts::byte_buffer<Buf>) 
          && (concepts::serializable<T> && ...)
teg_nodiscard teg_inline constexpr auto serialize(Buf& output_buffer, T const&... objs) -> error {
    
    return binary_serializer<Opt, Buf>{output_buffer}.serialize(objs...);
}

} // namespace teg

#endif // TEG_SERIALIZATION_H 