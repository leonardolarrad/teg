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

#ifndef TEG_MEMBERS_COUNT_H
#define TEG_MEMBERS_COUNT_H
#pragma once

#include <cstdint>
#include <type_traits>
#include <concepts>
#include <utility>
    
#include "concepts.h"

namespace teg::internal {

#if defined(_MSC_VER)

struct any { 
    template <class T> constexpr operator T&() const& noexcept;
    template <class T> constexpr operator T&&() const&& noexcept;
};

template <std::size_t I>
using indexed_any = any;

template <class T, class... A>
concept aggregate_initializable = aggregate<T> && requires { T{std::declval<A>()...}; };

template <class T, std::size_t N>
concept aggregate_initializable_with_n_args = aggregate<T>
    && []<std::size_t... I>(std::index_sequence<I...>) {
        return aggregate_initializable<T, indexed_any<I>...>;
    }(std::make_index_sequence<N>());    


template <aggregate T, std::size_t N = 0>
constexpr auto minimun_initialization() -> std::size_t {
    if constexpr (!aggregate_initializable_with_n_args<T, N>) {
        return minimun_initialization<T, N + 1>();
    } else {
        return N;
    }
}

template <aggregate T, std::size_t B, std::size_t E>
constexpr auto binary_search() -> std::size_t {
    using type = T;
    constexpr auto begin = B;
    constexpr auto end = E;
    constexpr auto middle = (begin + end) / 2;

    if constexpr (end - begin <= 1) {
        return begin;
    }
    else if constexpr (aggregate_initializable_with_n_args<type, middle>) {
        return binary_search<type, middle, end>();
    }
    else {
        return binary_search<type, begin, middle>();
    }
}

template <aggregate T>
constexpr auto members_count_impl() -> std::size_t {
    constexpr auto begin = minimun_initialization<T>();
    constexpr auto end = sizeof(T) * 8 + 1;

    return binary_search<T, begin, end>();
}

#else

struct any {
    template <class T> constexpr operator T() const noexcept;
};

template <typename T, typename... A>
concept aggregate_initializable = requires { T{ { std::declval<A>() }... }; };

template<typename T, typename ... A>
constexpr std::size_t forward_search() {
    if constexpr (aggregate_initializable<T, any, A...>) {
        return forward_search<T, A..., any>();
    }
    else {
        return sizeof...(A);
    }        
}

template <aggregate T>
constexpr auto members_count_impl() -> std::size_t {
    return forward_search<T>();
}

#endif // _MSC_VER

} // namespace teg::internal

namespace teg {

/// Number of members of an aggregate type.
/// @tparam T An aggregate type
///
/// @see https://en.cppreference.com/w/cpp/language/aggregate_initialization
template <aggregate T>
constexpr std::size_t members_count_v = internal::members_count_impl<T>();

} // namespace teg

#endif // TEG_MEMBERS_COUNT_H