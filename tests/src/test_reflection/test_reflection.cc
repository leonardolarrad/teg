#include <cstdint>
#include "teg/teg.h"
#include "test/test.h"

struct vec2 {
    std::int32_t x;
    std::int32_t y;
};

struct vec3 {
    std::int32_t x;
    std::int32_t y;
    std::int32_t z;
};

struct quat {
    vec3 v;
    std::int32_t w;
};

TEST_CASE("Members count") {
    // fundamental types
    ASSERT_EQ(teg::members_count<bool>, 0);
    ASSERT_EQ(teg::members_count<char>, 0);
    ASSERT_EQ(teg::members_count<int>, 0);
    ASSERT_EQ(teg::members_count<unsigned int>, 0);
    ASSERT_EQ(teg::members_count<float>, 0);
    ASSERT_EQ(teg::members_count<double>, 0);
    // c arrays
    ASSERT_EQ(teg::members_count<std::int32_t[5]>, 5);
    ASSERT_EQ(teg::members_count<std::int32_t[125]>, 125);
    ASSERT_EQ(teg::members_count<std::int32_t[125][5]>, 125);
    // classes
    ASSERT_EQ(teg::members_count<vec2>, 2);
    ASSERT_EQ(teg::members_count<vec3>, 3);
    ASSERT_EQ(teg::members_count<quat>, 2);
}
