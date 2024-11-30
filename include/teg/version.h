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

struct version {
    u8 major;
    u8 minor;
    u8 patch;

    constexpr bool operator==(version const&) const = default;
};

TEG_NODISCARD TEG_INLINE 
constexpr auto get_version() -> version {
    return TEG_VERSION;
}

struct magic_word {
    char id[3];
    u8 version;

    constexpr bool operator==(magic_word const&) const = default;
};

TEG_NODISCARD TEG_INLINE 
constexpr auto get_magic_word() -> magic_word {
    return { {'T', 'E', 'G'}, get_version().major };    
} 

} // namespace teg

#endif // TEG_VERSION_H
