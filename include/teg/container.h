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

#include "base_concepts.h"

namespace teg {

template <typename T>
concept optional = requires(unqualified<T> a) {
    typename unqualified<T>::value_type;
    a.value();
    a.has_value();
    a.operator bool();
    a.operator*();
};

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
    { container.get_allocator() } -> std::same_as<typename unqualified<C>::allocator_type>;
};

template <typename C, typename A, typename T>
concept valid_allocator = 
    std::same_as<
        typename unqualified<C>::allocator_type,
        typename std::allocator_traits<A>::template rebind_alloc<T>>;

template <typename T>
concept default_constructable = requires(T* p) {
    std::construct_at(p);
};

template <typename C, typename A, typename T>
concept allocator_constructable = 
       valid_allocator<C, A, T>
    && requires(A m, T* p) {
        std::allocator_traits<A>::construct(m, p);
    };

///  Specifies that an instance of the type can be default-constructed 
///  in-place by a given allocator.
template <typename C, typename T>
concept default_insertable = 
       ( basic_string<C>    && default_constructable<T>)
    || (!allocator_aware<C> && default_constructable<T>)
    || ( allocator_aware<C> && allocator_constructable<C, typename unqualified<C>::allocator_type, T>);




template <typename T>
concept default_move_constructable =
requires(T* p, T&& rv) {
    std::construct_at(p, std::forward<T>(rv));
};

template <typename C, typename A, typename T>
concept allocator_move_constructable = 
       valid_allocator<C, A, T>
    && requires(A m, T* p, T&& rv) {
        std::allocator_traits<A>::construct(m, p, std::forward<T>(rv));
    };

/// Specifies that an object of the type can be constructed into uninitialized
/// storage from an rvalue of that type by a given allocator.
template <typename C, typename T>
concept move_insertable = 
       ( basic_string<C>    && default_move_constructable<T>)
    || (!allocator_aware<C> && default_move_constructable<T>)
    || ( allocator_aware<C> && allocator_move_constructable<C, typename unqualified<C>::allocator_type, T>);




template <typename T>
concept default_copy_constructable =
requires(T* p, T v) {
    std::construct_at(p, v);
};

template <typename C, typename A, typename T>
concept allocator_copy_constructable = 
       valid_allocator<C, A, T>
    && requires(A m, T* p, T v) {
        std::allocator_traits<A>::construct(m, p, v);
    };

///  Specifies that an instance of the type can be copy-constructed 
///  in-place by a given allocator.
template <typename C, typename T>
concept copy_insertable = move_insertable<C, T>
    && ((basic_string<C>    && default_copy_constructable<T>)
    || (!allocator_aware<C> && default_copy_constructable<T>)
    || ( allocator_aware<C> && allocator_copy_constructable<C, typename unqualified<C>::allocator_type, T>));




template <typename T>
concept default_emplace_constructable = requires(T* p, T&& args) {
    std::construct_at(p, std::forward<T>(args), std::forward<T>(args));
};

template <typename C, typename A, typename T>
concept allocator_emplace_constructable = 
       valid_allocator<C, A, T>
    && requires(A m, T* p, T&& args) {
        std::allocator_traits<A>::construct(m, p, std::forward<T>(args), std::forward<T>(args));
    };

template <typename C, typename T>
concept emplace_constructible = 
       ( basic_string<C>    && default_emplace_constructable<T>)
    || (!allocator_aware<C> && default_emplace_constructable<T>)
    || ( allocator_aware<C> && allocator_emplace_constructable<C, typename unqualified<C>::allocator_type, T>);



template <typename T>
concept default_destroyable = requires(T* p) {
    std::destroy_at(p);
};


template <typename C, typename A, typename T>
concept allocator_destroyable = 
       valid_allocator<C, A, T>
    && requires(A m, T* p) {
        std::allocator_traits<A>::destroy(m, p);
    };

///  Specifies that an object of the type can be destroyed by 
///  a given allocator.
template <typename C, typename T>
concept erasable = 
       ( basic_string<C>    && default_destroyable<T>)
    || (!allocator_aware<C> && default_destroyable<T>)
    || ( allocator_aware<C> && allocator_destroyable<C, typename unqualified<C>::allocator_type, T>);




template <typename C, typename T>
concept container_element =
       std::same_as<T, typename unqualified<C>::value_type>
    && erasable<C, T>;

template <typename C, typename R>
concept container_element_reference = 
       std::same_as<R, typename unqualified<C>::reference>
    && std::same_as<R, typename unqualified<C>::value_type&>;

template <typename C, typename CR>
concept container_element_const_reference = 
       std::same_as<CR, typename unqualified<C>::const_reference>
    && std::same_as<CR, typename unqualified<C>::value_type const&>;    

template <typename C, typename I>
concept container_element_iterator = 
       std::same_as<I, typename unqualified<C>::iterator>
    && std::forward_iterator<I>
    && std::same_as<std::iter_value_t<I>, typename unqualified<C>::value_type>
    && std::convertible_to<I, typename unqualified<C>::const_iterator>;

