#include <vector>
#include <string>
#include <array>

#include <iostream>
#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial de/serialization") {    
    teg::buffer b;
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    teg::serialize(b, v0).or_throw();

    std::vector<int> v1;
    teg::deserialize(b, v1).or_throw();
    
    ASSERT_EQ(std::memcmp(v0.data(), v1.data(), v0.size() * sizeof(int)), 0);
    ASSERT(v0 == v1);
}

TEST_CASE("Non-trivial de/serialization") {
    SECTION("Vector of vectors") {
        teg::buffer b;
        std::vector<std::vector<int>> v0 = { { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 } };
        teg::serialize(b, v0).or_throw();

        std::vector<std::vector<int>> v1;
        teg::deserialize(b, v1).or_throw();
        
        ASSERT(v0 == v1);
    }
    SECTION("Vector of vectors of vectors") {
        teg::buffer b;
        std::vector<std::vector<std::vector<int>>> v0 = { 
            { { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 } }, 
            { { 11, 12, 13, 14, 15 }, 
            { 16, 17, 18, 19, 20 } } };
        teg::serialize(b, v0).or_throw();

        std::vector<std::vector<std::vector<int>>> v1;
        teg::deserialize(b, v1).or_throw();
        
        ASSERT(v0 == v1);
    }
    SECTION("Vector of non-trivial elements") {
        struct non_trivial_a {
            std::string s0 = "Text 0";
            std::string s1 = "Text 1"; 
            std::string s3 = "Text 2";

            bool operator==(const non_trivial_a&) const = default;
        };
        
        struct non_trivial_b {
            std::array<std::vector<non_trivial_a>, 2> a0 = { { { non_trivial_a{} }, { non_trivial_a{} } } };
            std::array<std::vector<non_trivial_a>, 2> a1 = { { { non_trivial_a{} }, { non_trivial_a{} } } };

            non_trivial_a na0 = non_trivial_a{};
            non_trivial_a na1 = non_trivial_a{};
            non_trivial_a na2 = non_trivial_a{};

            bool operator==(const non_trivial_b&) const = default;
        };

        constexpr auto cc1 = teg::aggregate<non_trivial_a>;
        constexpr auto cc2 = teg::aggregate<non_trivial_b>;
        std::cout << cc1 << " " << cc2 << std::endl;

        constexpr auto cc3 = teg::aggregate<std::array<std::vector<non_trivial_a>, 2>>;
        constexpr auto cc4 = teg::aggregate<std::vector<non_trivial_a>>;

        teg::buffer b;
        std::vector<non_trivial_b> v0 = { non_trivial_b{}, non_trivial_b{} };
        teg::serialize(b, v0).or_throw();

        std::vector<non_trivial_b> v1;
        teg::deserialize(b, v1).or_throw();
        
        ASSERT(v0 == v1);
    }
}