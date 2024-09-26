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
#include <array>
#include <functional>
#include <iterator>
#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include "concepts.h"

namespace teg {

template<character CHAR_TYPE, std::size_t SIZE, typename TRAITS = std::char_traits<CHAR_TYPE>>
class basic_fixed_string {
public:
    // Member types.
    using type_traits            = TRAITS;
    using value_type             = CHAR_TYPE;
    using storage_type           = std::array<CHAR_TYPE, SIZE+1>;
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using iterator               = typename storage_type::iterator;
    using const_iterator         = typename storage_type::const_iterator;
    using reverse_iterator       = typename storage_type::reverse_iterator;
    using const_reverse_iterator = typename storage_type::const_reverse_iterator;
    using string_view_type       = std::basic_string_view<CHAR_TYPE, TRAITS>;

    static constexpr auto npos = string_view_type::npos;

    // Constructors, copy, move, and destructor.
    constexpr basic_fixed_string() noexcept = default;

    constexpr basic_fixed_string(const value_type (&str)[SIZE+1]) noexcept {
        std::copy(std::begin(str), std::end(str), std::begin(m_data));
    }

    constexpr basic_fixed_string& operator=(const value_type (&str)[SIZE+1]) noexcept {
        std::copy(std::begin(str), std::end(str), std::begin(m_data));
        return *this;
    }

    // Iterators.
    [[nodiscard]] constexpr iterator begin() noexcept { return m_data.begin(); }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return m_data.begin(); }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return m_data.cbegin(); }
    [[nodiscard]] constexpr iterator end() noexcept { return m_data.end() - 1; }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return m_data.end() - 1; }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return m_data.cend() - 1; }
    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return m_data.rbegin() + 1; }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return m_data.rbegin() + 1; }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return m_data.crbegin() + 1; }
    [[nodiscard]] constexpr reverse_iterator rend() noexcept { return m_data.rend(); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return m_data.rend(); }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return m_data.rend(); }

    // Capacity.
    [[nodiscard]] constexpr size_type lenght() const noexcept { return SIZE; }
    [[nodiscard]] constexpr size_type size() const noexcept { return SIZE; }
    [[nodiscard]] constexpr size_type max_size() const noexcept { return SIZE; }
    [[nodiscard]] constexpr bool empty() const noexcept { return SIZE == 0; }

    // Element access.
    [[nodiscard]] constexpr pointer data() noexcept { return m_data.data(); }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return m_data.data(); }
    [[nodiscard]] constexpr const_pointer c_str() const noexcept { return m_data.data(); }

    [[nodiscard]] constexpr reference operator[](size_type pos) noexcept { return m_data[pos]; }
    [[nodiscard]] constexpr const_reference operator[](size_type pos) const noexcept { return m_data[pos]; }
    [[nodiscard]] constexpr reference at(size_type pos) noexcept { return m_data.at(pos); }
    [[nodiscard]] constexpr const_reference at(size_type pos)  const noexcept { return m_data.at(pos); }
    [[nodiscard]] constexpr reference front() noexcept { return m_data[0]; }
    [[nodiscard]] constexpr const_reference front() const noexcept { return m_data[0]; }
    [[nodiscard]] constexpr reference back() noexcept { return m_data[SIZE-1]; }
    [[nodiscard]] constexpr const_reference back() const noexcept { return m_data[SIZE-1]; }

    storage_type m_data;
};

template <std::size_t SIZE> using fixed_string = basic_fixed_string<char, SIZE>;
template <std::size_t SIZE> using fixed_wstring = basic_fixed_string<wchar_t, SIZE>;
template <std::size_t SIZE> using fixed_u8string  = basic_fixed_string<char8_t, SIZE>;
template <std::size_t SIZE> using fixed_u16string = basic_fixed_string<char16_t, SIZE>;
template <std::size_t SIZE> using fixed_u32string = basic_fixed_string<char32_t, SIZE>;

} // namespace teg