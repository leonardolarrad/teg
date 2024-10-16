#include <any>
#include <array>
#include <bitset>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Check concepts") {
    ASSERT((teg::concepts::variant<std::variant<int, float>>));
    ASSERT(!(teg::concepts::variant<std::any>));
}

TEST_CASE("Trivial de/serialization") {
    SECTION("Index 0") {
        teg::byte_buffer b;
        std::variant<int, float, std::string> v0 = 100;
        teg::serialize(b, v0).or_throw();

        std::variant<int, float, std::string> v1;
        teg::deserialize(b, v1).or_throw();

        ASSERT(std::holds_alternative<int>(v1));
        ASSERT(std::get<int>(v1) == 100);
    }
    SECTION("Index 1") {
        teg::byte_buffer b;
        std::variant<int, float, std::string> v0 = 100.0f;
        teg::serialize(b, v0).or_throw();

        std::variant<int, float, std::string> v1;
        teg::deserialize(b, v1).or_throw();

        ASSERT(std::holds_alternative<float>(v1));
        ASSERT(std::get<float>(v1) == 100.0f);
    }
    SECTION("Index 2") {
        teg::byte_buffer b;
        std::variant<int, float, std::string> v0 = "100";
        teg::serialize(b, v0).or_throw();

        std::variant<int, float, std::string> v1;
        teg::deserialize(b, v1).or_throw();

        ASSERT(std::holds_alternative<std::string>(v1));
        ASSERT(std::get<std::string>(v1) == "100");
    }
}

TEST_CASE("Aggregate de/serialization") {
    SECTION("Index 0") {
        teg::byte_buffer b;
        std::variant<std::variant<int, float>, std::string> v0 = 100;
        teg::serialize(b, v0).or_throw();

        std::variant<std::variant<int, float>, std::string> v1;
        teg::deserialize(b, v1).or_throw();

        ASSERT((std::holds_alternative<std::variant<int, float>>(v1)));
        ASSERT((std::get<int>(std::get<std::variant<int, float>>(v1)) == 100));
    }
    SECTION("Index 1") {
        teg::byte_buffer b;
        std::variant<std::variant<int, float>, std::string> v0 = "100";
        teg::serialize(b, v0).or_throw();

        std::variant<std::variant<int, float>, std::string> v1;
        teg::deserialize(b, v1).or_throw();

        ASSERT((std::holds_alternative<std::string>(v1)));
        ASSERT((std::get<std::string>(v1) == "100"));
    }
}