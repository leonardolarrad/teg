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
#include <iterator>
#include <cstddef>
#include <cstring>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <compare>

#include "concepts.h"

namespace teg {

template<character C, std::size_t N, typename TT = std::char_traits<C>>
class basic_fixed_string {
public:
    // Member types.
    using type_traits = TT;
    using value_type = C;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = value_type*;
    using const_iterator = const value_type*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using string_view_type = std::basic_string_view<value_type, type_traits>;

    // Construct/copy/move.
    constexpr basic_fixed_string() noexcept = default;
    
    constexpr basic_fixed_string(const value_type (&str)[N+1]) noexcept {
        std::copy(std::begin(str), std::end(str), std::begin(m_data));
    }

    constexpr basic_fixed_string(std::basic_string_view<value_type, type_traits> view) noexcept {
        std::copy(std::begin(view), std::end(view), std::begin(m_data));
        m_data[N] = '\0';
    }

    constexpr basic_fixed_string& operator=(const value_type (&str)[N+1]) noexcept {
        std::copy(std::begin(str), std::end(str), std::begin(m_data));
        return *this;
    }

    // Element access.
    [[nodiscard]] constexpr pointer data() noexcept { return m_data; }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return m_data; }
    [[nodiscard]] constexpr const_pointer c_str() const noexcept { return m_data; }

    [[nodiscard]] constexpr reference operator[](size_type i) requires(N > 0) { return m_data[i]; }
    [[nodiscard]] constexpr const_reference operator[](size_type i) const requires(N > 0) { return m_data[i]; }
    [[nodiscard]] constexpr reference front() requires(N > 0) { return m_data[0]; }
    [[nodiscard]] constexpr const_reference front() const requires(N > 0) { return m_data[0]; }
    [[nodiscard]] constexpr reference back() requires(N > 0) { return m_data[N-1]; }
    [[nodiscard]] constexpr const_reference back() const requires(N > 0) { return m_data[N-1]; }

    [[nodiscard]] constexpr reference at(size_type i) requires(N > 0) { 
        if (i >= N) {
            throw std::out_of_range("Index out of range.");
        }
        return m_data[i]; 
    }

    [[nodiscard]] constexpr const_reference at(size_type i) const requires(N > 0) { 
        if (i >= N) {
            throw std::out_of_range("Index out of range.");
        }
        return m_data[i]; 
    }

    // Iterators.
    [[nodiscard]] constexpr iterator begin() noexcept { return m_data; }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return m_data; }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return m_data; }
    [[nodiscard]] constexpr iterator end() noexcept { return m_data + N; }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return m_data + N; }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return m_data + N; }
    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(cend()); }
    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
    [[nodiscard]] constexpr reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept { return const_reverse_iterator(cbegin()); }
    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

    // Capacity.
    [[nodiscard]] constexpr size_type length() const noexcept { return N; }
    [[nodiscard]] constexpr size_type size() const noexcept { return N; }
    [[nodiscard]] constexpr size_type max_size() const noexcept { return N; }
    [[nodiscard]] constexpr bool empty() const noexcept { return N == 0; }

    // Conversions.
    [[nodiscard]] constexpr operator string_view_type() const noexcept { return { m_data, N }; }

    // Storage.
    value_type m_data[N+1] = {};
};

template <typename C, std::size_t N1, std::size_t N2, typename TT>
[[nodiscard]] constexpr bool operator==(basic_fixed_string<C, N1, TT> const& l, basic_fixed_string<C, N2, TT> const& r) noexcept
{
    using left_view_type = typename basic_fixed_string<C, N1, TT>::string_view_type;
    using right_view_type = typename basic_fixed_string<C, N2, TT>::string_view_type;

    return static_cast<left_view_type>(l) == static_cast<right_view_type>(r);
}

template <typename C, std::size_t N1, std::size_t N2, typename TT>
[[nodiscard]] constexpr auto operator<=>(const basic_fixed_string<C, N1, TT>& l, const basic_fixed_string<C, N2, TT>& r) noexcept
{
    using left_view_type = typename basic_fixed_string<C, N1, TT>::string_view_type;
    using right_view_type = typename basic_fixed_string<C, N2, TT>::string_view_type;

    return static_cast<left_view_type>(l) <=> static_cast<right_view_type>(r);
}

template <typename C, std::size_t N1, std::size_t N2, typename TT>
[[nodiscard]] constexpr bool operator==(basic_fixed_string<C, N1, TT> const& lhs, const C (&rhs)[N2]) noexcept
{
    using view_type = typename basic_fixed_string<C, N1, TT>::string_view_type;
    return static_cast<view_type>(lhs) == rhs;
}

template <typename C, std::size_t N1, std::size_t N2, typename TT>
[[nodiscard]] constexpr bool operator==(const C (&lhs)[N1], basic_fixed_string<C, N2, TT> const& rhs) noexcept
{
    using view_type = typename basic_fixed_string<C, N2, TT>::string_view_type;
    return lhs == static_cast<view_type>(rhs);
}

template <typename C, std::size_t N1, std::size_t N2, typename TT>
constexpr basic_fixed_string<C, N1 + N2, TT> operator+(basic_fixed_string<C, N1, TT> const& lhs, basic_fixed_string<C, N2, TT> const& rhs) noexcept
{
    basic_fixed_string<C, N1 + N2, TT> result;
    std::copy(std::begin(lhs), std::end(lhs), std::begin(result));
    std::copy(std::begin(rhs), std::end(rhs), std::begin(result) + N1);
    return result;
}

template <typename C, std::size_t N1, std::size_t N2, typename TT>
constexpr basic_fixed_string<C, N1 - 1 + N2, TT> operator+(const C (&l)[N1], const basic_fixed_string<C, N2, TT>& r) noexcept
{    
    return basic_fixed_string<C, N1, TT>(l) + r;
}

template <typename C, std::size_t N1, std::size_t N2, typename TT>
constexpr basic_fixed_string<C, N1 + N2 - 1, TT> operator+(const basic_fixed_string<C, N1, TT>& l, const C (&r)[N2]) noexcept
{
    return l + basic_fixed_string<C, N2, TT>(r);
}

// Deduction guide.
template <typename C, std::size_t N>
basic_fixed_string(const C(&)[N]) -> basic_fixed_string<C, N-1>;

// Aliases.
template <std::size_t N>
using fixed_string = basic_fixed_string<char, N>;

template <std::size_t N>
using fixed_u8string = basic_fixed_string<char8_t, N>;

template <std::size_t N>
using fixed_u16string = basic_fixed_string<char16_t, N>;

template <std::size_t N>
using fixed_u32string = basic_fixed_string<char32_t, N>;

template <std::size_t N>
using fixed_wstring = basic_fixed_string<wchar_t, N>;

} // namespace teg