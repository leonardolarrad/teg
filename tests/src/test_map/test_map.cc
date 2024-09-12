#include <map>
#include <unordered_map>
#include <string>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial map de/serialization") {
    SECTION("Map") {
        teg::buffer b;
        std::map<int, int> m0 = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };
        teg::serialize(b, m0).or_throw();

        std::map<int, int> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
    SECTION("Multimap") {
        teg::buffer b;
        std::multimap<int, int> m0 = { { 1, 1 }, { 1, 1 }, { 2, 2 }, { 2, 2 }, { 5, 5 } };
        teg::serialize(b, m0).or_throw();

        std::multimap<int, int> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
    SECTION("Unordered map") {
        teg::buffer b;
        std::unordered_map<int, int> m0 = { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };
        teg::serialize(b, m0).or_throw();

        std::unordered_map<int, int> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
}

TEST_CASE("Non-trivial map de/serialization") {

    using nconst_t = std::remove_const_t<const std::pair<std::string const, std::string>>;


    SECTION("Map") {
        teg::buffer b;
        std::map<std::string, std::string> m0 = { { "a", "a" }, { "b", "b" }, { "c", "c" }, { "d", "d" }, { "e", "e" } };
        teg::serialize(b, m0).or_throw();

        std::map<std::string, std::string> m1;
        teg::deserialize(b, m1).or_throw();
        ASSERT(m0 == m1);
    }
}