template <typename C, typename CI>
concept container_element_const_iterator = 
       std::same_as<CI, typename unqualified<C>::const_iterator>
    && std::forward_iterator<CI>
    && std::same_as<std::iter_value_t<CI>, typename unqualified<C>::value_type>;

template <typename C, typename S>
concept container_difference_type =
       std::signed_integral<S>
    && std::same_as<S, typename unqualified<C>::difference_type>
    && std::same_as<S, typename std::iterator_traits<typename unqualified<C>::iterator>::difference_type>
    && std::same_as<S, typename std::iterator_traits<typename unqualified<C>::const_iterator>::difference_type>;
    
template <typename C, typename S>
concept container_size_type = 
       std::unsigned_integral<S>
    && std::same_as<S, typename unqualified<C>::size_type>
    && std::in_range<S>(std::numeric_limits<typename unqualified<C>::difference_type>::max());

template <typename C, typename T>
concept shared_strongest_property = 
       (!std::equality_comparable<typename unqualified<C>::value_type> || std::equality_comparable<C>)
    && (!std::movable<typename unqualified<C>::value_type> || std::movable<C>)
    && (!std::copyable<typename unqualified<C>::value_type> || std::copyable<C> )
    && (!std::semiregular<typename unqualified<C>::value_type> || std::semiregular<C>)
    && (!std::regular<typename unqualified<C>::value_type> || std::regular<C>);

///  Containers are objects that store other objects. They control allocation
///  and deallocation  of these objects through constructors, destructors,
///  insert and erase operations. 
///
///  ISO/IEC 14882:2020 [container.requirements.general]
template <typename C>
concept container = !optional<C> &&
       container_element<C, typename unqualified<C>::value_type>
    && container_element_reference<C, typename unqualified<C>::reference>
    && container_element_const_reference<C, typename unqualified<C>::const_reference>
    && container_element_iterator<C, typename unqualified<C>::iterator>
    && container_element_const_iterator<C, typename unqualified<C>::const_iterator>
    && container_difference_type<C, typename unqualified<C>::difference_type>
    && container_size_type<C, typename unqualified<C>::size_type>
    && requires (unqualified<C> a, unqualified<C> const b) {
        { a.begin() }    -> std::same_as<typename unqualified<C>::iterator>;
        { a.end() }      -> std::same_as<typename unqualified<C>::iterator>;
        { b.begin() }    -> std::same_as<typename unqualified<C>::const_iterator>;
        { b.end() }      -> std::same_as<typename unqualified<C>::const_iterator>;
        { a.cbegin() }   -> std::same_as<typename unqualified<C>::const_iterator>;
        { a.cend() }     -> std::same_as<typename unqualified<C>::const_iterator>;    
        { a.max_size() } -> std::same_as<typename unqualified<C>::size_type>;
        { a.empty() }    -> std::convertible_to<bool>;
    };

///   If the iterator type of a container belongs to the bidirectional or 
///   random access iterator categories (23.3), the container is called reversible.
///
///  ISO/IEC 14882:2020 [container.requirements.general]
template <typename C>
concept reversible_container = container<C>
    && std::bidirectional_iterator<typename unqualified<C>::iterator>
    && std::bidirectional_iterator<typename unqualified<C>::const_iterator>
    && std::bidirectional_iterator<typename unqualified<C>::reverse_iterator>
    && std::bidirectional_iterator<typename unqualified<C>::const_reverse_iterator>
    && std::convertible_to<typename unqualified<C>::reverse_iterator, typename unqualified<C>::const_reverse_iterator>
    && requires(unqualified<C> a, unqualified<C> const b) {
        { a.rbegin() }  -> std::same_as<typename unqualified<C>::reverse_iterator>;
        { a.rend() }    -> std::same_as<typename unqualified<C>::reverse_iterator>;
        { b.rbegin() }  -> std::same_as<typename unqualified<C>::const_reverse_iterator>;
        { b.rend() }    -> std::same_as<typename unqualified<C>::const_reverse_iterator>;
        { a.crbegin() } -> std::same_as<typename unqualified<C>::const_reverse_iterator>;
        { a.crend() }   -> std::same_as<typename unqualified<C>::const_reverse_iterator>;
    };

template <typename C>
concept sized_container = container<C>
    && requires(unqualified<C> const a) {
        { a.size() } -> std::same_as<typename unqualified<C>::size_type>;
    };

template <typename T>
concept has_fixed_nonzero_size = std::integral_constant<
        std::size_t,
        std::remove_cvref_t<T>{}.size()
    >::value > 0;

template <typename C>
concept resizable_container = sized_container<C>
    && requires(C a, typename C::size_type const n) {
        a.resize(n);
    };

template <typename C>
concept clearable_container = container<C>
    && requires(C a) {
        a.clear();
    };

