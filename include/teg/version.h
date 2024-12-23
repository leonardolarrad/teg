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

#ifndef TEG_VERSION_H
#define TEG_VERSION_H

#include "def.h"

namespace teg {

TEG_NODISCARD TEG_INLINE 
constexpr auto version() -> std::tuple<u8, u8, u8> {
    return TEG_VERSION;
}   

TEG_NODISCARD TEG_INLINE 
constexpr auto magic_word() -> u32 {

    u8 magic_word[4];
    magic_word[0] = 'T';
    magic_word[1] = 'E';
    magic_word[2] = 'G';
    magic_word[3] = TEG_VERSION_MAJOR;

    return ((u32)magic_word[0] <<  0)
         | ((u32)magic_word[1] <<  8) 
         | ((u32)magic_word[2] << 16) 
         | ((u32)magic_word[3] << 24);
         
}

} // namespace teg

#endif // TEG_VERSION_H
