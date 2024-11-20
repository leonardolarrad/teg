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

#ifndef TEG_CONTAINER_CONCEPTS_H
#define TEG_CONTAINER_CONCEPTS_H

#include <concepts>
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <tuple>

#include "core_concepts.h"

namespace teg::concepts {

namespace internal {

// Basic string type traits.
template <class T>
struct is_basic_string : std::false_type {};

template <class C, class T, class A>
struct is_basic_string<std::basic_string<C, T, A>> : std::true_type {};

template <class T>
constexpr bool is_basic_string_v = is_basic_string<T>::value;

} // namespace internal

///  Determines whether a type `T` is `std::basic_string`.
///  \see https://en.cppreference.com/w/cpp/string/basic_string
///
template <class T>
concept is_basic_string = internal::is_basic_string_v<T>;

///  \brief Determines whether a type `T` is allocator-aware. 
///  
///  A type is allocator-aware if it holds an allocator and has a
///  `get_allocator()` accessor.
/// 
///  \see https://en.cppreference.com/w/cpp/memory/allocator
///
template <class T>
concept allocator_aware = requires (T type) {
    { type.get_allocator() } -> std::same_as<typename T::allocator_type>;
};

///  A type `T` that can be default-constructed at a given address `p`.
///  \see https://en.cppreference.com/w/cpp/memory/construct_at
///
template <class T>
concept default_constructable = requires(T* p) {
    std::construct_at(p);
};

///  A type `T` that can be default-constructed at a given address `p` using
///  an allocator `A`.
///  \see https://en.cppreference.com/w/cpp/memory/allocator_traits
///
template <class C, class A, class T>
concept allocator_constructable = 
    std::same_as<
        typename C::allocator_type,
        typename std::allocator_traits<A>::template rebind_alloc<T>
    >
    && requires(A m, T* p) {
        std::allocator_traits<A>::construct(m, p);
    };

///  Specifies whether an instance of the type `T` can be default-constructed 
///  in-place by a given allocator `C::allocator_type`.
/// 
///  \see https://en.cppreference.com/w/cpp/named_req/DefaultInsertable
///
template <class C, class T>
concept default_insertable = 
       ( is_basic_string<C> && default_constructable<T>)
    || (!allocator_aware<C> && default_constructable<T>)
    || ( allocator_aware<C> && allocator_constructable<C, typename C::allocator_type, T>);

///  A type `T` that can be constructed from an rvalue of that type at a given address `p`.
///  \see https://en.cppreference.com/w/cpp/memory/construct_at
///
template <class T>
concept default_move_constructable =
requires(T* p, T&& rv) {
    std::construct_at(p, std::forward<T>(rv));
};

///  A type `T` that can be constructed from an rvalue of that type at a given address `p` using
///  an allocator `A`.
///  \see https://en.cppreference.com/w/cpp/memory/allocator_traits
///
template <class C, class A, class T>
concept allocator_move_constructable = 
    std::same_as<
        typename C::allocator_type,
        typename std::allocator_traits<A>::template rebind_alloc<T>
    >
    && requires(A m, T* p, T&& rv) {
        std::allocator_traits<A>::construct(m, p, std::forward<T>(rv));
    };

/// Specifies that an object of the type `T` can be constructed into uninitialized
/// storage from an rvalue of that type by a given allocator `C::allocator_type`.
///
/// \see https://en.cppreference.com/w/cpp/named_req/MoveInsertable
///
template <class C, class T>
concept move_insertable = 
       ( is_basic_string<C> && default_move_constructable<T>)
    || (!allocator_aware<C> && default_move_constructable<T>)
    || ( allocator_aware<C> && allocator_move_constructable<C, typename C::allocator_type, T>);

///  A type `T` that can be copy-constructed at a given address `p`.
///  \see https://en.cppreference.com/w/cpp/memory/construct_at
///
template <class T>
concept default_copy_constructable =
requires(T* p, T v) {
    std::construct_at(p, v);
};

///  A type `T` that can be copy-constructed at a given address `p` using
///  an allocator `A`.
///  \see https://en.cppreference.com/w/cpp/memory/allocator_traits
///
template <class C, class A, class T>
concept allocator_copy_constructable = 
    std::same_as<
        typename C::allocator_type,
        typename std::allocator_traits<A>::template rebind_alloc<T>
    >
    && requires(A m, T* p, T v) {
        std::allocator_traits<A>::construct(m, p, v);
    };

///  Specifies that an instance of the type `T` can be copy-constructed 
///  in-place by a given allocator `C::allocator_type`.
///
///  \see https://en.cppreference.com/w/cpp/named_req/CopyInsertable
///
template <class C, class T>
concept copy_insertable = move_insertable<C, T>
    && ((is_basic_string<C> && default_copy_constructable<T>)
    || (!allocator_aware<C> && default_copy_constructable<T>)
    || ( allocator_aware<C> && allocator_copy_constructable<C, typename C::allocator_type, T>));

///  A type `T` that can be constructed in-place with rvalue arguments at a given address `p`.
///  \see https://en.cppreference.com/w/cpp/memory/construct_at
///
template <class T>
concept default_emplace_constructable = requires(T* p, T&& args) {
    std::construct_at(p, std::forward<T>(args));
};

///  A type `T` that can be constructed in-place with rvalue arguments at a given address `p` using
///  an allocator `A`.
///  \see https://en.cppreference.com/w/cpp/memory/allocator_traits
///
template <class C, class A, class T>
concept allocator_emplace_constructable = 
    std::same_as<
        typename C::allocator_type,
        typename std::allocator_traits<A>::template rebind_alloc<T>
    >
    && requires(A m, T* p, T&& args) {
        std::allocator_traits<A>::construct(m, p, std::forward<T>(args));
    };

///  Specifies that an instance of the type `T` can be constructed in-place
///  with rvalue arguments by a given allocator `C::allocator_type`.
///
///  \see https://en.cppreference.com/w/cpp/named_req/EmplaceConstructible
///
template <class C, class T>
concept emplace_constructible = 
       ( is_basic_string<C> && default_emplace_constructable<T>)
    || (!allocator_aware<C> && default_emplace_constructable<T>)
    || ( allocator_aware<C> && allocator_emplace_constructable<C, typename C::allocator_type, T>);

///  A type `T` that can be destroyed at a given address `p`.
///  \see https://en.cppreference.com/w/cpp/memory/destroy_at
///
template <typename T>
concept default_destroyable = requires(T* p) {
    std::destroy_at(p);
};

///  A type `T` that can be destroyed at a given address `p` using
///  an allocator `A`.
///  \see https://en.cppreference.com/w/cpp/memory/allocator_traits
///
template <class C, class A, class T>
concept allocator_destroyable = 
    std::same_as<
        typename C::allocator_type,
        typename std::allocator_traits<A>::template rebind_alloc<T>
    >
    && requires(A m, T* p) {
        std::allocator_traits<A>::destroy(m, p);
    };

///  Specifies that an object of the type `T` can be destroyed by 
///  a given allocator `C::allocator_type`.
///
///  \see https://en.cppreference.com/w/cpp/named_req/Erasable
///
template <class C, class T>
concept erasable = 
       ( is_basic_string<C> && default_destroyable<T>)
    || (!allocator_aware<C> && default_destroyable<T>)
    || ( allocator_aware<C> && allocator_destroyable<C, typename C::allocator_type, T>);

template <class C, class T>
concept container_element = std::same_as<T, typename C::value_type> && erasable<C, T>;

///  \brief A container type.
///  
///  Containers are objects that store other objects. They control allocation
///  and deallocation  of these objects through constructors, destructors,insert and
///  erase operations. ISO/IEC 14882:2020 [container.requirements.general].
///  
///  \see https://en.cppreference.com/w/cpp/named_req/Container
///  
template <class C>
concept container =
    /// Member types.
       container_element<C, typename C::value_type>
    // Reference types.
    && std::same_as<typename C::reference, typename C::value_type&>
    && std::same_as<typename C::const_reference, typename C::value_type const&>
    // Iterator types.
    && std::forward_iterator<typename C::iterator>
    && std::forward_iterator<typename C::const_iterator>
    && std::same_as<std::iter_value_t<typename C::iterator>, typename C::value_type>
    && std::same_as<std::iter_value_t<typename C::const_iterator>, typename C::value_type>
    && std::convertible_to<typename C::iterator, typename C::const_iterator>
    // Difference type.
    && std::signed_integral<typename C::difference_type>
    && std::same_as<
        typename C::difference_type, 
        typename std::iterator_traits<typename C::iterator>::difference_type>
    && std::same_as<
        typename C::difference_type, 
        typename std::iterator_traits<typename C::const_iterator>::difference_type>
    // Size type.
    && std::unsigned_integral<typename C::size_type>
    && (std::in_range<typename C::size_type>(std::numeric_limits<typename C::difference_type>::max()))
    /// Member functions.
    && requires (std::remove_cv_t<C> a, std::remove_cv_t<C> const b) {
        { a.begin() }    -> std::same_as<typename C::iterator>;
        { a.end() }      -> std::same_as<typename C::iterator>;
        { b.begin() }    -> std::same_as<typename C::const_iterator>;
        { b.end() }      -> std::same_as<typename C::const_iterator>;
        { a.cbegin() }   -> std::same_as<typename C::const_iterator>;
        { a.cend() }     -> std::same_as<typename C::const_iterator>;    
        { a.max_size() } -> std::same_as<typename C::size_type>;
        { a.empty() }    -> std::convertible_to<bool>;
    };

///  \brief A reversible container type.
///  
///  If the iterator type of a container belongs to the bidirectional or 
///  random access iterator categories, the container is called reversible.
///  ISO/IEC 14882:2020 [container.requirements.general].
///  
///  \see https://en.cppreference.com/w/cpp/named_req/ReversibleContainer
///  
template <class C>
concept reversible_container = container<C>
    && std::bidirectional_iterator<typename C::iterator>
    && std::bidirectional_iterator<typename C::const_iterator>
    && std::bidirectional_iterator<typename C::reverse_iterator>
    && std::bidirectional_iterator<typename C::const_reverse_iterator>
    && std::convertible_to<typename C::reverse_iterator, typename C::const_reverse_iterator>
    && requires(std::remove_cv_t<C> a, std::remove_cv_t<C> const b) {
        { a.rbegin() }  -> std::same_as<typename C::reverse_iterator>;
        { a.rend() }    -> std::same_as<typename C::reverse_iterator>;
        { b.rbegin() }  -> std::same_as<typename C::const_reverse_iterator>;
        { b.rend() }    -> std::same_as<typename C::const_reverse_iterator>;
        { a.crbegin() } -> std::same_as<typename C::const_reverse_iterator>;
        { a.crend() }   -> std::same_as<typename C::const_reverse_iterator>;
    };

///  \brief A sized container type.
///  
///  A container that has a `size()` accessor. Satisfied by all standard containers
///  except for `std::forward_list`.
///  
///  \see https://en.cppreference.com/w/cpp/container
///  
template <class C>
concept sized_container = container<C>
    && requires(C const a) {
        { a.size() } -> std::same_as<typename C::size_type>;
    };

///  \brief A container that can be cleared.
///
///  Satisfied by all standard containers except for `std::array`.
///
///  \see https://en.cppreference.com/w/cpp/container
///
template <class C>
concept clearable_container = container<C> && requires(C a) { a.clear(); };

///  \brief A container that can be resized.
///
///  Satisfied by `std::vector`, `std::basic_string`, `std::deque`, `std::list` 
///  and `std::forward_list`.
///
///  \see https://en.cppreference.com/w/cpp/container
///
template <class C>
concept resizable_container = sized_container<C>
    && requires(C a, typename C::size_type const n) { a.resize(n); };

///  \brief A container whose capacity can be reserved.
///
///  Satisfied by `std::vector`, `std::basic_string`, and all standard associative 
///  containers.
///
///  \see https://en.cppreference.com/w/cpp/container
///
template <class C>
concept reservable_container = sized_container<C> 
    && requires(C a, typename C::size_type const n) { a.reserve(n); };

///  \brief A container that can be reversed.
///
///  Satisfied only by `std::list` and `std::forward_list`.
///
///  \see https://en.cppreference.com/w/cpp/container
///
template <class C>
concept invertible_container = container<C> && requires(C a) { a.reverse(); };        

///  \brief A random access container type.
///
///  A double-ended container that allows indexed access.
///  Satisfied by `std::array`, `std::vector`, `std::deque` and `std::basic_string`.
///
template <class C>
concept random_access_container = 
       reversible_container<C> && sized_container<C>
    && std::random_access_iterator<typename C::iterator>
    && std::random_access_iterator<typename C::const_iterator>
    && requires(
        std::remove_cv_t<C> a, std::remove_cv_t<C> const b, 
        typename C::size_type const i
    ) {
        { a[i] } -> std::same_as<typename C::reference>;
        { b[i] } -> std::same_as<typename C::const_reference>;
    };

///  \brief A continguous container type.
///
///  A random access container that stores elements in a contiguous memory region.
///  Satisfied by `std::array`, `std::vector` and `std::basic_string`.
///
///  \see https://en.cppreference.com/w/cpp/named_req/ContiguousContainer
///
template <typename C>
concept contiguous_container = 
       random_access_container<C>
    && std::contiguous_iterator<typename C::iterator>
    && std::contiguous_iterator<typename C::const_iterator>
    && std::contiguous_iterator<typename C::pointer>
    && std::contiguous_iterator<typename C::const_pointer>
    && std::convertible_to<typename C::pointer, typename C::const_pointer>
    && requires(std::remove_cv_t<C> a, std::remove_cv_t<C> const b) {
        { a.data() } -> std::same_as<typename C::pointer>;
        { b.data() } -> std::same_as<typename C::const_pointer>;
    };

///  \brief A fixed size container type.
///
///  A contiguous container whose size is known at compile time.
///  Satisfied by `std::array`.
///
template <typename C>
concept fixed_size_container = contiguous_container<C> && 
    std::integral_constant<std::size_t, C{}.size()>::value > 0;

///  \brief A container capable of constructing elements in-place.
///
template <typename C>
concept inplace_constructing_container = container<C>
    && emplace_constructible<C, typename C::value_type>
    && requires (C a, typename C::value_type&& rv) {
        a.emplace();
        a.emplace(std::forward<typename C::value_type>(rv));
    };

///  \brief A container capable of constructing elements in-place at the back 
///  of its storage.
template <typename C>
concept back_inplace_constructing_container = container<C>
    && emplace_constructible<C, typename C::value_type>
    && requires (C a, typename C::value_type&& rv) {
        { a.emplace_back() } 
            -> std::same_as<typename C::reference>;            
        { a.emplace_back(std::forward<typename C::value_type>(rv)) } 
            -> std::same_as<typename C::reference>;
    };

///  \brief A container capable of constructing elements in-place at the front 
///  of its storage.
template <typename C>
concept front_inplace_constructing_container = container<C>
    && emplace_constructible<C, typename C::value_type>
    && requires (C a, typename C::value_type&& rv) {
        { a.emplace_front() } 
            -> std::same_as<typename C::reference>;
        { a.emplace_front(std::forward<typename C::value_type>(rv)) } 
            -> std::same_as<typename C::reference>;
    };

///  \brief A container capable of constructing elements in-place in an 
///  iterator range.
template <typename C>
concept range_constructing_container = container<C>
    && emplace_constructible<C, typename C::value_type>
    && requires (typename C::iterator i, typename C::iterator j) {
        C { i, j };
    };

///  \brief An associative container type.
///  
///  Associative containers provide fast retrieval of data based on keys.
///  The standard library provides four basic kinds of associative containers: 
///  set, multiset, map and multimap. ISO/IEC 14882:2020 [associative.reqmts].
///  
///  \see https://en.cppreference.com/w/cpp/named_req/AssociativeContainer
///  
template <typename C>
concept associative_container = 
       sized_container<C> 
    && clearable_container<C>
    && inplace_constructing_container<C> 
    && requires { 
        typename C::key_type; 
    };

///  \brief A set container type.
///  \see https://en.cppreference.com/w/cpp/container/set
///
template <typename C>
concept set_container = associative_container<C> 
    && std::same_as<typename C::key_type, typename C::value_type>;

///  \brief A map container type.
///  \see https://en.cppreference.com/w/cpp/container/map
///
template <typename C>
concept map_container = associative_container<C> && requires { typename C::mapped_type; };

} // namespace teg::concepts

#endif // TEG_CONTAINER_CONCEPTS_H
