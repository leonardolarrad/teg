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

#include <cstdint>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>
#include <span>

namespace teg {

template <typename T>
concept int_like = std::is_integral_v<std::remove_cvref_t<T>>;

template <typename T>
concept float_like = std::is_floating_point_v<std::remove_cvref_t<T>>;

template <typename T>
concept class_like = std::is_class_v<std::remove_cvref_t<T>>;

template <typename T>
concept container_like = requires(T container) {
    typename std::remove_cvref_t<T>::value_type;
    container.size();
    container.begin();
    container.end();
};

template <typename T>
concept span_like = container_like<T> && requires (T span) {
    T{(typename T::value_type*)nullptr, std::size_t{}};
    span.subspan(std::size_t{}, std::size_t{});
};

template <typename T>
concept dynamic_span_like = span_like<T> && T::extent == std::dynamic_extent;

template <typename T>
concept static_span_like = span_like<T> && T::extent != std::dynamic_extent;

template <typename T>
concept array_like = container_like<T> && requires(T array) {
    typename std::remove_cvref_t<T>::value_type;
    array.size();
};

template <class T>
concept c_array_like = std::is_array_v<T> 
    && std::extent_v<std::remove_cvref_t<T>> > 0;

template <typename T>
concept map_like = container_like<T> && requires(T map) {
    typename std::remove_cvref_t<T>::key_type;
    typename std::remove_cvref_t<T>::mapped_type;
};

template <typename T>
concept set_like = container_like<T> && requires(T set) {
    typename std::remove_cvref_t<T>::key_type;
};

template <typename T>
concept bitset_like = requires (T bitset){
    bitset.flip();
    bitset.set();
    bitset.reset();
    bitset.count();
} && (T{}.size()+7)/8 == sizeof(T);

template <typename T>
constexpr bool char_like = 
       std::is_same_v<T, signed char> 
    || std::is_same_v<T, char> 
    || std::is_same_v<T, unsigned char> 
    || std::is_same_v<T, wchar_t> 
    || std::is_same_v<T, char16_t> 
    || std::is_same_v<T, char32_t>;

template <typename T>
concept string_like = container_like<T> && requires(T string) {
    requires char_like<typename std::remove_cvref_t<T>::value_type>;
    string.length();
    string.data();
};

template <typename T>
concept string_view_like = string_like<T> && !requires(T string_view) {
    string_view.resize(std::size_t{});
};

template <typename T>
concept expected_like = requires(T expected) {
    typename std::remove_cvref_t<T>::value_type;
    typename std::remove_cvref_t<T>::error_type;
    typename std::remove_cvref_t<T>::unexpected_type;
    expected.has_value();
    expected.error();
    requires std::is_same_v<void, typename std::remove_cvref_t<T>::value_type> ||
    requires(T expected) {  
        expected.value();
    };
};

template <typename T>
concept optional_like = !expected_like<T> && requires(T optional) {
    typename std::remove_cvref_t<T>::value_type;
    optional.value();
    optional.has_value();
    optional.operator*();
};

template <typename T>
concept fundamental = std::is_fundamental_v<std::remove_cvref_t<T>> 
    || std::is_enum_v<std::remove_cvref_t<T>>;

template <typename T>
concept aggregate = std::is_aggregate_v<std::remove_cvref_t<T>>;

template <typename T>
concept deserializable = fundamental<T> || aggregate<T>;

} // namespace teg