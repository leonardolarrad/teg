#include "larrad/larrad.h"
#include "test/test.h"

int stuff(int x) {
    return x+x+x+x;
}

TEST_CASE("Second test") {
    ASSERT_EQ(stuff(1), 4);
}

TEST_CASE("Third test") {
    ASSERT_EQ(stuff(1), 4);
    ASSERT_EQ(stuff(1), 4);
}