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
        COMPTIME_ASSERT((teg::schema<char>() == teg::make_fixed_string("char")));
        COMPTIME_ASSERT((teg::schema<uint8_t>() == teg::make_fixed_string("u8")));
        COMPTIME_ASSERT((teg::schema<uint16_t>() == teg::make_fixed_string("u16")));
        COMPTIME_ASSERT((teg::schema<uint32_t>() == teg::make_fixed_string("u32")));
        COMPTIME_ASSERT((teg::schema<uint64_t>() == teg::make_fixed_string("u64")));
        COMPTIME_ASSERT((teg::schema<int8_t>() == teg::make_fixed_string("i8")));
        COMPTIME_ASSERT((teg::schema<int16_t>() == teg::make_fixed_string("i16")));
        COMPTIME_ASSERT((teg::schema<int32_t>() == teg::make_fixed_string("i32")));
        COMPTIME_ASSERT((teg::schema<int64_t>() == teg::make_fixed_string("i64")));
        COMPTIME_ASSERT((teg::schema<float>() == teg::make_fixed_string("f32")));
        COMPTIME_ASSERT((teg::schema<double>() == teg::make_fixed_string("f64")));
    }
    SECTION("Aggregate types") {
        struct v1 { teg::i32 x; teg::i32 y; };
        struct v2 { teg::i32 x; teg::i32 y; teg::f64 z; };
        struct v3 { teg::i32 x; teg::i32 y; teg::f64 z; teg::f64 w; };
        COMPTIME_ASSERT((teg::schema<v1>() == teg::make_fixed_string("{i32 i32}")));
        COMPTIME_ASSERT((teg::schema<v2>() == teg::make_fixed_string("{i32 i32 f64}")));
        COMPTIME_ASSERT((teg::schema<v3>() == teg::make_fixed_string("{i32 i32 f64 f64}")));
    }
    SECTION("C-array types") {
        COMPTIME_ASSERT((teg::schema<teg::i32[5]>() == teg::make_fixed_string("#[i32]")));
        COMPTIME_ASSERT((teg::schema<teg::i32[5][5]>() == teg::make_fixed_string("#[#[i32]]")));
        COMPTIME_ASSERT((teg::schema<teg::i32[5][5][5]>() == teg::make_fixed_string("#[#[#[i32]]]")));
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

TEST_CASE("Schema hash table") {
    constexpr auto t1 = teg::schema_hash_table<float>();

    for (auto const& hash : t1) {
        std::cout << hash << std::endl;
    }
}