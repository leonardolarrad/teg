#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <string>
#include <array>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Test schema") {
    struct A {
        int8_t a;
        int16_t b;
        uint32_t c;
        uint64_t d;
    };

    enum et {
    };

    constexpr auto b = std::is_enum_v<et>;
    using xt = std::underlying_type_t<et>;
    xt x = 0;
    constexpr auto b1 = std::is_enum_v<xt>;
    std::cout << b << std::endl;


    constexpr auto var = "Erm, hello?";
    std::cout << teg::xxhash::hash64(std::span<const char>{var, std::strlen(var)}) << std::endl;

    std::cout << sizeof(et) << std::endl;
    std::cout << std::integral<et> << std::endl;
    std::cout << std::signed_integral<et> << std::endl;
    std::cout << std::unsigned_integral<et> << std::endl;

    std::cout << teg::schema<std::array<A, 10>>().c_str() << std::endl;
    std::cout << teg::schema<std::string>().c_str() << std::endl;
    std::cout << teg::schema<std::vector<A>>().c_str() << std::endl;
    std::cout << teg::schema<A>().c_str() << std::endl;
    std::cout << teg::schema<int>().c_str() << std::endl; 
}
