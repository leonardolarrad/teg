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

#include "buffer.h"
#include "error.h"
#include "concepts.h"
#include <iostream>

namespace teg::internal {

class buffer_reader {
public:
    buffer_reader(buffer& buffer, std::size_t position = 0) 
        : m_buffer(buffer), m_position(position) {}

    constexpr void read_bytes(std::byte* data, std::size_t size) {
        if (m_position + size > m_buffer.size()) [[unlikely]] {
            return;
        }

        std::memcpy(data, m_buffer.data() + m_position, size);
        m_position += size;
    }

private:
    buffer& m_buffer;
    std::size_t m_position;
};

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, auto& obj) {
    using type = std::remove_cvref_t<decltype(obj)>;

    if constexpr (fundamental<type>) {        
        reader.read_bytes(reinterpret_cast<std::byte*>(&obj), sizeof(type));
        return {};
    }
    else {
        return visit_members(
            obj,
            [&reader](auto&&... objs) constexpr {
                return deserialize_many(reader, objs...);
            }
        );
    }
}

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, contiguous_container auto& container) {
    using type = std::remove_cvref_t<decltype(container)>;
    using value_type = typename type::value_type;
    using size_type = typename type::size_type;

    // Deserialize size.
    size_type size; 
    if (auto result = deserialize_one(reader, size); failure(result)) [[unlikely]] {
        return result;
    }

    // Deserialize elements.
    if constexpr (resizable_container<type> && trivially_copyable<value_type>) {
        // Optimization: memory copy trivially copyable elements.
        container.resize(size);

        std::byte* data = reinterpret_cast<std::byte*>(container.data());
        reader.read_bytes(data, size * sizeof(value_type));
        return {};
    } 
    else {
        if constexpr (reservable_container<type>) {
            // Optimization: allocate uninitialized memory in advance.
            container.reserve(size);
        }

        // Non-optimized path.
        if constexpr (back_inplace_constructing_container<type>) {
            for (size_type i = 0; i < size; ++i) {
                if (auto result = deserialize_one(reader, container.emplace_back()); failure(result)) [[unlikely]] {
                    return result;
                }
            }
            return {};
        }
        else if constexpr (front_inplace_constructing_container<type>) {
            for (size_type i = 0; i < size; ++i) {
                if (auto result = deserialize_one(reader, container.emplace_front()); failure(result)) [[unlikely]] {
                    return result;
                }
            }
            return {};
        }
        else {
            return error { std::errc::not_supported };
        }
    }
}


[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, fixed_size_container auto& container) {
    // The size is known at compile time; therefore, we don't need to deserialize it.
    // Deserialize only the elements.
    for (auto& elem : container) {
        if (auto result = deserialize_one(reader, elem); failure(result)) [[unlikely]] {
            return result;
        }
    }
    return {};
}

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, container auto& container) {
    using type = std::remove_cvref_t<decltype(container)>;
    using size_type = typename type::size_type;

    // Deserialize size.
    size_type size;
    if (auto result = deserialize_one(reader, size); failure(result)) [[unlikely]] {
        return result;
    }

    // Deserialize elements.
    if constexpr (back_inplace_constructing_container<type>) {
        for (size_type i = 0; i < size; ++i) {
            if (auto result = deserialize_one(reader, container.emplace_back()); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }
    else if constexpr (front_inplace_constructing_container<type>) {
        for (size_type i = 0; i < size; ++i) {
            if (auto result = deserialize_one(reader, container.emplace_front()); failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }
    else {
        return error { std::errc::not_supported };
    }
}

[[nodiscard]] inline constexpr 
error deserialize_many(buffer_reader& reader) {
    return {};
}

[[nodiscard]] inline constexpr 
error deserialize_many(buffer_reader& reader, auto& first_obj, auto&... remaining_objs) {
    auto result = deserialize_one(reader, first_obj);
    if (failure(result)) [[unlikely]] {
        return result;
    }

    return deserialize_many(reader, remaining_objs...);
}

} // namespace teg::internal

namespace teg {

[[nodiscard]] inline 
error deserialize(buffer& input_buffer, auto&... objs) {
    if constexpr (sizeof...(objs) == 0) {
        return {};
    }

    auto reader = internal::buffer_reader{ input_buffer };
    return internal::deserialize_many(reader, objs...);
}

} // namespace teg