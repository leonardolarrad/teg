#include <array>
#include <bitset>
#include <cstdint>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial de/serialization") {
    SECTION("Tuple") {
        std::tuple<int, float, std::string> t0 = { 100, 100.0f, "100" };

        teg::buffer b;
        teg::serialize(b, t0).or_throw();

        std::tuple<int, float, std::string> t1;
        teg::deserialize(b, t1).or_throw();

        ASSERT(std::get<0>(t1) == 100);
        ASSERT(std::get<1>(t1) == 100.0f);
        ASSERT(std::get<2>(t1) == "100");
    }
    SECTION("Pair") {
        std::pair<int, float> p0 = { 100, 100.0f };

        teg::buffer b;
        teg::serialize(b, p0).or_throw();

        std::pair<int, float> p1;
        teg::deserialize(b, p1).or_throw();

        ASSERT(p0 == p1);
    }
}

TEST_CASE("Aggregate de/serialization") {
    SECTION("Tuple") {
        std::tuple<std::tuple<int, float>, std::variant<int, float>> t0 = { { 100, 100.0f }, 100 };

        teg::buffer b;
        teg::serialize(b, t0).or_throw();

        std::tuple<std::tuple<int, float>, std::variant<int, float>> t1;
        teg::deserialize(b, t1).or_throw();

        ASSERT(std::get<0>(std::get<0>(t1)) == 100);
        ASSERT(std::get<1>(std::get<0>(t1)) == 100.0f);
        ASSERT(std::holds_alternative<int>(std::get<1>(t1)));
        ASSERT(std::get<int>(std::get<1>(t1)) == 100);
    }
    SECTION("Pair") {
        std::pair<std::pair<int, float>, std::string> p0 = { { 100, 100.0f }, "100" };

        teg::buffer b;
        teg::serialize(b, p0).or_throw();

        std::pair<std::pair<int, float>, std::string> p1;
        teg::deserialize(b, p1).or_throw();

        ASSERT(p0 == p1);
    }
}