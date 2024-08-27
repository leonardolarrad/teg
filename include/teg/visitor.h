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
        auto &&[obj1] = obj;
        return visitor(obj1);
    }
    else if constexpr (members_count == 2) {
        auto &&[obj1, obj2] = obj;
        return visitor(obj1, obj2);
    }
    else if constexpr (members_count == 3) {
        auto &&[obj1, obj2, obj3] = obj;
        return visitor(obj1, obj2, obj3);
    }
    else if constexpr (members_count == 4) {
        auto &&[obj1, obj2, obj3, obj4] = obj;
        return visitor(obj1, obj2, obj3, obj4);
    }
    else if constexpr (members_count == 5) {
        auto &&[obj1, obj2, obj3, obj4, obj5] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5);
    }
    else if constexpr (members_count == 6) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6);
    }
    else if constexpr (members_count == 7) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7);
    }
    else if constexpr (members_count == 8) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8);
    }
    else if constexpr (members_count == 9) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9);
    }
    else if constexpr (members_count == 10) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10);
    }
    else if constexpr (members_count == 11) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11);
    }
    else if constexpr (members_count == 12) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12);
    }
    else if constexpr (members_count == 13) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13);
    }
    else if constexpr (members_count == 14) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14] =
            obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14);
    }
    else if constexpr (members_count == 15) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14,
                obj15] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15);
    }
    else if constexpr (members_count == 16) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16);
    }
    else if constexpr (members_count == 17) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17);
    }
    else if constexpr (members_count == 18) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18);
    }
    else if constexpr (members_count == 19) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19);
    }
    else if constexpr (members_count == 20) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20);
    }
    else if constexpr (members_count == 21) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21);
    }
    else if constexpr (members_count == 22) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22);
    }
    else if constexpr (members_count == 23) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23);
    }
    else if constexpr (members_count == 24) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24);
    }
    else if constexpr (members_count == 25) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24,
                        obj25);
    }
    else if constexpr (members_count == 26) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26);
    }
    else if constexpr (members_count == 27) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27] =
            obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27);
    }
    else if constexpr (members_count == 28) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28] =
            obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28);
    }
    else if constexpr (members_count == 29) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29);
    }
    else if constexpr (members_count == 30) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30);
    }
    else if constexpr (members_count == 31) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31);
    }
    else if constexpr (members_count == 32) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32);
    }
    else if constexpr (members_count == 33) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33);
    }
    else if constexpr (members_count == 34) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34);
    }
    else if constexpr (members_count == 35) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35);
    }
    else if constexpr (members_count == 36) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36);
    }
    else if constexpr (members_count == 37) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36,
                        obj37);
    }
    else if constexpr (members_count == 38) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38);
    }
    else if constexpr (members_count == 39) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39);
    }
    else if constexpr (members_count == 40) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40] =
            obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40);
    }
    else if constexpr (members_count == 41) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41] =
            obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41);
    }
    else if constexpr (members_count == 42) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42);
    }
    else if constexpr (members_count == 43) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43);
    }
    else if constexpr (members_count == 44) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44);
    }
    else if constexpr (members_count == 45) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45);
    }
    else if constexpr (members_count == 46) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46);
    }
    else if constexpr (members_count == 47) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47);
    }
    else if constexpr (members_count == 48) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48);
    }
    else if constexpr (members_count == 49) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48,
                        obj49);
    }
    else if constexpr (members_count == 50) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50);
    }
    else if constexpr (members_count == 51) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51);
    }
    else if constexpr (members_count == 52) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52);
    }
    else if constexpr (members_count == 53) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53] =
            obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53);
    }
    else if constexpr (members_count == 54) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54] =
            obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54);
    }
    else if constexpr (members_count == 55) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55);
    }
    else if constexpr (members_count == 56) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56);
    }
    else if constexpr (members_count == 57) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56, obj57] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56, obj57);
    }
    else if constexpr (members_count == 58) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56, obj57, obj58] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56, obj57, obj58);
    }
    else if constexpr (members_count == 59) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56, obj57, obj58, obj59] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56, obj57, obj58, obj59);
    }
    else if constexpr (members_count == 60) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56, obj57, obj58, obj59, obj60] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56, obj57, obj58, obj59, obj60);
    }
    else if constexpr (members_count == 61) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56, obj57, obj58, obj59, obj60, obj61] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56, obj57, obj58, obj59, obj60,
                        obj61);
    }
    else if constexpr (members_count == 62) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56, obj57, obj58, obj59, obj60, obj61, obj62] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56, obj57, obj58, obj59, obj60, obj61,
                        obj62);
    }
    else if constexpr (members_count == 63) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56, obj57, obj58, obj59, obj60, obj61, obj62, obj63] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56, obj57, obj58, obj59, obj60, obj61,
                        obj62, obj63);
    }
    else if constexpr (members_count == 64) {
        auto &&[obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13, obj14, obj15,
                obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25, obj26, obj27, obj28,
                obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37, obj38, obj39, obj40, obj41,
                obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49, obj50, obj51, obj52, obj53, obj54,
                obj55, obj56, obj57, obj58, obj59, obj60, obj61, obj62, obj63, obj64] = obj;
        return visitor(obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10, obj11, obj12, obj13,
                        obj14, obj15, obj16, obj17, obj18, obj19, obj20, obj21, obj22, obj23, obj24, obj25,
                        obj26, obj27, obj28, obj29, obj30, obj31, obj32, obj33, obj34, obj35, obj36, obj37,
                        obj38, obj39, obj40, obj41, obj42, obj43, obj44, obj45, obj46, obj47, obj48, obj49,
                        obj50, obj51, obj52, obj53, obj54, obj55, obj56, obj57, obj58, obj59, obj60, obj61,
                        obj62, obj63, obj64);
    }
}

} // namespace teg