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

#ifndef TEG_CORE_CONCEPTS_H
#define TEG_CORE_CONCEPTS_H

#include "teg/def.h"
#include "teg/util.h"

namespace teg::concepts {

///  \brief A fundamental type.
///  \see https://en.cppreference.com/w/cpp/language/types
///  
template <typename T>
concept fundamental = std::is_fundamental_v<T>;

///  \brief An enum type.
///  \see https://en.cppreference.com/w/cpp/language/enum
///  
template <typename T>
concept is_enum = std::is_enum_v<T>;

///  \brief A character type.
///  \see https://en.cppreference.com/w/cpp/language/types
///  
template <class T>
concept character = 
       std::is_same_v<std::remove_cv_t<T>, char> 
    || std::is_same_v<std::remove_cv_t<T>, signed char> 
    || std::is_same_v<std::remove_cv_t<T>, unsigned char> 
    || std::is_same_v<std::remove_cv_t<T>, wchar_t> 
    || std::is_same_v<std::remove_cv_t<T>, char8_t>
    || std::is_same_v<std::remove_cv_t<T>, char16_t> 
    || std::is_same_v<std::remove_cv_t<T>, char32_t>;

///  \brief A class type.
///  \see https://en.cppreference.com/w/cpp/language/class
///  
template <class T>
concept is_class = std::is_class_v<T>;

///  \brief A trivial type.
///  \see https://en.cppreference.com/w/cpp/named_req/TrivialType
///  
template <class T>
concept trivial = std::is_trivial_v<T>;

///  \brief A trivially copyable type.
///  \see https://en.cppreference.com/w/cpp/named_req/TriviallyCopyable
///  
template <class T>
concept trivially_copyable = std::is_trivially_copyable_v<T>;

///  \brief A standard layout type.
///  \see https://en.cppreference.com/w/cpp/named_req/StandardLayoutType
///  
template <class T>
concept standard_layout = std::is_standard_layout_v<T>;

    
///  \brief An c-array type.
///  \see https://en.cppreference.com/w/cpp/language/array
///  
template <class T>
concept c_array = std::is_array_v<T>;

///  \brief A bounded c-array type.
///
///  A c-array is considered bounded when its size is kwown at compile-time.
///  
///  \see https://en.cppreference.com/w/cpp/types/is_bounded_array
///  
template <class T>
concept bounded_c_array = std::is_bounded_array_v<T>;

///  \brief An unbounded c-array type.
///
///  A c-array is considered unbounded when its size is unknown at compile-time.
///  Most of the time this occurs when a c-array decays to a pointer.
///
///  \see https://en.cppreference.com/w/cpp/types/is_unbounded_array
///  \see https://en.cppreference.com/w/cpp/language/array#Arrays_of_unknown_bound
///  
template <class T>
concept unbounded_c_array = std::is_unbounded_array_v<T>;

///  \brief An aggregate type.
///
///  An aggregate is an array or a class with no user-declared or inherited constructors,
///  no private or protected direct non-static data members, no virtual functions, and
///  no virtual, private, or protected base classes. ISO/IEC 14882:2020 [dcl.init.aggr].
///
///  \see https://en.cppreference.com/w/cpp/language/aggregate_initialization
///  
template <class T>
concept aggregate = std::is_aggregate_v<T> && !unbounded_c_array<T>;

///  \brief A tuple-like element type.
///  
///  Asserts that a element of a tuple-like type `T` can be accessed at compile-time 
///  by an index `I`.
///  
///  \tparam T A tuple-like type.
///  \tparam I A compile-time index.
///  
///  \see https://en.cppreference.com/w/cpp/utility/tuple/tuple_element
///  \see https://en.cppreference.com/w/cpp/utility/tuple/get
///  
template<class T, std::size_t I>
concept tuple_element = requires(T t) {
        typename std::tuple_element_t<I, std::remove_const_t<T>>;
        { std::get<I>(t) } -> std::convertible_to<const std::tuple_element_t<I, T>&>;
    };

template <class T>
concept tuple_size = requires { typename std::tuple_size<T>::type; };

///  \brief A tuple-like type.
///  
///  A tuple is a heterogeneous, fixed-size collection of values. 
///  ISO/IEC 14882:2020 [tuple.general].
///  
///  \note Elements of tuple-like objects can be bound with structured binding.
///  \note Tuple-like types: std::tuple, std::pair, std::array and std::ranges::subrange.
///  \see https://en.cppreference.com/w/cpp/utility/tuple
///  
template<class T>
concept tuple = tuple_size<T>
    && std::derived_from<
        std::tuple_size<T>, 
        std::integral_constant<std::size_t, std::tuple_size_v<T>>
    >
    && []<std::size_t... I>(std::index_sequence<I...>) constexpr { 
        return (tuple_element<T, I> && ...); 
    }(std::make_index_sequence<std::tuple_size_v<T>>());

///  \brief A pair-like type.
///  
///  A tuple-like type that contains two elements.
///  \see https://en.cppreference.com/w/cpp/utility/pair
///  
template <class T>
concept pair = tuple<T> && std::tuple_size_v<T> == 2;

///  \brief A span type.
///  
///  A span is a view over a contiguous sequence of objects, the storage of which is
///  owned by some other object. ISO/IEC 14882:2020 [span.overview].
///  
///  \see https://en.cppreference.com/w/cpp/container/span
///  
template <class T>
concept span = 
    std::same_as<
        typename T::value_type, 
        std::remove_cv_t<typename T::element_type>
    > 
    && std::contiguous_iterator<typename T::iterator>
    && std::random_access_iterator<typename T::reverse_iterator>
    && requires(T span, typename T::size_type i) {
        { span.size() } -> std::same_as<typename T::size_type>;
        { span.data() } -> std::same_as<typename T::pointer>;

        { span.begin() }  -> std::same_as<typename T::iterator>;
        { span.end() }    -> std::same_as<typename T::iterator>;
        { span.rbegin() } -> std::same_as<typename T::reverse_iterator>;
        { span.rend() }   -> std::same_as<typename T::reverse_iterator>;

        { span.operator[](i) } -> std::same_as<typename T::reference>;
        { span.front() }       -> std::same_as<typename T::reference>;
        { span.back() }        -> std::same_as<typename T::reference>;
    };

///  \brief A span type with a dynamic extent.
///  
///  A span can either have a static extent, in which case the number of elements in the
///  sequence is known at compile-time and encoded in the type, or a dynamic extent.
///  
///  \see https://en.cppreference.com/w/cpp/container/span
///  
template <class T>
concept dynamic_span = span<T> && T::extent == std::dynamic_extent;

///  \brief A span type with a static extent.
///  
///  A span can either have a static extent, in which case the number of elements in the
///  sequence is known at compile-time and encoded in the type, or a dynamic extent.
///  
///  \see https://en.cppreference.com/w/cpp/container/span
///  
template <class T>
concept static_span = span<T> && T::extent != std::dynamic_extent;

///  \brief An optional type.
///  
///  An optional type is an object that may or may not contain a value.
///  \see https://en.cppreference.com/w/cpp/utility/optional
///  
template <typename T>
concept optional = requires(T a) {
    typename T::value_type;
    a.value();
    a.has_value();
    a.operator bool();
    a.operator*();
};

namespace internal {

// Unique pointer type traits.
template <class T>
struct is_unique_ptr 
    : std::false_type {};

template <class T>
struct is_unique_ptr<std::unique_ptr<T, std::default_delete<T>>>
    : std::true_type {};

template <class T>
constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

// Shared pointer type traits.
template <typename T>
struct is_shared_ptr 
    : std::false_type {};

template <typename T>
struct is_shared_ptr<std::shared_ptr<T>> 
    : std::true_type {};

template <typename T>
constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

} // namespace internal

///  \brief A unique pointer type.
///  \see https://en.cppreference.com/w/cpp/memory/unique_ptr
template <class T>
concept unique_ptr = internal::is_unique_ptr_v<T>;

///  \brief A shared pointer type.
///  \see https://en.cppreference.com/w/cpp/memory/shared_ptr
template <class T>
concept shared_ptr = internal::is_shared_ptr_v<T>;

///  \brief An owning pointer type.
///  
///  A pointer type that is responsible for managing the lifetime of
///  the pointed-to object.
/// 
///  \see https://en.cppreference.com/w/cpp/header/memory
template <typename T>
concept owning_ptr = unique_ptr<T> || shared_ptr<T>;

namespace internal {
    
// Variant type traits.
template <class T>
constexpr inline bool is_variant_v = false;

template <class... T>
constexpr inline bool is_variant_v<std::variant<T...>> = true;

} // namespace internal

///  \brief A variant type.
///  
///  A variant represents a type-safe union (sometimes called a tagged union or
///  a discriminated union).
///  
///  \see https://en.cppreference.com/w/cpp/utility/variant
///  
template <class T>
concept variant = internal::is_variant_v<T>;

template <class T>
concept bit_castable = requires { std::bit_cast<T>(std::array<std::byte, sizeof(T)>{}); }
    && is_constexpr_friendly([] { std::ignore = std::bit_cast<T>(std::array<std::byte, sizeof(T)>{}); });

} // namespace teg::concepts

#endif