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
#include <compare>
#include "concepts.h"

namespace teg {

template<character C, std::size_t S, typename TT = std::char_traits<C>>
class basic_fixed_string {
public:
    // Member types.
    using type_traits            = TT;
    using value_type             = C;
    using storage_type           = std::array<C, S+1>;
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
    using string_view_type       = std::basic_string_view<C, TT>;

    static constexpr auto npos = string_view_type::npos;

    // Constructors, copy, move, and destructor.
    constexpr basic_fixed_string() noexcept = default;

    constexpr basic_fixed_string(const value_type (&str)[S+1]) noexcept {
        std::copy(std::begin(str), std::end(str), std::begin(m_data));
    }

    constexpr basic_fixed_string(std::basic_string_view<value_type, type_traits> view) noexcept {
        std::copy(std::begin(view), std::end(view), std::begin(m_data));
        m_data[S] = '\0';
    }
 
    constexpr basic_fixed_string& operator=(const value_type (&str)[S+1]) noexcept {
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
    [[nodiscard]] constexpr size_type lenght() const noexcept { return S; }
    [[nodiscard]] constexpr size_type size() const noexcept { return S; }
    [[nodiscard]] constexpr size_type max_size() const noexcept { return S; }
    [[nodiscard]] constexpr bool empty() const noexcept { return S == 0; }

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
    [[nodiscard]] constexpr reference back() noexcept { return m_data[S-1]; }
    [[nodiscard]] constexpr const_reference back() const noexcept { return m_data[S-1]; }

    [[nodiscard]] constexpr operator string_view_type() const noexcept { return {data(), S}; }

    storage_type m_data;
};

template <typename C, std::size_t S1, std::size_t S2, typename TT>
[[nodiscard]] constexpr bool operator==(basic_fixed_string<C, S1, TT> const& lhs, basic_fixed_string<C, S2, TT> const& rhs) noexcept
{
    if constexpr (S1 != S2) {
        return false;
    }
    else {
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
    }
}

template <typename C, std::size_t S, typename TT>
[[nodiscard]] constexpr bool operator==(basic_fixed_string<C, S, TT> const& lhs, std::basic_string_view<C, TT> rhs) noexcept
{
    using string_view_type = typename basic_fixed_string<C, S, TT>::string_view_type;
    return static_cast<string_view_type>(lhs) == rhs;
}

template <typename C, std::size_t S, typename TT>
[[nodiscard]] constexpr bool operator==(std::basic_string_view<C, TT> lhs, basic_fixed_string<C, S, TT> const& rhs) noexcept
{
    using string_view_type = typename basic_fixed_string<C, S, TT>::string_view_type;
    return lhs == static_cast<string_view_type>(rhs);
}

template <typename C, std::size_t S1, std::size_t S2, typename TT>
[[nodiscard]] constexpr bool operator==(basic_fixed_string<C, S1, TT> const& lhs, const C (&rhs)[S2]) noexcept
{
    if constexpr (S1 != S2-1) {
        return false;
    }
    else {
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
    }
}

template <typename C, std::size_t S1, std::size_t S2, typename TT>
[[nodiscard]] constexpr auto operator<=>(const basic_fixed_string<C, S1, TT>& lhs, const basic_fixed_string<C, S2, TT>& rhs) noexcept
{
    return std::lexicographical_compare_three_way(
        std::begin(lhs), std::end(lhs), 
        std::begin(rhs), std::end(rhs));
}

template <typename C, std::size_t S1, std::size_t S2, typename TT>
constexpr basic_fixed_string<C, S1 + S2, TT> operator+(const basic_fixed_string<C, S1, TT>& lhs, const basic_fixed_string<C, S2, TT>& rhs) noexcept
{
    basic_fixed_string<C, S1 + S2, TT> result;
    std::copy(std::begin(lhs), std::end(lhs), std::begin(result));
    std::copy(std::begin(rhs), std::end(rhs), std::begin(result) + S1);

    return result;
}

template <typename C, std::size_t S1, std::size_t S2, typename TT>
constexpr basic_fixed_string<C, S1 - 1 + S2, TT> operator+(const C (&lhs)[S1], const basic_fixed_string<C, S2, TT>& rhs) noexcept
{
    basic_fixed_string<C, S1 - 1 + S2, TT> result;
    std::copy(std::begin(lhs), std::end(lhs), std::begin(result));
    std::copy(std::begin(rhs), std::end(rhs), std::begin(result) + S1 - 1);

    return result;
}

template <typename C, std::size_t S1, std::size_t S2, typename TT>
constexpr basic_fixed_string<C, S1 + S2 - 1, TT> operator+(const basic_fixed_string<C, S1, TT>& lhs, const C (&rhs)[S2]) noexcept
{
    basic_fixed_string<C, S1 + S2 - 1, TT> result;
    std::copy(std::begin(lhs), std::end(lhs), std::begin(result));
    std::copy(std::begin(rhs), std::end(rhs), std::begin(result) + S1);

    return result;
}

// Deduction guide
template <typename C, std::size_t S>
basic_fixed_string(const C(&)[S]) -> basic_fixed_string<C, S-1>;

template <std::size_t S>
class fixed_string : public basic_fixed_string<char, S> {
    public:
    using basic_fixed_string<char, S>::basic_fixed_string;
};

template <std::size_t S>
fixed_string(const char (&)[S]) -> fixed_string<S-1>;

template <std::size_t S>
class fixed_wstring : public basic_fixed_string<wchar_t, S> {
    public:
    using basic_fixed_string<wchar_t, S>::basic_fixed_string;
};

template <std::size_t S>
fixed_wstring(const wchar_t (&)[S]) -> fixed_wstring<S-1>;

template <std::size_t S>
class fixed_u8string : public basic_fixed_string<char8_t, S> {
    public:
    using basic_fixed_string<char8_t, S>::basic_fixed_string;
};

template <std::size_t S>
fixed_u8string(const char8_t (&)[S]) -> fixed_u8string<S-1>;

template <std::size_t S>
class fixed_u16string : public basic_fixed_string<char16_t, S> {
    public:
    using basic_fixed_string<char16_t, S>::basic_fixed_string;
};

template <std::size_t S>
fixed_u16string(const char16_t (&)[S]) -> fixed_u16string<S-1>;

template <std::size_t S>
class fixed_u32string : public basic_fixed_string<char32_t, S> {
    public:
    using basic_fixed_string<char32_t, S>::basic_fixed_string;
};

template <std::size_t S>
fixed_u32string(const char32_t (&)[S]) -> fixed_u32string<S-1>;

} // namespace teg