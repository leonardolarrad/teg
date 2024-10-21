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

#include <cstddef>
#include <concepts>
#include <type_traits>
#include <utility>
#include <span>
#include <bit>
#include <cstring>

#include "alignment.h"
#include "buffer.h"
#include "container_concepts.h"
#include "core_concepts.h"
#include "error.h"
#include "members_visitor.h"

namespace teg::concepts {

template <class T>
concept serializable = true;

template <class T>
concept trivially_serializable = trivially_copyable<T> && packed_layout<T>;

} // namespace teg::concepts

namespace teg {

template <class B = byte_buffer>
    requires concepts::byte_buffer<B>
class binary_serializer {
public:
    static constexpr bool has_resizable_buffer = concepts::resizable_container<B>;

    using span_type = decltype(std::span{std::declval<B&>()});
    using buffer_type = std::conditional_t<has_resizable_buffer, B&, span_type>;
    using byte_type = typename std::remove_reference_t<buffer_type>::value_type;
    
    binary_serializer() = delete;
    binary_serializer(binary_serializer const&) = delete;
    binary_serializer& operator=(binary_serializer const&) = delete;

    constexpr explicit binary_serializer(B & buffer) : m_buffer(buffer), m_position(0) {}
    constexpr explicit binary_serializer(B && buffer) : m_buffer(buffer), m_position(0) {}

