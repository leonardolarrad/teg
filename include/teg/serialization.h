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

#pragma once
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <cstring>
#include <tuple>
#include <variant>
#include <vector>

#include "buffer.h"
#include "concepts.h"
#include "error.h"
#include "members_count.h"
#include "visitor.h"

namespace teg::internal {

class buffer_writer {
public:
    buffer_writer(buffer& buffer, std::size_t position = 0) 
        : m_buffer(buffer), m_position(position) {}

    inline error write_bytes(std::byte const* data, std::size_t size) {
        std::memcpy(m_buffer.data() + m_position, data, size);
        m_position += size;
        return {};
    }

private:
    buffer& m_buffer;
    std::size_t m_position;
};


[[nodiscard]] constexpr inline 
error serialize_one(buffer_writer& writer, auto const& obj) {
    using type = std::remove_cvref_t<decltype(obj)>;

    if constexpr (trivially_copyable<type>) {
        std::byte const* data = reinterpret_cast<std::byte const*>(&obj);
        std::size_t size = sizeof(type);

        return writer.write_bytes(data, size);       
    }
    else {
        return visit_members(
            obj, 
            [&writer](auto&&... objs) constexpr {
                return serialize_many(writer, objs...);
            }
        );
    }
}

[[nodiscard]] constexpr inline 
error serialize_one(buffer_writer& writer, optional auto const& optional)  {
    if (!optional.has_value()) [[unlikely]] {
        return serialize_one(writer, std::byte(false));
    } else {
        return serialize_many(writer, std::byte(true), *optional);
    }
}

template <class T> requires tuple<T> && (!container<T>)
[[nodiscard]] constexpr inline 
error serialize_one(buffer_writer& writer, T const& tuple) {    
    return std::apply(
        [&writer](auto&&... elements) constexpr {
            return serialize_many(writer, elements...);
        },
        tuple
    );
}

[[nodiscard]] constexpr inline 
error serialize_one(buffer_writer& writer, variant auto const& variant)  {
    // Check valueless by exception.
    if (variant.valueless_by_exception()) [[unlikely]] {
        return error { std::errc::invalid_argument };
    }

    // Serialize index.
    if (auto result = serialize_one(writer, variant.index()); failure(result)) [[unlikely]] {
        return result;        
    }    

    // Serialize value.
    return std::visit(
        [&writer](auto&& value) constexpr {
            return serialize_one(writer, value);
        },
        variant
    );
}

[[nodiscard]] constexpr inline 
error serialize_one(buffer_writer& writer, container auto const& container) {
    using type = std::remove_cvref_t<decltype(container)>;
    using size_type = typename type::size_type;
    
    // Serialize size.
    if constexpr (sized_container<type>) {
        // Optimization: don't need to compute the size; it is already known at runtime.
        if (auto result = serialize_one(writer, container.size()); failure(result)) [[unlikely]] {
            return result;
        }
    }
    else {
        // Non-optimized path.
        // Some containers (like `std::forward_list`) don't have `size()` observer.
        size_type size = std::distance(container.begin(), container.end());
        if (auto result = serialize_one(writer, size); failure(result)) [[unlikely]] {
            return result;
        }
    }

    // Serialize elements.
    for (auto const& element : container) {
        if (auto result = serialize_one(writer, element); failure(result)) [[unlikely]] {
            return result;
        }
    }
    return {};
}

[[nodiscard]] constexpr inline 
error serialize_one(buffer_writer& writer, contiguous_container auto const& container) {
    using type = std::remove_cvref_t<decltype(container)>;
    using value_type = typename type::value_type;    
    
    // Serialize size.
    if constexpr (!fixed_size_container<type>) {
        // If the container has fixed size the size is known at compile time; 
        // therefore, we don't need to serialize it.
        // Otherwise, serialize it.
        if (auto result = serialize_one(writer, container.size()); failure(result)) [[unlikely]] {
            return result;
        }
    }

    // Serialize elements.
    if constexpr (trivially_copyable<value_type>) {
        // Optimization: memory copy elements.
        std::byte const* data = reinterpret_cast<const std::byte*>(container.data());
        std::size_t size = container.size();
        
        return writer.write_bytes(data, size * sizeof(value_type));
    }
    else {
        // Non-optimized path.
        for (auto const& element : container) {
            if (auto result = serialize_one(writer, element); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }
}

[[nodiscard]] constexpr inline 
error serialize_one(buffer_writer& writer, c_array auto const& array) {
    using type = std::remove_cvref_t<decltype(array)>;
    
    // Serialize elements.
    if constexpr (trivially_copyable<type>) {
        // Optimization: memory copy elements.
        std::byte const* data = reinterpret_cast<const std::byte*>(&array);
        std::size_t size = sizeof(array);
        
        return writer.write_bytes(data, size);
    }
    else {
        // Non-optimized path.
        for (auto const& element : array) {
            if (auto result = serialize_one(writer, element); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }
}

[[nodiscard]] constexpr inline 
error serialize_one(buffer_writer& writer, owning_pointer auto const& pointer)  {
    if (pointer == nullptr) [[unlikely]] {
        return std::errc::invalid_argument;
    }

    return serialize_one(writer, *pointer);
}

[[nodiscard]] constexpr inline 
error serialize_many(buffer_writer& writer) {
    return {};
}

[[nodiscard]] constexpr inline 
error serialize_many(buffer_writer& writer, const auto& first_obj, const auto&... remaining_objs) {
    auto result = serialize_one(writer, first_obj);
    if (failure(result)) [[unlikely]] {
        return result;
    }

    return serialize_many(writer, remaining_objs...);
}

} // namespace teg::internal

namespace teg {	
    
template <typename T>
concept serializable = fundamental<T> || aggregate<T> || true;

template <serializable... T>
[[nodiscard]] inline 
error serialize(buffer& output_buffer, const T&... objs) {
    if constexpr (sizeof...(objs) == 0) {
        return {};
    }

    auto curr_size = output_buffer.size();
    auto objs_size = buffer_size(objs...);
    output_buffer.resize(curr_size + objs_size);

    auto writer = internal::buffer_writer{ output_buffer };
    return internal::serialize_many(writer, objs...);
}

} // namespace teg