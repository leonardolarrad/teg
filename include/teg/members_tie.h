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

#ifndef TEG_MEMBERS_TIE_H
#define TEG_MEMBERS_TIE_H

#include "teg/members_visitor.h"

namespace teg::internal {

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 1>) {
    auto& [_001] = t;
    return std::tie(_001);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 2>) {
    auto& [_001, _002] = t;
    return std::tie(_001, _002);
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

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 11>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 12>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 13>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 14>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 15>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 16>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 17>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 18>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 19>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 20>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 21>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 22>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 23>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 24>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 25>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 26>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 27>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 28>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 29>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 30>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 31>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 32>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 33>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 34>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 35>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 36>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 37>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 38>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 39>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 40>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 41>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 42>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 43>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 44>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 45>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 46>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 47>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 48>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 49>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 50>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 51>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 52>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 53>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 54>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 55>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 56>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 57>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 58>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 59>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 60>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 61>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060, 
           _061] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060, 
           _061);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 62>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060, 
           _061, _062] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060, 
           _061, _062);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 63>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060, 
           _061, _062, _063] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060, 
           _061, _062, _063);
}

template <class T>
constexpr decltype(auto) tie_members_impl(T& t, std::integral_constant<std::size_t, 64>) {
    auto& [_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060, 
           _061, _062, _063, _064] = t;
    return std::tie(_001, _002, _003, _004, _005, _006, _007, _008, _009, _010, _011, _012, _013, _014, _015, _016, _017, _018, _019, _020, 
           _021, _022, _023, _024, _025, _026, _027, _028, _029, _030, _031, _032, _033, _034, _035, _036, _037, _038, _039, _040, 
           _041, _042, _043, _044, _045, _046, _047, _048, _049, _050, _051, _052, _053, _054, _055, _056, _057, _058, _059, _060, 
           _061, _062, _063, _064);
}


} // namespace teg::internal

namespace teg {

///  \brief Creates a tuple of lvalua references to the members of a given aggregate.
///  \tparam T An aggregate type.
///  
template <class T>
    requires concepts::accesible_aggregate<std::remove_cvref_t<T>>
TEG_INLINE constexpr auto tie_members(T& t) -> decltype(auto) {
    return internal::tie_members_impl(
        t, 
        std::integral_constant<std::size_t, members_count_v<T>>()
    ); 
}

} // namespace teg

#endif // TEG_MEMBERS_TIE_H