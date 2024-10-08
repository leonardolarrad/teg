#include <cstdint>
#include <cstring>
#include <functional>

#include "teg/teg.h"
#include "test/test.h"

struct int_2 {
    int x, y;
};

struct int_3 {
    int x, y, z;
};

struct int_2_2 {
    int_2 pos;
    int_2 size;
};

struct int_64 {
    int a1, a2, a3, a4, a5, a6, a7, a8;
    int b1, b2, b3, b4, b5, b6, b7, b8;
    int c1, c2, c3, c4, c5, c6, c7, c8;
    int d1, d2, d3, d4, d5, d6, d7, d8;
    int e1, e2, e3, e4, e5, e6, e7, e8;
    int f1, f2, f3, f4, f5, f6, f7, f8;
    int g1, g2, g3, g4, g5, g6, g7, g8;
    int h1, h2, h3, h4, h5, h6, h7, h8;
};

struct int_100 {
    int a1, a2, a3, a4, a5, a6, a7, a8, a9, a10;
    int b1, b2, b3, b4, b5, b6, b7, b8, b9, b10;
    int c1, c2, c3, c4, c5, c6, c7, c8, c9, c10;
    int d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
    int e1, e2, e3, e4, e5, e6, e7, e8, e9, e10;
    int f1, f2, f3, f4, f5, f6, f7, f8, f9, f10;
    int g1, g2, g3, g4, g5, g6, g7, g8, g9, g10;
    int h1, h2, h3, h4, h5, h6, h7, h8, h9, h10;
    int i1, i2, i3, i4, i5, i6, i7, i8, i9, i10;
    int j1, j2, j3, j4, j5, j6, j7, j8, j9, j10;
};

class mix_12 {
public:
    std::size_t a;
    std::string b;
    int_2 c;
    int_3 d;
    int_2_2 e;
    std::int8_t f;
    std::int32_t g;
    std::int32_t h;
    std::int32_t i;
    std::int64_t j;
    std::int32_t k;
    std::int64_t l;  
};

struct int_arr_2_16 {
    int a[16];
    int b[16];
};

struct char_arr_2_32 {
    const char a[32];
    const char b[32];
};

struct string_2 {
    std::string a;
    std::string b;
};

TEST_CASE("Reflect members count") {
    // c arrays
    ASSERT_EQ(teg::members_count_v<std::int32_t[5]>, 5);
    ASSERT_EQ(teg::members_count_v<std::int32_t[125]>, 125);
    ASSERT_EQ(teg::members_count_v<std::int32_t[125][5]>, 125);
    // classes
    ASSERT_EQ(teg::members_count_v<int_2>, 2);
    ASSERT_EQ(teg::members_count_v<int_3>, 3);
    ASSERT_EQ(teg::members_count_v<int_64>, 64);
    ASSERT_EQ(teg::members_count_v<int_100>, 100);
    ASSERT_EQ(teg::members_count_v<int_2_2>, 2);
    ASSERT_EQ(teg::members_count_v<mix_12>, 12);
    ASSERT_EQ(teg::members_count_v<int_arr_2_16>, 2);
    ASSERT_EQ(teg::members_count_v<char_arr_2_32>, 2);       
    ASSERT_EQ(teg::members_count_v<string_2>, 2);
}

TEST_CASE("Visit members") {
    {
        auto r = teg::visit_members(
            int_2 { 1999, 1998 },
            [](auto x, auto y) {
                ASSERT_EQ(x, 1999);
                ASSERT_EQ(y, 1998);
                return x + y;
            }
        );
        ASSERT_EQ(r, 3997);
    }
    {
        teg::visit_members(
            char_arr_2_32 { "hello", "world" },
            [](auto x, auto y) {
                ASSERT_EQ(std::string(x), "hello");
                ASSERT_EQ(std::string(y), "world");
            }
        );
    }
    {
        auto r = teg::visit_members(
            string_2 { "hello", "world" },
            [](auto& x, auto& y) {
                ASSERT_EQ(x, "hello");
                ASSERT_EQ(y, "world");
                return x + " " + y;
            }
        );
        ASSERT_EQ(r, "hello world");
    }
}