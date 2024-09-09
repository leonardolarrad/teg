#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <string>

#include <iostream>
#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("De/serialize std::vector") {
    {
        teg::buffer b;
        std::vector<int> v0 = { 1, 2, 3, 4, 5 };
        teg::serialize(b, v0).or_throw();

        std::vector<int> v1;
        teg::deserialize(b, v1).or_throw();
        
        ASSERT_EQ(std::memcmp(v0.data(), v1.data(), v0.size() * sizeof(int)), 0);
        ASSERT(v0 == v1);
    }
}
