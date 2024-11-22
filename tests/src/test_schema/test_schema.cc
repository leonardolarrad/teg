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

TEST_CASE("Schema version") {
    struct v1 { teg::i32 x; teg::i32 y; };
    struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };
    struct v3 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<teg::f64, 3> w; };
    struct e1 { teg::i32 x; teg::compatible<teg::f64, 2> z; teg::f64 y; };
    struct e2 { teg::compatible<teg::f64, 2> z; };
    struct e3 { teg::i32 x; teg::compatible<teg::f64, 2> y; teg::compatible<teg::f32, 1024> z; };
    struct e4 { teg::i32 x; teg::compatible<teg::f64, 3> y; teg::compatible<teg::f32, 2> z; };

    COMPTIME_ASSERT((teg::version_count_v<teg::compatible<teg::f64, 2>> == 0)); // Root cannot be a compatible object
    COMPTIME_ASSERT(teg::version_count_v<int> == 1); // For non-agreggate types the version is always 1
    COMPTIME_ASSERT(teg::version_count_v<v1> == 1);  // In this casse the aggregate dont have a compatible object, and therefore
                                                     // is the original version
    COMPTIME_ASSERT(teg::version_count_v<v2> == 2);  
    COMPTIME_ASSERT(teg::version_count_v<v3> == 3);

    COMPTIME_ASSERT(teg::version_count_v<e1> == 0);
    COMPTIME_ASSERT(teg::version_count_v<e2> == 0);
    COMPTIME_ASSERT(teg::version_count_v<e3> == 0);
    COMPTIME_ASSERT(teg::version_count_v<e4> == 0);
}

TEST_CASE("Schema encoding with versions") {
    struct v1 { teg::i32 x; teg::i32 y; };
    struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };
    struct v3 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<v1, 3> w; };
    struct v4 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<teg::f64, 2> w; };
        
    std::cout << teg::schema<v1>().c_str() << std::endl;
    std::cout << teg::schema<v2>().c_str() << std::endl;
    std::cout << teg::schema<v2, 2>().c_str() << std::endl;
    std::cout << teg::schema<v3>().c_str() << std::endl;
    std::cout << teg::schema<v3, 2>().c_str() << std::endl;
    std::cout << teg::schema<v3, 3>().c_str() << std::endl;
    std::cout << teg::schema<v4>().c_str() << std::endl;
    std::cout << teg::schema<v4, 2>().c_str() << std::endl;
    std::cout << teg::schema<v4, 3>().c_str() << std::endl;
}
