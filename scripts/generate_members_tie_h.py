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
            formatted_bindings += "\n           "
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
f"""template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, {n}>) {{
    auto& [{generate_arguments(n)}] = t;
    return std::tie({generate_arguments(n)});
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

#ifndef TEG_MEMBERS_TIE_H
#define TEG_MEMBERS_TIE_H

#include "teg/members_visitor.h"

namespace teg::internal {{

{generate_all_functions(n)}
}} // namespace teg::internal

namespace teg {{

///  \\brief Creates a tuple of lvalua references to the members of a given aggregate.
///  \\tparam T An aggregate type.
///  
template <class T>
    requires concepts::accesible_aggregate<std::remove_cvref_t<T>>
TEG_INLINE constexpr auto tie_members(T& t) -> decltype(auto) {{
    return internal::tie_members_impl(
        t, 
        std::integral_constant<std::size_t, members_count_v<T>>()
    ); 
}}

}} // namespace teg

#endif // TEG_MEMBERS_TIE_H"""
    return header