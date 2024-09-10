// Copyright (c) 2024 Adrian & Leonardo Larrad.
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include <vector>
#include <cstddef>
#include <concepts>
#include <algorithm>
#include <variant>
#include <algorithm>
#include <cstring>

#include "concepts.h"
#include "member_count.h"
#include "error.h"
#include "visitor.h"
#include "buffer.h"

namespace teg::internal {

class buffer_writer {
public:
    buffer_writer(buffer& buffer, std::size_t position = 0) 
        : m_buffer(buffer), m_position(position) {}

    inline void write_bytes(const std::byte* data, std::size_t size) {
        std::memcpy(m_buffer.data() + m_position, data, size);
        m_position += size;
    }

private:
    buffer& m_buffer;
    std::size_t m_position;
};

[[nodiscard]] inline constexpr 
error serialize_one(buffer_writer& writer, auto const& obj) {
    using type = std::remove_cvref_t<decltype(obj)>;

    if constexpr (fundamental<type>) {
        writer.write_bytes(reinterpret_cast<const std::byte*>(&obj), sizeof(type));
        return {};        
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

[[nodiscard]] inline constexpr 
error serialize_one(buffer_writer& writer, contiguous_container auto const& container) {
    using type = std::remove_cvref_t<decltype(container)>;
    using value_type = typename type::value_type;

    if (auto result = serialize_one(writer, container.size()); failure(result)) [[unlikely]] {
        return result;
    }    
    
    // Serialize elements.
    if constexpr (trivially_copyable<value_type>) {
        // Optimization: memory copy trivially copyable elements.
        const std::byte* data = reinterpret_cast<const std::byte*>(container.data());
        std::size_t size = container.size();

        writer.write_bytes(data, size * sizeof(value_type));
        return {};
    }
    else {
        // Non-optimized path.
        for (auto const& elem : container) {
            if (auto result = serialize_one(writer, elem); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }
}

[[nodiscard]] inline constexpr 
error serialize_one(buffer_writer& writer, fixed_size_container auto const& container) {
    using type = std::remove_cvref_t<decltype(container)>;
    using value_type = typename type::value_type;    
    
    // The size is known at compile time; therefore, we don't need to serialize it.    
    // Serialize only the elements.
    if constexpr (trivially_copyable<value_type>) {
        // Optimization: memory copy trivially copyable elements.
        const std::byte* data = reinterpret_cast<const std::byte*>(container.data());
        std::size_t size = container.size();

        writer.write_bytes(data, size * sizeof(value_type));
        return {};
    }
    else {
        // Non-optimized path.
        for (auto const& elem : container) {
            if (auto result = serialize_one(writer, elem); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }
}

[[nodiscard]] inline constexpr 
error serialize_one(buffer_writer& writer, container auto const& obj) {
    using type = std::remove_cvref_t<decltype(obj)>;
    using size_type = typename type::size_type;
    
    // Serialize size.
    if constexpr (sized_container<type>) {
        // Optimization: don't need to compute the size; it is already known at runtime.
        if (auto result = serialize_one(writer, obj.size()); failure(result)) [[unlikely]] {
            return result;
        }
    }
    else {
        // Non-optimized path.
        // Some containers (like `std::forward_list`) don't have `size()` observer.
        size_type size = std::distance(obj.begin(), obj.end());
        if (auto result = serialize_one(writer, size); failure(result)) [[unlikely]] {
            return result;
        }
    }

    // Serialize elements.
    for (auto const& elem : obj) {
        if (auto result = serialize_one(writer, elem); failure(result)) [[unlikely]] {
            return result;
        }
    }
    return {};
}

[[nodiscard]] inline constexpr 
error serialize_many(buffer_writer& writer) {
    return {};
}

[[nodiscard]] inline constexpr 
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