template <typename C>
concept reservable_container = sized_container<C>
    && requires(C a, typename C::size_type const n) {
        a.reserve(n);
    };

template <typename C>
concept invertible_container = container<C>
    && requires(C a) {
        a.reverse();
    };        
    
///  A double-ended container that allows indexed access.
///  Satisfied by array, vector, deque and basic_string.
template <typename C>
concept random_access_container = 
       reversible_container<C> && sized_container<C>
    && std::random_access_iterator<typename unqualified<C>::iterator>
    && std::random_access_iterator<typename unqualified<C>::const_iterator>
    && requires(
        unqualified<C> a, unqualified<C> const b, 
        typename unqualified<C>::size_type const i
    ) {
        { a[i] } -> std::same_as<typename unqualified<C>::reference>;
        { b[i] } -> std::same_as<typename unqualified<C>::const_reference>;
    };

/// A random access container that stores elements in a contiguous memory region.
/// Satisfied by array, vector and basic_string.
template <typename C>
concept contiguous_container = 
       random_access_container<C>
    && std::contiguous_iterator<typename unqualified<C>::iterator>
    && std::contiguous_iterator<typename unqualified<C>::const_iterator>
    && std::contiguous_iterator<typename unqualified<C>::pointer>
    && std::contiguous_iterator<typename unqualified<C>::const_pointer>
    && std::convertible_to<typename unqualified<C>::pointer, typename unqualified<C>::const_pointer>
    && requires(unqualified<C> a, unqualified<C> const b) {
        { a.data() } -> std::same_as<typename unqualified<C>::pointer>;
        { b.data() } -> std::same_as<typename unqualified<C>::const_pointer>;
    };

template <typename C>
concept trivial_contiguous_container = 
       contiguous_container<C> 
    && resizable_container<C>
    && trivially_copyable<typename unqualified<C>::value_type>;

template <typename C>
concept fixed_size_container = contiguous_container<C> && has_fixed_nonzero_size<C>;

template <typename C>
concept inplace_constructing_container = container<C>
    && emplace_constructible<C, typename unqualified<C>::value_type>
    && requires (C a, typename unqualified<C>::value_type&& rv) {
        a.emplace();
        a.emplace(std::forward<typename unqualified<C>::value_type>(rv));
    };

template <typename C>
concept back_inplace_constructing_container = container<C>
    && emplace_constructible<C, typename unqualified<C>::value_type>
    && requires (C a, typename unqualified<C>::value_type&& rv) {
        { a.emplace_back() } -> std::same_as<typename unqualified<C>::reference>;
        { a.emplace_back(std::forward<typename unqualified<C>::value_type>(rv)) } -> std::same_as<typename unqualified<C>::reference>;
    };

template <typename C>
concept front_inplace_constructing_container = container<C>
    && emplace_constructible<C, typename unqualified<C>::value_type>
    && requires (C a, typename unqualified<C>::value_type&& rv) {
        { a.emplace_front() } -> std::same_as<typename unqualified<C>::reference>;
        { a.emplace_front(std::forward<typename unqualified<C>::value_type>(rv)) } -> std::same_as<typename unqualified<C>::reference>;
    };

template <typename C>
concept range_constructing_container = container<C>
    && emplace_constructible<C, typename unqualified<C>::value_type>
    && requires (typename unqualified<C>::iterator i, typename unqualified<C>::iterator j) {
        C { i, j };
    };

///  Associative containers provide fast retrieval of data based on keys.
///  The standard library provides four basic kinds of associative containers: 
///  set, multiset, map and multimap.
///
///  ISO/IEC 14882:2020 [associative.reqmts]
template <typename C>
concept associative_container = 
       sized_container<C>
    && clearable_container<C>
    && inplace_constructing_container<C> 
    && requires {
        typename unqualified<C>::key_type;
    };

template <typename C>
concept set_container = 
       associative_container<C>
    && std::same_as<typename unqualified<C>::key_type, typename unqualified<C>::value_type>;

template <typename C>
concept map_container = 
       associative_container<C>
    && requires {
        typename unqualified<C>::mapped_type;
    };

template <typename C, typename T>
concept container_of = container<C> && std::same_as<T, typename unqualified<C>::value_type>;

template <typename C, typename T>
concept random_access_container_of = random_access_container<C> && std::same_as<T, typename unqualified<C>::value_type>;

template <typename C, typename T>
concept contiguous_container_of = contiguous_container<C> && std::same_as<T, typename unqualified<C>::value_type>;


///  @brief Concept for associative containers.
/// 
///  An associative container is a container that stores its elements in a way
///  that allows efficient lookup of elements based on a key.
/// 
///  The standard library provides four basic kinds of associative containers:
///  set, multiset, map and multimap.
/// 
///  @tparam C The container type.
///  @tparam T The type of elements stored in the container.
/// 
///  @see associative_container
template <typename C, typename T>
concept associative_container_of = associative_container<C> && std::same_as<T, typename unqualified<C>::value_type>;

} // namespace teg