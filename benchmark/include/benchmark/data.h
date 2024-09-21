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

    constexpr bool operator==(ecommerce_user const&) const = default;
};

enum ecommerce_product_category {
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

    constexpr bool operator==(ecommerce_product const&) const = default;
};

struct ecommerce_page {
    ecommerce_user user;    
    std::string permanent_url;
    std::string query;
    std::uint32_t page;
    std::uint32_t total_pages;
    std::uint32_t results_per_page;
    std::uint32_t total_results;
    std::vector<ecommerce_product> products;

    constexpr bool operator==(ecommerce_page const&) const = default;
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
    std::uniform_int_distribution<std::int64_t> dist;
    return dist(rng);
}

std::string random_string(random_number_engine auto& rng, std::size_t bytes) {
    static char charset[] = "0123456789" "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz";
    static auto dist = std::uniform_int_distribution<std::string::size_type>(0, std::strlen(charset) - 1);

    bytes = std::max(std::size_t(0), bytes - sizeof(std::string::size_type));
    
    std::string str;
    if (bytes == 0) {
        return str;
    }

    str.reserve(bytes);
    std::generate_n(std::back_inserter(str), bytes, [&] { return charset[dist(rng)]; });
    return str;
}

ecommerce_user random_user_data(random_number_engine auto& rng, std::size_t bytes) {    
    bytes = std::max(bytes, std::size_t(64));
    bytes -= sizeof(std::uint64_t);
    bytes -= sizeof(std::vector<std::string>::size_type);    
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

ecommerce_product_category random_product_category(random_number_engine auto& rng) {
    auto dist = std::uniform_int_distribution<int>(0, 10);
    return (ecommerce_product_category)dist(rng);
}

ecommerce_product random_product_data(random_number_engine auto& rng, std::size_t bytes) {
    bytes = std::max(bytes, std::size_t(128));
    bytes -= sizeof(std::uint64_t);
    bytes -= sizeof(std::vector<std::string>::size_type);
    bytes -= sizeof(ecommerce_product_category);
    bytes -= sizeof(double) * 2;
    bytes -= sizeof(std::uint32_t) * 2;
    bytes -= sizeof(std::uint8_t);

    std::size_t remaining_bytes = bytes / 7;
    std::size_t additional_bytes = bytes % 7;

    ecommerce_product product;
    product.uuid = (std::uint64_t) random_unsigned_integer(rng);
    product.name = random_string(rng, remaining_bytes);
    product.description = random_string(rng, remaining_bytes);
    product.category = random_product_category(rng);
    product.tags.emplace_back(random_string(rng, remaining_bytes));
    product.tags.emplace_back(random_string(rng, remaining_bytes));
    product.tags.emplace_back(random_string(rng, remaining_bytes));
    product.image_lo_res_url = random_string(rng, remaining_bytes);
    product.image_hi_res_url = random_string(rng, remaining_bytes + additional_bytes);
    product.price = (double) random_real(rng);
    product.discount = (double) random_real(rng);
    product.stock = (std::uint32_t) random_signed_integer(rng);
    product.rating = (std::uint8_t) random_signed_integer(rng);
    product.reviews = (std::uint32_t) random_signed_integer(rng);

    return product;
}

ecommerce_page random_page_data(random_number_engine auto& rng, std::size_t bytes) {
    bytes = std::max(bytes, std::size_t(1024));
    bytes -= sizeof(std::uint32_t)*4;
    bytes -= sizeof(std::vector<ecommerce_product>::size_type);
    
    std::size_t head_bytes = bytes * (2.0/10.0);
    std::size_t body_bytes = bytes * (8.0/10.0);
    std::size_t remaining_bytes = bytes - head_bytes - body_bytes;

    std::size_t head_bytes_3 = head_bytes / 3;
    remaining_bytes += body_bytes % 3;
    std::size_t body_bytes_6 = body_bytes / 6;
    remaining_bytes += body_bytes % 6;

    ecommerce_page page;
    page.user = random_user_data(rng, head_bytes_3 + remaining_bytes);
    page.permanent_url = random_string(rng, head_bytes_3);
    page.query = random_string(rng, head_bytes_3);
    page.page = (std::uint32_t) random_signed_integer(rng);
    page.total_pages = (std::uint32_t) random_signed_integer(rng);
    page.results_per_page = (std::uint32_t) random_signed_integer(rng);
    page.total_results = (std::uint32_t) random_signed_integer(rng);
    page.products.emplace_back(random_product_data(rng, body_bytes_6));
    page.products.emplace_back(random_product_data(rng, body_bytes_6));  
    page.products.emplace_back(random_product_data(rng, body_bytes_6));
    page.products.emplace_back(random_product_data(rng, body_bytes_6));
    page.products.emplace_back(random_product_data(rng, body_bytes_6));
    page.products.emplace_back(random_product_data(rng, body_bytes_6));
    return page;
}
    
ecommerce_page generate_benchmark_data(std::size_t bytes) {
    std::seed_seq seed{99, 5, 11};
    std::mt19937_64 engine {seed};
    return random_page_data(engine, bytes);
}

std::vector<ecommerce_page> generate_benchmark_data(std::size_t samples, std::size_t bytes) {
    std::vector<ecommerce_page> pages;

    std::seed_seq seed{99, 5, 11};
    std::mt19937_64 engine {seed};

    pages.reserve(samples);
    for (std::size_t i = 0; i < samples; ++i) {
        pages.emplace_back(random_page_data(engine, bytes));
    }
    return pages;    
}

} // namespace benchmarking