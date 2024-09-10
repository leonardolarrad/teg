#include <set>

#include <iostream>
#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial de/serialization") {    
    teg::buffer b;
    std::set<int> s0 = { 1, 2, 3, 4, 5 };
    teg::serialize(b, s0).or_throw();

    std::set<int> s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}
