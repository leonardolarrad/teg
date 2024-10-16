#include <array>
#include <string>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

struct vec2 {
    int x, y;
    constexpr bool operator==(vec2 const&) const = default;
};

constexpr auto cx_int() {
    teg::byte_buffer buffer{};

    int i0 = 66;    
    teg::serialize(buffer, i0).or_throw();

    int i1;
    teg::deserialize(buffer, i1).or_throw();
    return i1;
}

constexpr auto cx_c_array() {
    teg::byte_buffer buffer{};

    int i0[5] = { 1, 2, 3, 4, 55 };    
    teg::serialize(buffer, i0).or_throw();

    int i1[5];
    teg::deserialize(buffer, i1).or_throw();
    return i1[4];
}

constexpr auto cx_md_c_array() {
    teg::byte_buffer buffer{};

    int a0[5][5] = { 
        { 1, 2, 3, 4, 77 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 }, 
        { 16, 17, 18, 19, 20 }, { 21, 22, 23, 24, 22 } }; 
    teg::serialize(buffer, a0).or_throw();
    
    int a1[5][5];
    teg::deserialize(buffer, a1).or_throw();    
    return a1[4][4];
}

constexpr auto cx_std_array() {
    teg::byte_buffer buffer{};

    std::array<int, 5> i0 = { 1, 2, 3, 4, 77 };
    teg::serialize(buffer, i0).or_throw();

    std::array<int, 5> i1;
    teg::deserialize(buffer, i1).or_throw();
    return i1[4];
}

constexpr auto cx_fixed_string() {
    teg::byte_buffer buffer{};

    auto s0 = teg::make_fixed_string("Hello World!");
    teg::serialize(buffer, s0).or_throw();
    
    teg::fixed_string<12> s1;
    teg::deserialize(buffer, s1).or_throw();
    return s1;
}

constexpr auto cx_struct() {
    teg::byte_buffer buffer{};

    vec2 v0 = { 11, 99 };
    teg::serialize(buffer, v0).or_throw();
    
    vec2 v1;
    teg::deserialize(buffer, v1).or_throw();
    return v1;
}

TEST_CASE("Compile-time de/serialization") {    
    constexpr auto cx0 = cx_int();
    COMPTIME_ASSERT_EQ(cx0, 66);    

    constexpr auto cx1 = cx_c_array();
    COMPTIME_ASSERT_EQ(cx1, 55);

    constexpr auto cx2 = cx_md_c_array();
    COMPTIME_ASSERT_EQ(cx2, 22);

    constexpr auto cx3 = cx_std_array();
    COMPTIME_ASSERT_EQ(cx3, 77);

    constexpr auto cx4 = cx_fixed_string();
    COMPTIME_ASSERT_EQ(cx4, "Hello World!");

    constexpr auto cx5 = cx_struct();
    COMPTIME_ASSERT((cx5 == vec2{ 11, 99 }));
}