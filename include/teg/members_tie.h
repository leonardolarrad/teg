#ifndef TEG_MEMBERS_TIE_H
#define TEG_MEMBERS_TIE_H

#include <tuple>
#include <type_traits>
#include <utility>

#include "teg/def.h"
#include "teg/members_count.h"

namespace teg {


} // namespace teg

namespace teg::internal {

template <class... A>
constexpr auto tie(A&&... args) noexcept {
    return std::tuple<A&...>{ args... };
}

template<typename T, typename Arg>
constexpr decltype(auto) add_cv_like(Arg& arg) noexcept {
    if constexpr (std::is_const<T>::value && std::is_volatile<T>::value) {
        return const_cast<const volatile Arg&>(arg);
    }
    else if constexpr (std::is_const<T>::value) {
        return const_cast<const Arg&>(arg);
    }
    else if constexpr (std::is_volatile<T>::value) {
        return const_cast<volatile Arg&>(arg);
    }
    else {
        return const_cast<Arg&>(arg);
    }
}

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=78939
template<typename T, typename Sb, typename Arg>
constexpr decltype(auto) as_const(Arg& arg) noexcept {
    using output_arg_t = std::conditional_t<!std::is_reference<Sb>(), decltype(add_cv_like<T>(arg)), Sb>;
    return const_cast<output_arg_t>(arg);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 1>) {
    auto& [_001] = t;
    return std::tie(_001);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 2>) {
    auto& [_001, _002] = const_cast<std::remove_cv_t<T>&>(t);
    return tie(as_const<T, decltype(_001)>(_001), as_const<T, decltype(_002)>(_002));
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 3>) {
    auto& [_001, _002, _003] = t;
    return std::tie(_001, _002, _003);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 4>) {
    auto& [_001, _002, _003, _004] = t;
    return std::tie(_001, _002, _003, _004);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 5>) {
    auto& [_001, _002, _003, _004, _005] = t;
    return std::tie(_001, _002, _003, _004, _005);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 6>) {
    auto& [_001, _002, _003, _004, _005, _006] = t;
    return std::tie(_001, _002, _003, _004, _005, _006);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 7>) {
    auto& [_001, _002, _003, _004, _005, _006, _007] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 8>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 9>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 10>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010);
}

} // namespace teg::internal

namespace teg {

template <typename T>
teg_inline constexpr auto tie_members(T& t) -> decltype(auto) {
    return internal::tie_members_impl(
        t, 
        std::integral_constant<std::size_t, members_count_v<T>>()
    ); 
}


} // namespace teg

#endif // TEG_MEMBERS_TIE_H
