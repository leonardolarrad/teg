#include <map>
#include <unordered_map>
#include <string>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial de/serialization") {
    SECTION("Map") {
        teg::byte_array b;
        std::map<int, int> m0 = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };
        teg::serialize(b, m0).or_throw();

        std::map<int, int> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
    SECTION("Multimap") {
        teg::byte_array b;
        std::multimap<int, int> m0 = { { 1, 1 }, { 1, 1 }, { 2, 2 }, { 2, 2 }, { 5, 5 } };
        teg::serialize(b, m0).or_throw();

        std::multimap<int, int> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
    SECTION("Unordered map") {
        teg::byte_array b;
        std::unordered_map<int, int> m0 = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };
        teg::serialize(b, m0).or_throw();

        std::unordered_map<int, int> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
    SECTION("Unordered multimap") {
        teg::byte_array b;
        std::unordered_multimap<int, int> m0 = { { 1, 1 }, { 1, 1 }, { 2, 2 }, { 2, 2 }, { 5, 5 } };
        teg::serialize(b, m0).or_throw();

        std::unordered_multimap<int, int> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
}

TEST_CASE("Aggregate de/serialization") {    
    SECTION("Map") {
        teg::byte_array b;
        std::map<std::string, std::string> m0 = { 
            { "a", "a" }, { "b", "b" }, 
            { "c", "c" }, { "d", "d" }, 
            { "e", "e" } };
        teg::serialize(b, m0).or_throw();

        std::map<std::string, std::string> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
    SECTION("Multimap") {
        teg::byte_array b;
        std::multimap<std::string, std::string> m0 = { 
            { "a", "a" }, { "b", "b" }, 
            { "c", "c" }, { "d", "d" }, 
            { "e", "e" } };
        teg::serialize(b, m0).or_throw();

        std::multimap<std::string, std::string> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
    SECTION("Unordered map") {
        teg::byte_array b;
        std::unordered_map<std::string, std::string> m0 = { 
            { "a", "a" }, { "b", "b" }, 
            { "c", "c" }, { "d", "d" }, 
            { "e", "e" } };
        teg::serialize(b, m0).or_throw();

        std::unordered_map<std::string, std::string> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
    SECTION("Unordered multimap") {
        teg::byte_array b;
        std::unordered_multimap<std::string, std::string> m0 = { 
            { "a", "a" }, { "b", "b" }, 
            { "c", "c" }, { "d", "d" }, 
            { "e", "e" } };
        teg::serialize(b, m0).or_throw();

        std::unordered_multimap<std::string, std::string> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
}
