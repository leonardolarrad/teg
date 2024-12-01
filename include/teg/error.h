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

#ifndef TEG_ERROR_H
#define TEG_ERROR_H

#include "teg/def.h"

namespace teg {

struct error {
    std::errc code;

    constexpr error(std::errc code = {}) noexcept 
        : code(code) {}

    constexpr operator std::errc() const noexcept { 
        return code; 
    }

    constexpr void or_throw() const {
        if (code != std::errc{}) TEG_UNLIKELY {
        #ifdef __cpp_exceptions
            throw std::system_error(std::make_error_code(code));
        #else
            std::abort();
        #endif
        }
    }
};

constexpr bool success(error err) {
    return err.code == std::errc{};
}

constexpr bool failure(error err) {
    return err.code != std::errc{};
}

} // namespace teg

#endif // TEG_ERROR_H