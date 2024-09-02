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
#include <type_traits>
#include <concepts>
#include <iterator>
#include <memory>
#include <utility>
#include <string>

namespace teg {

template<typename T, typename ... U>
concept any_of = (std::same_as<T, U> || ...);

template <typename T>
struct is_basic_string : std::false_type {};

template <typename C, typename T, typename A>
struct is_basic_string<std::basic_string<C, T, A>> : std::true_type {};

template <typename T>
constexpr bool is_basic_string_v = is_basic_string<T>::value;

template <typename T>
concept basic_string = is_basic_string_v<T>;

template <typename C>
concept allocator_aware = requires (C container) {
    { container.get_allocator() } -> std::same_as<typename C::allocator_type>;
};



template <typename T>
concept default_constructable =
requires(T* p) {
    std::construct_at(p);
};

template <typename C, typename A, typename T>
concept allocator_constructable = requires(A m, T* p) {
    requires std::same_as<
        typename C::allocator_type, 
        typename std::allocator_traits<A>::rebind_alloc<T>>;

    std::allocator_traits<A>::construct(m, p);
};

///  Specifies that an instance of the type can be default-constructed 
///  in-place by a given allocator.
template <typename C, typename T>
concept default_insertable = 
       ( basic_string<C>    && default_constructable<T>)
    || (!allocator_aware<C> && default_constructable<T>)
    || ( allocator_aware<C> && allocator_constructable<C, typename C::allocator_type, T>);



template <typename T>
concept default_move_constructable =
requires(T* p, T&& rv) {
    std::construct_at(p, std::forward<T>(rv));
};

template <typename C, typename A, typename T>
concept allocator_move_constructable = requires(A m, T* p, T&& rv) {
    requires std::same_as<
        typename C::allocator_type, 
        typename std::allocator_traits<A>::rebind_alloc<T>>;

    std::allocator_traits<A>::construct(m, p, std::forward<T>(rv));
};

/// Specifies that an object of the type can be constructed into uninitialized
/// storage from an rvalue of that type by a given allocator.
template <typename C, typename T>
concept move_insertable = 
       ( basic_string<C>    && default_move_constructable<T>)
    || (!allocator_aware<C> && default_move_constructable<T>)
    || ( allocator_aware<C> && allocator_move_constructable<C, typename C::allocator_type, T>);



template <typename T>
concept default_copy_constructable =
requires(T* p, T v) {
    std::construct_at(p, v);
};

template <typename C, typename A, typename T>
concept allocator_copy_constructable = requires(A m, T* p, T v) {
    requires std::same_as<
        typename C::allocator_type, 
        typename std::allocator_traits<A>::rebind_alloc<T>>;

    std::allocator_traits<A>::construct(m, p, v);
};

///  Specifies that an instance of the type can be copy-constructed 
///  in-place by a given allocator.
template <typename C, typename T>
concept copy_insertable = move_insertable<C, T>
    && ( basic_string<C>    && default_copy_constructable<T>)
    || (!allocator_aware<C> && default_copy_constructable<T>)
    || ( allocator_aware<C> && allocator_copy_constructable<C, typename C::allocator_type, T>);



template <typename T, typename... Args>
concept default_emplace_constructable =
requires(T* p, Args&&... args) {
    std::construct_at(p, args...);
};

template <typename C, typename A, typename T, typename... Args>
concept allocator_emplace_constructable = requires(A m, T* p, Args&&... args) {
    requires std::same_as<
        typename C::allocator_type, 
        typename std::allocator_traits<A>::rebind_alloc<T>>;

    std::allocator_traits<A>::construct(m, p, std::forward<T>(args)...);
};

template <typename C, typename T>
concept emplace_constructible = 
       ( basic_string<C>    && default_emplace_constructable<T>)
    || (!allocator_aware<C> && default_emplace_constructable<T>)
    || ( allocator_aware<C> && allocator_emplace_constructable<C, typename C::allocator_type, T>);



template <typename T>
concept default_destroyable = requires(T* p) {
    std::destroy_at(p);
};

template <typename C, typename A, typename T>
concept allocator_destroyable = requires(A m, T* p) {
    requires std::same_as<
        typename C::allocator_type, 
        typename std::allocator_traits<A>::rebind_alloc<T>>;

    std::allocator_traits<A>::destroy(m, p);
};

///  Specifies that an object of the type can be destroyed by 
///  a given allocator.
template <typename C, typename T>
concept erasable = 
       ( basic_string<C>    && default_destroyable<T>)
    || (!allocator_aware<C> && default_destroyable<T>)
    || ( allocator_aware<C> && allocator_destroyable<C, typename C::allocator_type, T>);



///  Containers are objects that store other objects. They control allocation
///  and deallocation  of these objects through constructors, destructors,
///  insert and erase operations. 
/// 
///  ISO/IEC 14882:2020 [container.requirements.general]
template <typename C>
concept container = std::regular<C> 
    && requires (C container) {
        /// Value type requiements
        typename C::value_type;
        requires std::copy_constructible<typename C::value_type>;
        requires teg::copy_insertable<C, typename C::value_type>;
        requires teg::emplace_constructible<C, typename C::value_type>;
        requires teg::erasable<C, typename C::value_type>;
        requires std::equality_comparable<typename C::value_type>;

        typename C::reference;
        typename C::const_reference;
        requires std::same_as<typename C::reference, typename C::value_type&>;
        requires std::same_as<typename C::const_reference, typename C::value_type const&>;

        typename C::iterator;
        typename C::const_iterator;
        requires std::forward_iterator<typename C::iterator>;
        requires std::forward_iterator<typename C::const_iterator>;
        requires std::convertible_to<typename C::iterator, typename C::const_iterator>;
        requires std::same_as<std::iter_value_t<typename C::iterator>, typename C::value_type>;
        requires std::same_as<std::iter_value_t<typename C::const_iterator>, typename C::value_type>;
        requires std::same_as<std::iter_reference_t<typename C::iterator>, typename C::reference>
            || std::same_as<std::iter_reference_t<typename C::iterator>, typename C::const_reference>;
        requires std::same_as<std::iter_reference_t<typename C::const_iterator>, typename C::const_reference>;

        typename C::difference_type;
        typename C::size_type; 
        requires std::signed_integral<typename C::difference_type>;
        requires std::unsigned_integral<typename C::size_type>;
        requires std::in_range<typename C::size_type>(std::numeric_limits<typename C::difference_type>::max());
        requires std::same_as<
            typename C::difference_type,
            typename std::iterator_traits<typename C::iterator>::difference_type>;
        requires std::same_as<
            typename C::difference_type,
            typename std::iterator_traits<typename C::const_iterator>::difference_type>;

        /// Container requiements
        { container.begin() } -> any_of<typename C::iterator, typename C::const_iterator>;
        { container.end() } -> any_of<typename C::iterator, typename C::const_iterator>;
        { container.cbegin() } -> std::same_as<typename C::const_iterator>;
        { container.cend() } -> std::same_as<typename C::const_iterator>;    
        { container.max_size() } -> std::same_as<typename C::size_type>;
        { container.empty() } -> std::convertible_to<bool>;
    };

template <typename T>
concept fixed_nonzero_size = requires {
    { T{}.size() } -> std::convertible_to<std::size_t>;
    { T{}.size() > 0 } -> std::convertible_to<bool>;
    requires T{}.size() > 0;
};

template <typename T>
concept fixed_size_container = container<T> && fixed_nonzero_size<T>;

///  A sequence container organizes a finite set of objects, all of the same type, 
///  into a strictly linear arrangement. The library provides four basic kinds of 
///  sequence containers: vector, forward_list, list, and deque. In addition, array
///  is provided as a sequence container which provides limited sequence operations 
///  because it has a fixed number of elements.
///
///  ISO/IEC 14882:2020 [sequence.reqmts]
template <typename T>
concept sequence_container = container<T>
    && requires (T container) {
        typename T::value_type;
        typename T::reference;
        typename T::const_reference;
        typename T::iterator;
        typename T::const_iterator;
    };

///  Associative containers provide fast retrieval of data based on keys.
///  The standard library provides four basic kinds of associative containers: 
///  set, multiset, map and multimap.
///
///  ISO/IEC 14882:2020 [associative.reqmts]
template <typename C>
concept associative_container = container<C> && requires (C container) {
    typename C::key_type;
};

template <typename T>
concept serializable_container =
       fixed_size_container<T> 
    || sequence_container<T> 
    || associative_container<T>;
    
} // namespace teg