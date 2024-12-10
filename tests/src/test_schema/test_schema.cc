#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>
#include <string>
#include <array>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Schema encoding") {
    SECTION("Builtin types") {
        COMPTIME_ASSERT((teg::schema<1, char>() == teg::make_fixed_string("char")));
        COMPTIME_ASSERT((teg::schema<1, uint8_t>() == teg::make_fixed_string("u8")));
        COMPTIME_ASSERT((teg::schema<1, uint16_t>() == teg::make_fixed_string("u16")));
        COMPTIME_ASSERT((teg::schema<1, uint32_t>() == teg::make_fixed_string("u32")));
        COMPTIME_ASSERT((teg::schema<1, uint64_t>() == teg::make_fixed_string("u64")));
        COMPTIME_ASSERT((teg::schema<1, int8_t>() == teg::make_fixed_string("i8")));
        COMPTIME_ASSERT((teg::schema<1, int16_t>() == teg::make_fixed_string("i16")));
        COMPTIME_ASSERT((teg::schema<1, int32_t>() == teg::make_fixed_string("i32")));
        COMPTIME_ASSERT((teg::schema<1, int64_t>() == teg::make_fixed_string("i64")));
        COMPTIME_ASSERT((teg::schema<1, float>() == teg::make_fixed_string("f32")));
        COMPTIME_ASSERT((teg::schema<1, double>() == teg::make_fixed_string("f64")));
    }
    SECTION("Aggregate types") {
        struct v1 { teg::i32 x; teg::i32 y; };
        struct v2 { teg::i32 x; teg::i32 y; teg::f64 z; };
        struct v3 { teg::i32 x; teg::i32 y; teg::f64 z; teg::f64 w; };
        COMPTIME_ASSERT((teg::schema<1, v1>() == teg::make_fixed_string("{i32 i32}")));
        COMPTIME_ASSERT((teg::schema<1, v2>() == teg::make_fixed_string("{i32 i32 f64}")));
        COMPTIME_ASSERT((teg::schema<1, v3>() == teg::make_fixed_string("{i32 i32 f64 f64}")));
    }
    SECTION("C-array types") {
        COMPTIME_ASSERT((teg::schema<1, teg::i32[5]>() == teg::make_fixed_string("#[i32]")));
        COMPTIME_ASSERT((teg::schema<1, teg::i32[5][5]>() == teg::make_fixed_string("#[#[i32]]")));
        COMPTIME_ASSERT((teg::schema<1, teg::i32[5][5][5]>() == teg::make_fixed_string("#[#[#[i32]]]")));
    }
}

TEST_CASE("Schema version") {
    struct v1 { teg::i32 x; teg::i32 y; };
    struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };
    struct v3 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<teg::f64, 3> w; };

    COMPTIME_ASSERT((teg::version_count_v<teg::compatible<teg::f64, 2>> == 0)); // Root cannot be a compatible object
    COMPTIME_ASSERT(teg::version_count_v<int> == 1); // For non-agreggate types the version is always 1
    COMPTIME_ASSERT(teg::version_count_v<v1> == 1);  // In this casse the aggregate dont have a compatible object, and therefore
                                                     // is the original version
    COMPTIME_ASSERT(teg::version_count_v<v2> == 2);  
    COMPTIME_ASSERT(teg::version_count_v<v3> == 3);
}

TEST_CASE("Schema encoding with versions") {
    struct v1 { teg::i32 x; teg::i32 y; };
    struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };
    struct v3 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<v1, 3> w; };
    struct v4 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<teg::f64, 2> w; };
        
    std::cout << teg::schema<1, v1>().c_str() << std::endl;
    std::cout << teg::schema<1, v2>().c_str() << std::endl;
    std::cout << teg::schema<2, v2>().c_str() << std::endl;
    std::cout << teg::schema<1, v3>().c_str() << std::endl;
    std::cout << teg::schema<2, v3>().c_str() << std::endl;
    std::cout << teg::schema<3, v3>().c_str() << std::endl;
    std::cout << teg::schema<1, v4>().c_str() << std::endl;
    std::cout << teg::schema<2, v4>().c_str() << std::endl;
    std::cout << teg::schema<3, v4>().c_str() << std::endl;
}

TEST_CASE("Schema hash table") {
    struct v4 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<teg::f64, 2> w; };

    constexpr auto t1 = teg::schema_hash_table<float, int, teg::compatible<int, 2>>();

    for (auto const& hash : t1) {
        std::cout << hash << std::endl;
    }

    std::cout << std::endl;
    constexpr auto t2 = teg::schema_hash_table<v4>();

    for (auto const& hash : t2) {
        std::cout << hash << std::endl;
    }

    std::cout << teg::schema<1, v4>().c_str() << std::endl;
    std::cout << teg::schema<2, v4>().c_str() << std::endl;

}