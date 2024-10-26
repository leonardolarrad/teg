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

import argparse

def generate_function(i):
    """ Generate the function implementation `visit_members` for `i` members:
        
        template <class F, class T>
        constexpr inline decltype(auto) visit_members_impl(F&& f, T&& t, std::integral_constant<std::size_t, 3>) {
            auto&& [_001, _002, _003] = std::forward<T>(t);
            return std::forward<F>(f)(
                std::forward<decltype(_001)>(_001), std::forward<decltype(_002)>(_002), std::forward<decltype(_003)>(_003));
        }
    """

    function_template = \
f"""template <class F, class T>
constexpr inline decltype(auto) visit_members_impl(F&& f, T&& t, std::integral_constant<std::size_t, {i}>) {{
    auto&& [{format_bindings(i)}] = std::forward<T>(t);
    
    return std::forward<F>(f)(
            {format_forward_arguments(i)});
}}
"""
    return function_template

def format_bindings(i):
    """Format the bindings for `i` members.
       Eg. _001, _002, _003, ...
    """
    variables = [f'_{str(j).zfill(3)}' for j in range(1, i+1)]
    formatted_bindings = ""

    for idx, var in enumerate(variables):
        if idx > 0 and idx % 20 == 0:
            formatted_bindings += "\n            "
        formatted_bindings += var
    
        if idx < len(variables) - 1:
            formatted_bindings += ", "
    
    return formatted_bindings

def format_forward_arguments(i):
    """Format the forward arguments for `i` members.
       Eg. _001, _002, _003, ...
    """

    arguments = [f'_{str(j).zfill(3)}' for j in range(1, i+1)]
    formatted_arguments = ""
    
    for idx, arg in enumerate(arguments):
        if idx > 0 and idx % 20 == 0:
            formatted_arguments += "\n            "
        formatted_arguments += arg
    
        if idx < len(arguments) - 1:
            formatted_arguments += ", "
    
    return formatted_arguments

def generate_functions_for_range(start, end):
    """Generate the functions for `start` to `end`."""
    
    functions = ""
    for i in range(start, end + 1):
        functions += generate_function(i) + "\n"
    
    return functions

def write_to_file(filename, content):
    """Write the generated code to a file."""
    with open(filename, 'w') as file:
        file.write(content)

if __name__ == '__main__':
    # Parse command line arguments.
    parser = argparse.ArgumentParser("gen_members_visitor")
    parser.add_argument('n', help="number of functions to generate", type=int, default=64)
    parser.add_argument('--file', help="file to write the generated code to", type=str, default="generated.tmp")
    args = parser.parse_args()

    # Generate the code.
    generated_code = generate_functions_for_range(1, args.n)

    # Write the generated code to a file.
    write_to_file(args.file, generated_code)