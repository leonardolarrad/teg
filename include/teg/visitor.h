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
#include <variant>
#include <concepts>

#include "member_count.h"
#include "concepts.h"

namespace teg {

constexpr static auto MAX_VISIT_MEMBERS = 64;

template<typename O, typename V>
constexpr decltype(auto) inline visit_members(O&& obj, V&& visitor) {
    using type = std::remove_cvref_t<O>;

    constexpr auto members_count = teg::members_count<type>;
    if constexpr (members_count == 0 && std::is_class_v<type> 
        && !std::is_same_v<type, std::monostate>) {
        static_assert(!sizeof(type), "Unsupported type");   
    }
    static_assert(members_count <= MAX_VISIT_MEMBERS, "Exceed max visit members");

    if constexpr (members_count == 0) {
        return visitor();
    }
    else if constexpr (members_count == 1) {
        auto &&[e1] = obj;
        return visitor(e1);
    }
    else if constexpr (members_count == 2) {
        auto &&[e1, e2] = obj;
        return visitor(e1, e2);
    }
    else if constexpr (members_count == 3) {
        auto &&[e1, e2, e3] = obj;
        return visitor(e1, e2, e3);
    }
    else if constexpr (members_count == 4) {
        auto &&[e1, e2, e3, e4] = obj;
        return visitor(e1, e2, e3, e4);
    }
    else if constexpr (members_count == 5) {
        auto &&[e1, e2, e3, e4, e5] = obj;
        return visitor(e1, e2, e3, e4, e5);
    }
    else if constexpr (members_count == 6) {
        auto &&[e1, e2, e3, e4, e5, e6] = obj;
        return visitor(e1, e2, e3, e4, e5, e6);
    }
    else if constexpr (members_count == 7) {
        auto &&[e1, e2, e3, e4, e5, e6, e7] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7);
    }
    else if constexpr (members_count == 8) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8);
    }
    else if constexpr (members_count == 9) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9);
    }
    else if constexpr (members_count == 10) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10);
    }
    else if constexpr (members_count == 11) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11);
    }
    else if constexpr (members_count == 12) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12);
    }
    else if constexpr (members_count == 13) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13);
    }
    else if constexpr (members_count == 14) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14] =
            obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14);
    }
    else if constexpr (members_count == 15) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14,
                e15] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15);
    }
    else if constexpr (members_count == 16) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16);
    }
    else if constexpr (members_count == 17) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17);
    }
    else if constexpr (members_count == 18) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18);
    }
    else if constexpr (members_count == 19) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19);
    }
    else if constexpr (members_count == 20) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20);
    }
    else if constexpr (members_count == 21) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21);
    }
    else if constexpr (members_count == 22) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22);
    }
    else if constexpr (members_count == 23) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23);
    }
    else if constexpr (members_count == 24) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24);
    }
    else if constexpr (members_count == 25) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24,
                        e25);
    }
    else if constexpr (members_count == 26) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26);
    }
    else if constexpr (members_count == 27) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27] =
            obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27);
    }
    else if constexpr (members_count == 28) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28] =
            obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28);
    }
    else if constexpr (members_count == 29) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29);
    }
    else if constexpr (members_count == 30) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30);
    }
    else if constexpr (members_count == 31) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31);
    }
    else if constexpr (members_count == 32) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32);
    }
    else if constexpr (members_count == 33) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33);
    }
    else if constexpr (members_count == 34) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34);
    }
    else if constexpr (members_count == 35) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35);
    }
    else if constexpr (members_count == 36) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36);
    }
    else if constexpr (members_count == 37) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36,
                        e37);
    }
    else if constexpr (members_count == 38) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38);
    }
    else if constexpr (members_count == 39) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39);
    }
    else if constexpr (members_count == 40) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40] =
            obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40);
    }
    else if constexpr (members_count == 41) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41] =
            obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41);
    }
    else if constexpr (members_count == 42) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42);
    }
    else if constexpr (members_count == 43) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43);
    }
    else if constexpr (members_count == 44) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44);
    }
    else if constexpr (members_count == 45) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45);
    }
    else if constexpr (members_count == 46) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46);
    }
    else if constexpr (members_count == 47) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47);
    }
    else if constexpr (members_count == 48) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48);
    }
    else if constexpr (members_count == 49) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48,
                        e49);
    }
    else if constexpr (members_count == 50) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50);
    }
    else if constexpr (members_count == 51) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51);
    }
    else if constexpr (members_count == 52) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52);
    }
    else if constexpr (members_count == 53) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53] =
            obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53);
    }
    else if constexpr (members_count == 54) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54] =
            obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54);
    }
    else if constexpr (members_count == 55) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55);
    }
    else if constexpr (members_count == 56) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56);
    }
    else if constexpr (members_count == 57) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56, e57] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56, e57);
    }
    else if constexpr (members_count == 58) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56, e57, e58] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56, e57, e58);
    }
    else if constexpr (members_count == 59) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56, e57, e58, e59] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56, e57, e58, e59);
    }
    else if constexpr (members_count == 60) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56, e57, e58, e59, e60] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60);
    }
    else if constexpr (members_count == 61) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56, e57, e58, e59, e60, e61] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60,
                        e61);
    }
    else if constexpr (members_count == 62) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56, e57, e58, e59, e60, e61, e62] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61,
                        e62);
    }
    else if constexpr (members_count == 63) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56, e57, e58, e59, e60, e61, e62, e63] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61,
                        e62, e63);
    }
    else if constexpr (members_count == 64) {
        auto &&[e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,
                e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28,
                e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41,
                e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54,
                e55, e56, e57, e58, e59, e60, e61, e62, e63, e64] = obj;
        return visitor(e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13,
                        e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25,
                        e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37,
                        e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
                        e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61,
                        e62, e63, e64);
    }
}

} // namespace teg