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

import generate_members_tie_h
import generate_members_visitor_h
import argparse

def write_to_file(filename, content):
    """Write the generated code to a file."""
    with open(filename, 'w') as file:
        file.write(content)

if __name__ == '__main__':
    # Parse command line arguments.
    parser = argparse.ArgumentParser("generate_headers")
    parser.add_argument('--n', help="number of functions to generate", type=int, default=64)
    args = parser.parse_args()

    members_tie_h = generate_members_tie_h.generate_header(args.n)
    members_visitor_h = generate_members_visitor_h.generate_header(args.n)
    
    write_to_file("members_tie.h.gen", members_tie_h)
    write_to_file("members_visitor.h.gen", members_visitor_h)