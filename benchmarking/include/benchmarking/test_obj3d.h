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

#pragma once
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <random>

namespace benchmarking::test_obj3d {

struct ivec3 {
    int64_t x, y, z;
    constexpr bool operator==(ivec3 const&) const = default;
};

struct fvec3 {
    double x, y, z;
    constexpr bool operator==(fvec3 const&) const = default;
};

using vertex = fvec3;
using normal = fvec3;

struct face {
    ivec3 vertex_index;
    ivec3 normal_index;   
    constexpr bool operator==(face const&) const = default;
};

struct obj_3d {
    std::vector<vertex> vertices;
    std::vector<normal> normals;
    std::vector<face> faces;
    constexpr bool operator==(obj_3d const&) const = default;    
};

template <typename T>
concept random_number_engine = requires(T engine, std::seed_seq seed) {
    { engine() } -> std::convertible_to<typename T::result_type>;
    engine.seed();
    engine.seed(seed);
};

double random_real(random_number_engine auto& rng) {
    std::uniform_real_distribution<double> dist;
    return dist(rng);
}

std::uint64_t random_unsigned_integer(random_number_engine auto& rng) {
    std::uniform_int_distribution<std::uint64_t> dist;
    return dist(rng);
}

std::int64_t random_signed_integer(random_number_engine auto& rng) {
    std::uniform_int_distribution<std::int16_t> dist;
    return dist(rng);
}

std::vector<vertex> random_vertices(random_number_engine auto& rng, std::size_t bytes) {
    bytes = std::max(bytes, sizeof(vertex));
    bytes -= sizeof(std::vector<vertex>::size_type);
    
    auto vertex_count = bytes / sizeof(vertex);
    std::vector<vertex> vertices;
    vertices.reserve(vertex_count);
    for (std::size_t i = 0; i < vertex_count; ++i) {
        vertices.emplace_back(vertex{random_real(rng), random_real(rng), random_real(rng)});
    }
    return vertices;
}

face random_face(random_number_engine auto& rng) {
    return face{ivec3{random_signed_integer(rng), random_signed_integer(rng), random_signed_integer(rng)},
                ivec3{random_signed_integer(rng), random_signed_integer(rng), random_signed_integer(rng)}};
}

std::vector<face> random_faces(random_number_engine auto& rng, std::size_t bytes) {
    bytes = std::max(bytes, sizeof(face));
    bytes -= sizeof(std::vector<face>::size_type);
    
    const auto face_count = bytes / sizeof(face);
    std::vector<face> faces;
    faces.reserve(face_count);
    for (std::size_t i = 0; i < face_count; ++i) {
        faces.emplace_back(random_face(rng));       
    }
    return faces;
}


auto generate_benchmark_data(std::size_t bytes) -> obj_3d {
    obj_3d test_data {};

    std::seed_seq seed{99, 5, 11};
    std::mt19937_64 engine {seed};

    const auto vert_bytes = bytes * (2.f/10.f);
    const auto norm_bytes = bytes * (3.f/10.f);
    const auto face_bytes = bytes * (5.f/10.f);
    const auto remaining = bytes - (vert_bytes + norm_bytes + face_bytes);

    return obj_3d{
        random_vertices(engine, vert_bytes),
        random_vertices(engine, norm_bytes),
        random_faces(engine, face_bytes + remaining)
    };
}

} // namespace benchmarking::obj_3d