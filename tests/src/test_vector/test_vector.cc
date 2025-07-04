#include <array>
#include <string>
#include <vector>

#include <iostream>
#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial de/serialization") {    
    teg::byte_array b;
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    teg::serialize(b, v0).or_throw();

    std::vector<int> v1;
    teg::deserialize(b, v1).or_throw();
    
    ASSERT_EQ(std::memcmp(v0.data(), v1.data(), v0.size() * sizeof(int)), 0);
    ASSERT(v0 == v1);
}

TEST_CASE("Aggregate de/serialization") {
    SECTION("Vector of vectors") {
        teg::byte_array b;
        std::vector<std::vector<int>> v0 = { { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 } };
        teg::serialize(b, v0).or_throw();

        std::vector<std::vector<int>> v1;
        teg::deserialize(b, v1).or_throw();
        
        ASSERT(v0 == v1);
    }
    SECTION("Vector of vectors of vectors") {
        teg::byte_array b;
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

        teg::byte_array b = {};
        std::vector<non_trivial_a> v0 = { non_trivial_a{}, non_trivial_a{}, non_trivial_a{} };
        teg::serialize(b, v0).or_throw();
        
        std::vector<non_trivial_a> v1;
        teg::deserialize(b, v1).or_throw();
        
        ASSERT(v0 == v1);
    }
}