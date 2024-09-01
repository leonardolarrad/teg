#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <string>
#include <iostream>

#include "teg/teg.h"
#include "test/test.h"

class valid_elem {
public:
    valid_elem() = default;
};

class invalid_elem {
private:
    invalid_elem();
};

TEST_CASE("Concept: teg::container") {    
    ASSERT((teg::container<std::array<valid_elem, 10>>));    
    ASSERT((teg::container<std::vector<valid_elem>>));
    ASSERT((teg::container<std::deque<valid_elem>>));
    ASSERT((teg::container<std::forward_list<valid_elem>>));
    ASSERT((teg::container<std::list<valid_elem>>));
    ASSERT((teg::container<std::basic_string<char>>));
    
    ASSERT((teg::container<std::set<valid_elem>>));
    ASSERT((teg::container<std::map<int, valid_elem>>));
    ASSERT((teg::container<std::multiset<valid_elem>>));
    ASSERT((teg::container<std::multimap<int, valid_elem>>));

    ASSERT((teg::container<std::unordered_set<int>>));    
    ASSERT((teg::container<std::unordered_map<int, valid_elem>>));
    ASSERT((teg::container<std::unordered_multiset<int>>));
    ASSERT((teg::container<std::unordered_multimap<int, valid_elem>>));

}

auto test_container(teg::container auto const& c) {
    return true;
}

auto test_container(auto const& c) {
    return false;
}

TEST_CASE("Overload resolution with concepts") {
    std::vector<int> v0 = std::vector<int>{1, 2, 3};
    ASSERT(test_container(v0));
    std::vector<int> const& v1 = std::vector<int>{1, 2, 3};
    ASSERT(test_container(v1));
    ASSERT(!test_container(int{}));
}