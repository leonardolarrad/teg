###  Copyright (c) 2024 Adrian & Leonardo Larrad.
###  
###  This software is provided 'as-is', without any express or implied
###  warranty. In no event will the authors be held liable for any damages
###  arising from the use of this software.
###  
###  Permission is granted to anyone to use this software for any purpose,
###  including commercial applications, and to alter it and redistribute it
###  freely, subject to the following restrictions:
###  
###  1. The origin of this software must not be misrepresented; you must not
###     claim that you wrote the original software. If you use this software
###     in a product, an acknowledgment in the product documentation would be
###     appreciated but is not required.
###  2. Altered source versions must be plainly marked as such, and must not be
###     misrepresented as being the original software.
###  3. This notice may not be removed or altered from any source distribution.

def generate_arguments(n: int) -> str:
    variables = [f'_{str(i).zfill(3)}' for i in range(1, n+1)]
    formatted_bindings = ""

    for idx, var in enumerate(variables):
        if idx > 0 and idx % 20 == 0:
            formatted_bindings += "\n            "
        formatted_bindings += var
    
        if idx < len(variables) - 1:
            formatted_bindings += ", "
    
    return formatted_bindings

def generate_all_functions(n: int) -> str:
    functions = ""

    for i in range(1, n+1):
        functions += generate_function_impl(i)

    return functions

def generate_function_impl(n: int) -> str:
    function_template = \
f"""template <class F, class T>
constexpr decltype(auto) visit_members_impl(F&& f, T&& t, std::integral_constant<std::size_t, {n}>) {{
    auto&& [{generate_arguments(n)}] = t;
    return f({generate_arguments(n)});
}}

"""
    return function_template

def generate_header(n: int) -> str:
    header = \
f"""///  Copyright (c) 2024 Adrian & Leonardo Larrad.
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

#ifndef TEG_MEMBERS_VISITOR_H
#define TEG_MEMBERS_VISITOR_H

#include "teg/members_count.h"

namespace teg::internal {{

{generate_all_functions(n)}}} // namespace teg::internal

namespace teg {{

///  \\brief The maximum number of members that can be visited using `visit_members`.
///  
constexpr auto max_visit_members = {n};

namespace concepts {{

///  \\brief A type that can be bound to an identifier list using structured binding.
///  
///  Structured binding allows an object to be bound to a list of identifiers.
///  In other languagues, this mechanism is referred to as "destructuring" or
///  "unpacking".
///  
///  \\example
///  \\code
///     auto [a, b, c] = std::make_tuple(1, 2, 3);
///     a = 0;
///  \\endcode
///  \\see https://en.cppreference.com/w/cpp/language/structured_binding
///  
template <class T>
concept accesible_aggregate = 
    aggregate<T> && members_count_v<std::remove_cvref_t<T>> <= max_visit_members;

}} // namespace concepts

/// \\brief Invoke the callable object `f` with the members of `t` as arguments.
/// \\param   f  A callable object.
/// \\param   t  An object that can be bound to an identifier list using structured binding.
/// \\return  The return value of `f(m1, m2, ..., mN)` where `m1, m2, ..., mN` are the members of `t`.
///  
///  \\example
///  \\code
///     struct S {{
///         int a, b, c;
///     }};
///     constexpr auto s = S{{ 1, 2, 3 }};
///     constexpr int r = visit_members(
///        [](auto&&... members) {{ return (members + ...); 
///     }}, s);
///     static_assert(r == 6);
///  \\endcode
///
///  \\note It works similar to `std::apply` for tuple-like types.
///  \\see https://en.cppreference.com/w/cpp/utility/apply
///  \\see https://en.cppreference.com/w/cpp/language/structured_binding
///  
template<class F, class T> 
    requires (concepts::accesible_aggregate<std::remove_cvref_t<T>>)
TEG_INLINE constexpr decltype(auto) visit_members(F&& f, T&& t) noexcept {{
    if constexpr (teg::concepts::tuple<std::remove_cvref_t<T>>) {{
        return internal::visit_members_impl(
            std::forward<F>(f),
            std::forward<T>(t), 
            std::integral_constant<std::size_t, std::tuple_size_v<std::remove_cvref_t<T>>>{{}}   
        );
    }}
    else {{
        return internal::visit_members_impl(
            std::forward<F>(f),
            std::forward<T>(t), 
            std::integral_constant<std::size_t, members_count_v<std::remove_cvref_t<T>>>{{}}        
        );
    }}
}}

}} // namespace teg

#endif // TEG_MEMBERS_VISITOR_H"""
    return header