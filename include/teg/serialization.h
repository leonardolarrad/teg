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
error serialize_one(buffer_writer& writer, const auto& obj) {
    using type = std::remove_cvref_t<decltype(obj)>;

    if constexpr (std::is_fundamental_v<type> || std::is_enum_v<type>) {
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

[[nodiscard]] inline constexpr 
error serialize(buffer& output_buffer, const auto&... objs) {
    if constexpr (sizeof...(objs) == 0) {
        return {};
    }

    auto curr_size = output_buffer.size();
    auto objs_size = compute_serialized_size(objs...);
    output_buffer.resize(curr_size + objs_size);

    auto writer = internal::buffer_writer{ output_buffer };
    return internal::serialize_many(writer, objs...);
}

} // namespace teg