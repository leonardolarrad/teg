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

#include "buffer.h"
#include "error.h"
#include "concepts.h"
#include "index_table.h"

#include <iostream>

namespace teg::internal {

class buffer_reader {
public:
    buffer_reader(buffer& buffer, std::size_t position = 0) 
        : m_buffer(buffer), m_position(position) {}

    constexpr error read_bytes(std::byte* data, std::size_t size) {
        if (m_position + size > m_buffer.size()) [[unlikely]] {
            return error { std::errc::no_buffer_space };
        }

        std::memcpy(data, m_buffer.data() + m_position, size);
        m_position += size;
        return {};
    }

private:
    buffer& m_buffer;
    std::size_t m_position;
};

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, auto& obj) {
    using type = std::remove_cvref_t<decltype(obj)>;

    if constexpr (trivially_copyable<type>) {  
        std::byte* data = reinterpret_cast<std::byte*>(&obj);
        std::size_t size = sizeof(type);

        return reader.read_bytes(data, size);
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
error deserialize_one(buffer_reader& reader, optional auto& optional) {
    using type = std::remove_reference_t<decltype(optional)>;
    using value_type = typename type::value_type;
    
    // Deserialize `has_value` flag.
    std::byte has_value;    
    if (auto result = deserialize_one(reader, has_value); failure(result)) [[unlikely]] {
        return result;
    }

    // Check if there is a value to deserialize.
    if (!bool(has_value)) {
        optional = std::nullopt;
        return {};
    }

    // Deserialize value.
    if (!optional.has_value()) {
        optional = value_type{};
    }

    if (auto result = deserialize_one(reader, *optional); failure(result)) [[unlikely]] {
        return result;
    }
    return {};
}

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, associative_container auto& container) {
    using type = std::remove_reference_t<decltype(container)>;
    using size_type = typename type::size_type;
    
    // Pre-condition: the container is empty.
    // In C++17 and later, aggregates can have user-provided constructors (member initializers),
    // allowing default-constructed non-empty containers.
    container.clear();
    
    // Deserialize size.
    size_type size;
    auto result = deserialize_one(reader, size);
    if (failure(result)) [[unlikely]] {
        return result;
    }

    // Deserialize elements.
    if constexpr (reservable_container<type>) {
        // Optimization: allocate uninitialized memory in advance.
        container.reserve(size);
    }

    if constexpr (map_container<type>) {
        // By default `std::map` uses `std::pair<const key_type, mapped_type>`
        // as its value_type. We can not deserialize const-qualified types.
        // Get rid of the const qualifier.
        using value_type = std::pair<typename type::key_type, typename type::mapped_type>;
        value_type element;

        for (size_type i = 0; i < size; ++i) {
            auto result = deserialize_one(reader, element);
            if (failure(result)) [[unlikely]] {
                return result;
            }
            container.emplace(std::move(element));
        }
        return {};
    }
    else {
        using value_type = typename type::value_type;
        value_type element;

        for (size_type i = 0; i < size; ++i) {
            auto result = deserialize_one(reader, element);
            if (failure(result)) [[unlikely]] {
                return result;
            }
            container.emplace(std::move(element));
        }
        return {};
    }
}

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, fixed_size_container auto& container) {
    using type = std::remove_cvref_t<decltype(container)>;
    using value_type = typename type::value_type;
    
    // The size is known at compile time; therefore we don't need to deserialize it.
    // Deserialize only the elements.
    if constexpr (trivially_copyable<value_type>) {
        // Optimization: memory copy elements.
        std::byte* data = reinterpret_cast<std::byte*>(container.data());
        return reader.read_bytes(data, container.size() * sizeof(value_type));
    }
    else {
        // Non-optimized path.
        for (auto& element : container) {
            auto result = deserialize_one(reader, element);
            if (failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }
}

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, contiguous_container auto& container) {
    using type = std::remove_reference_t<decltype(container)>;
    using value_type = typename type::value_type;
    using size_type = typename type::size_type;

    if constexpr (clearable_container<type>) {
        // Pre-condition: the container is empty.
        // In C++17 and later, aggregates can have user-provided constructors (member initializers),
        // allowing default-constructed non-empty containers.
        container.clear();
    }

    // Deserialize size.
    size_type size;
    auto result = deserialize_one(reader, size);
    if (failure(result)) [[unlikely]] {
        return result;
    }

    // Deserialize elements.
    if constexpr (resizable_container<type> && trivially_copyable<value_type>) {
        // Optimization: memory copy elements.
        container.resize(size);

        std::byte* data = reinterpret_cast<std::byte*>(container.data());
        return reader.read_bytes(data, size * sizeof(value_type));
    }
    else {
        if constexpr (reservable_container<type>) {
            // Optimization: allocate uninitialized memory in advance.
            container.reserve(size);
        }

        // Non-optimized path.
        if constexpr (back_inplace_constructing_container<type>) {
            for (size_type i = 0; i < size; ++i) {
                auto result = deserialize_one(reader, container.emplace_back());
                if (failure(result)) [[unlikely]] {
                    return result;
                }
            }
            return {};
        }
        else if constexpr (front_inplace_constructing_container<type>) {
            for (size_type i = 0; i < size; ++i) {
                auto result = deserialize_one(reader, container.emplace_front());
                if (failure(result)) [[unlikely]] {
                    return result;
                }
            }
            if constexpr (invertible_container<type>) {
                container.reverse();
            }
            return {};
        }
        else {
            return error { std::errc::not_supported };
        }
    }
}

[[nodiscard]] inline constexpr
error deserialize_one(buffer_reader& reader, container auto& container) {
    using type = std::remove_cvref_t<decltype(container)>;
    using value_type = typename type::value_type;
    using size_type = typename type::size_type;

    if constexpr (clearable_container<type>) {
        // Pre-condition: the container is empty.
        // In C++17 and later, aggregates can have user-provided constructors (member initializers),
        // allowing default-constructed non-empty containers.
        container.clear();
    }

    // Deserialize size.
    size_type size;
    auto result = deserialize_one(reader, size);
    if (failure(result)) [[unlikely]] {
        return result;
    }

    // Deserialize elements.
    if constexpr (reservable_container<type>) {
        // Optimization: allocate uninitialized memory in advance.
        container.reserve(size);
    }

    if constexpr (back_inplace_constructing_container<type>) {
        for (size_type i = 0; i < size; ++i) {
            auto result = deserialize_one(reader, container.emplace_back());
            if (failure(result)) [[unlikely]] {
                return result;
            }
        }
        return {};
    }
    else if constexpr (front_inplace_constructing_container<type>) {
        for (size_type i = 0; i < size; ++i) {
            auto result = deserialize_one(reader, container.emplace_front());
            if (failure(result)) [[unlikely]] {
                return result;
            }
        }
        if constexpr (invertible_container<type>) {
            container.reverse();
        }
        return {};
    }
    else if constexpr (inplace_constructing_container<type>) {
        value_type element;

        for (size_type i = 0; i < size; ++i) {
            auto result = deserialize_one(reader, element);
            if (failure(result)) [[unlikely]] {
                return result;
            }
            container.emplace(std::move(element));
        }
        return {};
    }
    else {
        return error { std::errc::not_supported };
    }
}

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, owning_pointer auto& pointer) {
    using type = ref_unqualified<decltype(pointer)>;
    using element_type = typename type::element_type;

    // Deserialize element.
    auto data = std::make_unique<element_type>();

    auto result = deserialize_one(reader, *data);
    if (failure(result)) [[unlikely]] {
        return result;
    }

    // Transfer ownership.
    pointer.reset(data.release());
    return {};
}

[[nodiscard]] inline constexpr 
error deserialize_one(buffer_reader& reader, variant auto& variant) {
    using comptime_type = ref_unqualified<decltype(variant)>;

    // Deserialize index.
    std::size_t index;
    auto result = deserialize_one(reader, index);
    if (failure(result)) [[unlikely]] {
        return result;
    }

    // Deserialize element.
    constexpr std::size_t table_size = std::variant_size_v<comptime_type>;

    if (index >= table_size) {
        return error { std::errc::invalid_argument };
    }
    
    return index_table_lookup<table_size>(index, [&](auto comptime_index) {
        /// Using an index table we transform a runtime index into 
        /// a compile-time index. With this technique we can then deserialize
        /// the variant alternative (based on the index) at runtime.
        std::variant_alternative_t<comptime_index, comptime_type> element;        
        if (result = deserialize_one(reader, element); failure(result)) [[unlikely]] {
            return result;
        }

        variant.template emplace<comptime_index>(std::move(element));
        return teg::error {};
    });
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