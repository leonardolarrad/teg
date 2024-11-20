#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <string>
#include <array>

#include "teg/md5.h"
#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Test schema") {
    struct A {
        int8_t a;
        int16_t b;
        uint32_t c;
        uint64_t d;
    };

    struct B {};

    constexpr auto c0 = teg::schema_encoder::encode<A>();
    constexpr auto m0 = teg::md5_hash_u64(c0);
    std::cout << m0 << std::endl;
    std::cout << teg::schema<std::array<A, 10>>().c_str() << std::endl;
    std::cout << teg::schema<std::string>().c_str() << std::endl;
    std::cout << teg::schema<std::vector<A>>().c_str() << std::endl;
    std::cout << teg::schema<A>().c_str() << std::endl;
    std::cout << teg::schema<int>().c_str() << std::endl; 
}

TEST_CASE("C-array schema") {
    std::cout << teg::schema<int[10]>().c_str() << std::endl;
}

TEST_CASE("Optional schema") {
    std::cout << teg::schema<std::optional<int>>().c_str() << std::endl;
}

TEST_CASE("Owning pointer schema") {
    std::unique_ptr<int> opt0 = std::make_unique<int>(100);
    std::cout << teg::schema<std::unique_ptr<int>>().c_str() << std::endl;
}

TEST_CASE("Variant schema") {
    std::variant<int, float, std::string> v = 100;
    std::cout << teg::schema<std::variant<int, float, std::string>>().c_str() << std::endl;
}

TEST_CASE("Tuple schema") {
    std::tuple<int, float, std::string> v = std::make_tuple(100, 100.0f, "100");
    std::cout << teg::schema<std::tuple<int, float, std::string>>().c_str() << std::endl;
}

TEST_CASE("Tuple!?") {
    struct A {
        teg::i32 x;
        teg::i32 y;
    };


    auto a = A{1, 2};

    teg::get_member<0>(a) = 9;
    auto ax = teg::get_member<0>(a);
    std::cout << ax << std::endl;

    auto l = []<std::size_t... I>(std::index_sequence<I...>) {
        ((std::cout << teg::get_member<I>(A{5, 6}) << std::endl), ...);
    };
    l(std::make_index_sequence<teg::members_count_v<A>>{});

}

TEST_CASE("Versioning") {
    struct v1 { teg::i32 x; teg::i32 y; };
    struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };
    struct v3 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<teg::f64, 3> w; };

    auto cx3 = teg::schema_analyzer::version_count<v3>();
    std::cout << cx3 << std::endl;
    /*constexpr auto cx4 = teg::version_count<v3>();*/
}


