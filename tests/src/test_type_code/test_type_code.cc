#include <string>
#include <concepts>
#include <type_traits>
#include <iostream>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Type code") {
    SECTION("Fundamental types") {
        constexpr auto code = teg::get_type_code<int>();

        for (auto c : code) {
            std::cout << int(c);
        }
        std::cout << std::endl;
    }
    SECTION("Containers") {
        constexpr auto code = teg::get_type_code<std::vector<int>>();

        for (auto c : code) {
            std::cout << int(c);
        }
        std::cout << std::endl;
    }
    SECTION("Aggregates") {
        struct vec2 {
            float x, y;
        };

        auto code = teg::get_type_code<vec2>();

        for (auto c : code) {
            std::cout << int(c) << " ";
        }
        std::cout << std::endl;
    }
}