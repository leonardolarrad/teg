#include <set>
#include <string>
#include <unordered_set>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial de/serialization") {
    SECTION("Set") {
        teg::byte_array b;
        std::set<int> s0 = { 1, 2, 3, 4, 5 };
        teg::serialize(b, s0).or_throw();

        std::set<int> s1;
        teg::deserialize(b, s1).or_throw();
        ASSERT(s0 == s1);
    }
    SECTION("Multiset") {
        teg::byte_array b;
        std::multiset<int> s0 = { 1, 1, 2, 2, 5 };
        teg::serialize(b, s0).or_throw();

        std::multiset<int> s1;
        teg::deserialize(b, s1).or_throw();
        ASSERT(s0 == s1);
    }
    SECTION("Unordered set") {
        teg::byte_array b;
        std::unordered_set<int> s0 = { 1, 2, 3, 4, 5 };
        teg::serialize(b, s0).or_throw();

        std::unordered_set<int> s1;
        teg::deserialize(b, s1).or_throw();
        ASSERT(s0 == s1);
    }
    SECTION("Unordered multiset") {
        teg::byte_array b;
        std::unordered_multiset<int> s0 = { 1, 1, 2, 2, 5 };

        teg::serialize(b, s0).or_throw();

        std::unordered_multiset<int> s1;
        teg::deserialize(b, s1).or_throw();
        ASSERT(s0 == s1);
    }
}

TEST_CASE("Aggregate de/serialization") {
    SECTION("Set") {
        teg::byte_array b;
        std::set<std::string> s0 = { "a", "b", "c", "d", "e" };
        teg::serialize(b, s0).or_throw();

        std::set<std::string> s1;
        teg::deserialize(b, s1).or_throw();
        ASSERT(s0 == s1);
    }
    SECTION("Multiset") {
        teg::byte_array b;
        std::multiset<std::string> s0 = { "a", "b", "c", "d", "e" };
        teg::serialize(b, s0).or_throw();

        std::multiset<std::string> s1;
        teg::deserialize(b, s1).or_throw();
        ASSERT(s0 == s1);
    }
    SECTION("Unordered set") {
        teg::byte_array b;
        std::unordered_set<std::string> s0 = { "a", "b", "c", "d", "e" };
        teg::serialize(b, s0).or_throw();

        std::unordered_set<std::string> s1;
        teg::deserialize(b, s1).or_throw();
        ASSERT(s0 == s1);
    }
    SECTION("Unordered multiset") {
        teg::byte_array b;
        std::unordered_multiset<std::string> s0 = { "a", "b", "c", "d", "e" };
        teg::serialize(b, s0).or_throw();

        std::unordered_multiset<std::string> s1;
        teg::deserialize(b, s1).or_throw();
        ASSERT(s0 == s1);
    }
}