    template <class... T> requires (concepts::serializable<T> && ...)
    [[nodiscard]] static constexpr inline auto buffer_size(T const&... objs) ->  std::size_t {
        return buffer_size_many(objs...);
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
    ///      auto result = serializer.serialize("A string!", 55, 9.99f);
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
    [[nodiscard]] constexpr inline auto serialize(T const&... objs) -> error {        
        if constexpr (sizeof...(objs) == 0) {
            return {};
        }

        if constexpr (has_resizable_buffer) {
            m_buffer.resize(m_buffer.size() + buffer_size(objs...));
        }
        
        return serialize_many(objs...);
    }

private:
    // Buffer size implementation.

    template <class T0, class... TN> 
    [[nodiscard]] static constexpr inline auto 
        buffer_size_many(T0 const& first_obj, TN const&... remaining_objs) -> std::size_t 
    {
        return buffer_size_one(first_obj) + buffer_size_many(remaining_objs...);
    }

    [[nodiscard]] static constexpr inline auto buffer_size_many() -> std::size_t {
        return 0;
    }

    template <class T> 
        requires (concepts::trivially_serializable<T>)
    [[nodiscard]] static constexpr inline auto buffer_size_one(T const& trivial) -> std::size_t {        
        return sizeof(trivial);
    }

    template <class T> 
        requires (concepts::aggregate<T>) 
              && (!concepts::bounded_c_array<T>)
              && (!concepts::container<T>)
              && (!concepts::trivially_serializable<T>)
    [[nodiscard]] static constexpr inline auto buffer_size_one(T const& aggregate) -> std::size_t {
        return visit_members(
            [&](auto&&... member) constexpr {
                return buffer_size_many(member...);
            },
            aggregate
        );
    }

    template <class T> requires (concepts::bounded_c_array<T>) && (!concepts::trivially_serializable<T>)
    [[nodiscard]] static constexpr inline auto buffer_size_one(T const& c_array) -> std::size_t {
        return std::size(c_array) * buffer_size_one(c_array[0]);
    }

    template <class T> requires (concepts::container<T>) && (!concepts::trivially_serializable<T>)
    [[nodiscard]] static constexpr inline auto buffer_size_one(T const& container) -> std::size_t {
        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;
        using size_type = typename container_type::size_type;
        
        size_type size = 0;

        if constexpr (!concepts::fixed_size_container<container_type>) {
            size = sizeof(size_type);
        }

        if constexpr (
               concepts::sized_container<container_type>
            && concepts::trivially_serializable<element_type>
        ) {
            size += sizeof(element_type) * container.size();
            return size;
        } 
        else if constexpr (
            concepts::trivially_serializable<element_type>
        ) {
            size += sizeof(element_type) * std::distance(container.begin(), container.end());
            return size;        
        } else {
            for (auto const& element : container) {
                size += buffer_size_one(element);
            }
            return size;
        }        
    }

    template <class T> requires (concepts::owning_ptr<T>)
    [[nodiscard]] static constexpr inline auto buffer_size_one(T const& ptr) -> std::size_t {
        if (ptr == nullptr) {
            return 0;
        }    
        return buffer_size_one(*ptr);
    }

    template <class T> requires (concepts::optional<T>)
    [[nodiscard]] static constexpr inline auto buffer_size_one(T const& optional) -> std::size_t {    
        if (optional.has_value()) {
            return sizeof(std::byte) + buffer_size_one(optional.value());
        }
        else {
            return sizeof(std::byte);
        }
    }

    template <class T> requires (concepts::tuple<T>) && (!concepts::container<T>)
    [[nodiscard]] static constexpr inline auto buffer_size_one(T const& tuple) -> std::size_t {    
        return std::apply(
            [](auto&&... elements) constexpr {
                return buffer_size_many(elements...);
            },
            tuple
        );
    }

    template <class T> requires (concepts::variant<T>)
    [[nodiscard]] static constexpr inline auto buffer_size_one(T const& variant) -> std::size_t {
        std::size_t const index_size = sizeof(variant.index());
        std::size_t const element_size = std::visit(
            [](auto&& value) constexpr {
                return buffer_size_one(value);
            },
            variant
        );
        return index_size + element_size;
    }

    // Serialization implementation.

    template <class T0, class... TN>
    [[nodiscard]] constexpr inline auto serialize_many(T0 const& first_obj, TN const&... remaining_objs) -> error {        
        if (auto result = serialize_one(first_obj); failure(result)) [[unlikely]] {
            return result;
        }

        return serialize_many(remaining_objs...);
    }

    [[nodiscard]] constexpr inline auto serialize_many() -> error {
        return {};
    }

    template <class T>
        requires (concepts::trivially_serializable<T>)
    [[nodiscard]] constexpr inline auto serialize_one(T const& obj) -> error {
        // Trivial serialization.
        if (std::is_constant_evaluated()) {
            // At compile-time.
            using src_array_type = std::array<byte_type, sizeof(obj)>;
            
            auto src_array = std::bit_cast<src_array_type>(obj);
            for (auto& src_byte : src_array) {
                m_buffer[m_position++] = src_byte;
            }

            return {};
        }
        else {
            // At run-time.
            auto* dst = m_buffer.data() + m_position;
            auto* const src = reinterpret_cast<byte_type const*>(&obj);
            auto const size = sizeof(obj);

            std::memcpy(dst, src, size);
            m_position += size;
            return {};
        }
    }

    template <class T>
        requires (concepts::aggregate<T>)
              && (!concepts::container<T>)
              && (!concepts::bounded_c_array<T>) 
              && (!concepts::trivially_serializable<T>)
    [[nodiscard]] constexpr inline auto serialize_one(T const& aggregate) -> error {
        return visit_members(
            [&](auto&&... members) {
                return serialize_many(members...);
            },
            aggregate
        );
    }

    template <class T> requires (concepts::bounded_c_array<T>) && (!concepts::trivially_serializable<T>)
    [[nodiscard]] constexpr inline auto serialize_one(T const& c_array) -> error {
        // Serialize elements.            
        for (auto const& element : c_array) {
            if (auto result = serialize_one(element); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }

    template <class T> requires (concepts::container<std::remove_cvref_t<T>>) && (!concepts::trivially_serializable<T>) 
    [[nodiscard]] constexpr inline auto serialize_one(T const& container) -> error {
        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;
        using size_type = typename container_type::size_type;
        
        // Serialize the size if needed.
        if constexpr (!concepts::fixed_size_container<container_type>) {
            // If the container its fixed, the size is known at compile-time;
            // thus, serialization is unnecessary. Otherwise, serialize its size.
            if constexpr (concepts::sized_container<container_type>) {
                // Optimization: we don't need to compute the size; it is already known at run-time.
                if (auto result = serialize_one(container.size()); failure(result)) [[unlikely]] {
                    return result;
                }
            }
            else {
                // Non-optimized path: some containers (like `std::forward_list`) don't have a `size()` observer.
                size_type const size = std::distance(container.begin(), container.end());
                if (auto result = serialize_one(size); failure(result)) [[unlikely]] {
                    return result;
                }
            }
        }

        // Serialize the elements.
        if constexpr (
            concepts::contiguous_container<container_type> &&
            concepts::trivially_serializable<element_type>
        ) {
            // Optimized path: memory copy elements.
            auto* dst = m_buffer.data() + m_position;
            auto const* src = reinterpret_cast<byte_type const*>(container.data());
            auto const size = container.size() * sizeof(element_type);
            
            std::memcpy(dst, src, size);
            m_position += size;
            return {};   
        }
        else {
            // Non-optimized path: serialize each element one by one.
            for (auto const& element : container) {
                if (auto result = serialize_one(element); failure(result)) [[unlikely]] {
                    return result;
                }
            }
            return {};
        }
    }

    template <class T> requires (concepts::owning_ptr<T>)
    [[nodiscard]] constexpr inline auto serialize_one(T const& ptr) -> error  {
        if (ptr == nullptr) [[unlikely]] {
            return std::errc::invalid_argument;
        }

        return serialize_one(*ptr);
    }

    template <class T> requires (concepts::optional<T>)
    [[nodiscard]] constexpr inline auto serialize_one(T const& optional) -> error {
        if (!optional.has_value()) [[unlikely]] {
            return serialize_one(byte_type(false));
        } else {
            return serialize_many(byte_type(true), *optional);
        }
    }

    template <class T> 
        requires (concepts::tuple<T>) 
              && (!concepts::container<T>)
              && (!concepts::trivially_serializable<T>)
    [[nodiscard]] constexpr inline auto serialize_one(T const& tuple) -> error {    
        return std::apply(
            [&](auto&&... elements) constexpr {
                return serialize_many(elements...);
            },
            tuple
        );
    }

    template <class T> requires (concepts::variant<T>)
    [[nodiscard]] constexpr inline auto serialize_one(T const& variant) -> error {
        // Check valueless by exception.
        if (variant.valueless_by_exception()) [[unlikely]] {
            return error { std::errc::invalid_argument };
        }

        // Serialize the index.
        if (auto result = serialize_one(variant.index()); failure(result)) [[unlikely]] {
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

private:
    buffer_type m_buffer = {};
    std::size_t m_position = 0;
};

template <class B, class... T>
    requires (concepts::byte_buffer<B>) && (concepts::serializable<T> && ...)
constexpr inline auto serialize(B & output_buffer, T const&... objs) -> error {
    return binary_serializer<B>{output_buffer}.serialize(objs...);
}

} // namespace teg

#endif // TEG_SERIALIZATION_H 