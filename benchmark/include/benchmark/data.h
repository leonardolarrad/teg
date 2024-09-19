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
#include <vector>
#include <string>
#include <cstdint>
#include <random>

namespace benchmarking {

struct ecommerce_user {
    std::uint64_t uuid;    
    std::string name;
    std::string email;
    std::vector<std::string> recent_searches;
};

enum class ecommerce_product_category {
    electronics,
    books,
    clothing,
    home,
    garden,
    toys,
    food,
    baby,
    pets,
    health,
    beauty
};

struct ecommerce_product {
    std::uint64_t uuid;
    std::string name;
    std::string description;
    ecommerce_product_category category;
    std::vector<std::string> tags;
    std::string image_lo_res_url;
    std::string image_hi_res_url;
    double price;
    double discount;
    std::uint32_t stock;
    std::uint8_t rating;
    std::uint32_t reviews;
};

struct ecommerce_search_result {
    ecommerce_user user;    
    std::string permanent_url;
    std::string query;
    std::uint32_t page;
    std::uint32_t total_pages;
    std::uint32_t results_per_page;
    std::uint32_t total_results;
    std::vector<ecommerce_product> products;
};

template <typename T>
concept random_number_engine = requires(T engine, std::seed_seq seed) {
    { engine() } -> std::convertible_to<typename T::result_type>;
    engine.seed();
    engine.seed(seed);
};

std::uint64_t random_unsigned_integer(random_number_engine auto& rng) {
    std::uniform_int_distribution<std::uint64_t> dist;
    return dist(rng);
}

std::int64_t random_signed_integer(random_number_engine auto& rng) {
    std::uniform_int_distribution<std::int64_t> dist;
    return dist(rng);
}

std::string random_string(random_number_engine auto& rng, std::size_t bytes) {
    static char charset[] = "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";
    static auto dist = std::uniform_int_distribution<std::string::size_type>(0, std::strlen(charset) - 1);
    
    std::string str;

    if (bytes == 0) {
        return str;
    }

    str.reserve(bytes);
    std::generate_n(std::back_inserter(str), bytes, [&] { return charset[dist(rng)]; });
    return str;
}

ecommerce_user random_user_data(random_number_engine auto& rng, std::size_t bytes) {    
    bytes = std::max(bytes, std::size_t(256));
    bytes -= sizeof(std::uint64_t);
    bytes -= sizeof(std::vector<std::string>::size_type)*1;
    bytes -= sizeof(std::string::size_type)*5;
    std::size_t remaining_bytes = bytes / 5;
    std::size_t additional_bytes = bytes % 5;

    ecommerce_user user;
    user.uuid = (std::uint64_t) random_unsigned_integer(rng);
    user.name = random_string(rng, remaining_bytes);
    user.email = random_string(rng, remaining_bytes);
    user.recent_searches.emplace_back(random_string(rng, remaining_bytes));
    user.recent_searches.emplace_back(random_string(rng, remaining_bytes));
    user.recent_searches.emplace_back(random_string(rng, remaining_bytes + additional_bytes));

    return user;
}


} // namespace benchmarking