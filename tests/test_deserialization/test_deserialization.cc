#include <iostream>
#include <test_macros.h>

int stuff(int x) {
    return x+x+x+x;
}

TEST_CASE("First test") {
    CHECK(stuff(1) != 4);
}

TEST_CASE("Second test") {
    CHECK_EQ(stuff(1), 4);
}

TEST_CASE("Third test") {
    CHECK_EQ(stuff(1), 4);
    CHECK_EQ(stuff(1), 4);
}