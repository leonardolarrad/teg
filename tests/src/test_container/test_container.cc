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

TEST_CASE("De/serialize std::array") {    
    teg::buffer b;
    std::array<int, 2> arr0 = { 1, 2 };
    teg::serialize(b, arr0).or_throw();

    std::array<int, 2> arr1;
    teg::deserialize(b, arr1).or_throw();
    ASSERT_EQ(std::memcmp(arr0.data(), arr1.data(), sizeof(arr0)), 0);    
}

TEST_CASE("De/serialize std::vector") {    
    teg::buffer b;
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    teg::serialize(b, v0).or_throw();

    std::vector<int> v1;
    teg::deserialize(b, v1).or_throw();
    
    ASSERT_EQ(std::memcmp(v0.data(), v1.data(), v0.size() * sizeof(int)), 0);
    ASSERT(v0 == v1);    
}

TEST_CASE("De/serialize std::string") {    
    teg::buffer b;
    std::string s0 = "Hello World!";
    teg::serialize(b, s0).or_throw();

    std::string s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT_EQ(s0, s1);    
}

TEST_CASE("De/serialize std::deque") {    
    teg::buffer b;
    std::deque<int> d0 = { 1, 2, 3, 4, 5, 6 };
    teg::serialize(b, d0).or_throw();

    std::deque<int> d1;
    teg::deserialize(b, d1).or_throw();
    ASSERT(d0 == d1);    
}

TEST_CASE("De/serialize std::list") {    
    teg::buffer b;
    std::list<int> l0 = { 1, 2, 3, 4, 5, 6 };
    teg::serialize(b, l0).or_throw();

    std::list<int> l1;
    teg::deserialize(b, l1).or_throw();
    ASSERT(l0 == l1);    
}

TEST_CASE("De/serialize std::forward_list") {    
    teg::buffer b;
    std::forward_list<int> fl0 = std::forward_list<int>({ 1, 2, 3, 4, 5, 6 });
    teg::serialize(b, fl0).or_throw();

    std::forward_list<int> fl1;
    teg::deserialize(b, fl1).or_throw();
    ASSERT(fl0 == fl1);    
}

