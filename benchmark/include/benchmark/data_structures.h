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
// 2. Altered source versions must be plainly marked as such, and mu    st not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once
#include <concepts>
#include <cstddef>
#include <string>
#include <cstdint>
#include <vector>

namespace benchmarking {

enum class color {
    red, green, blue
};

struct vec3 {
    float x, y, z;
    constexpr bool operator==(const vec3&) const = default;
};

struct weapon {
    std::string name;
    std::int16_t damage;
    color color;
    constexpr bool operator==(const weapon&) const = default;
};

struct monster {
    std::string name;    
    color color;
    std::int16_t health;
    std::int16_t mana;
    vec3 position;
    std::vector<vec3> path;
    weapon equipped_weapon;
    std::vector<weapon> weapons;
    std::vector<std::uint8_t> inventory;
    constexpr bool operator==(const monster&) const = default;
};

} // namespace benchmarking