#include <array>
#include <string>
#include <variant>
#include <vector>
#include <bitset>
#include <cstdint>

#include "teg/index_table.h"
#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial de/serialization") {
    SECTION("Index 0") {
        teg::buffer b;
        std::variant<int, float, std::string> v0 = 100;
        teg::serialize(b, v0).or_throw();

        std::variant<int, float, std::string> v1;
        teg::deserialize(b, v1).or_throw();

        ASSERT(std::holds_alternative<int>(v1));
        ASSERT(std::get<int>(v1) == 100);
    }
    SECTION("Index 1") {
        teg::buffer b;
        std::variant<int, float, std::string> v0 = 100.0f;
        teg::serialize(b, v0).or_throw();

        std::variant<int, float, std::string> v1;
        teg::deserialize(b, v1).or_throw();

        ASSERT(std::holds_alternative<float>(v1));
        ASSERT(std::get<float>(v1) == 100.0f);
    }
    SECTION("Index 2") {
        teg::buffer b;
        std::variant<int, float, std::string> v0 = "100";
        teg::serialize(b, v0).or_throw();

        std::variant<int, float, std::string> v1;
        teg::deserialize(b, v1).or_throw();

        ASSERT(std::holds_alternative<std::string>(v1));
        ASSERT(std::get<std::string>(v1) == "100");
    }
}