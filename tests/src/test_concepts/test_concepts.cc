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


struct _default {};

struct movable_elem {
    movable_elem() = default;
    movable_elem(const movable_elem&) = delete;
    movable_elem& operator=(const movable_elem&) = delete;
    movable_elem(movable_elem&&) = default;
    movable_elem& operator=(movable_elem&&) = default;
};

struct copyable_elem {
    copyable_elem() = default;
    copyable_elem(const copyable_elem&) = default;
    copyable_elem& operator=(const copyable_elem&) = default;
    copyable_elem(copyable_elem&&) = default;
    copyable_elem& operator=(copyable_elem&&) = default;
};

struct nonmovable_elem {
    nonmovable_elem() = default;
    nonmovable_elem(const nonmovable_elem&) = default;
    nonmovable_elem& operator=(const nonmovable_elem&) = default;
    nonmovable_elem(nonmovable_elem&&) = default;
    nonmovable_elem& operator=(nonmovable_elem&&) = delete;
};


class invalid_elem {
private:
    invalid_elem();
};

TEST_CASE("Basic understanding") {
    std::vector<invalid_elem> vec;
    //vec.resize(1);
    //std::array<int, 5> a = std::array<int, 5>({ 1, 2, 3, 4, 5 });

    ASSERT((std::default_initializable<_default>));
    ASSERT((std::movable<_default>));
    ASSERT((std::copyable<_default>));

    ASSERT((std::movable<movable_elem>));
    ASSERT(!(std::copyable<movable_elem>));

    ASSERT((std::movable<copyable_elem>));
    ASSERT((std::copyable<copyable_elem>));

    ASSERT(!(std::movable<nonmovable_elem>));
    ASSERT(!(std::copyable<nonmovable_elem>));
    ASSERT((std::copy_constructible<nonmovable_elem>));
    ASSERT((std::is_copy_assignable_v<nonmovable_elem>));
}

class valid_elem {
public:
   //valid_elem() = default;
   //~valid_elem() = default;
   //valid_elem(const valid_elem&) = default;
   //valid_elem(valid_elem&&) = default;
   //valid_elem& operator=(const valid_elem&) = default;
   //valid_elem& operator=(valid_elem&&) = default;
   //bool operator==(const valid_elem&) const = default;
};

TEST_CASE("Concept teg::container") {
    ASSERT((teg::container<std::array<int, 10>>));    
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

TEST_CASE("Concept teg::reversible_container") {
    // reversible containers
    ASSERT((teg::reversible_container<std::array<int, 10>>));                              // array  
    ASSERT((teg::reversible_container<std::vector<valid_elem>>));                          // vector
    ASSERT((teg::reversible_container<std::deque<valid_elem>>));                           // deque
    ASSERT((teg::reversible_container<std::list<valid_elem>>));                            // list
    ASSERT((teg::reversible_container<std::map<int, valid_elem>>));                        // map
    ASSERT((teg::reversible_container<std::multimap<int, valid_elem>>));                   // multimap
    ASSERT((teg::reversible_container<std::set<valid_elem>>));                             // set
    ASSERT((teg::reversible_container<std::multiset<valid_elem>>));                        // multiset
    ASSERT((teg::reversible_container<std::basic_string<char>>));                          // basic string
    // non reversible containers
    ASSERT(!(teg::reversible_container<std::forward_list<valid_elem>>));                   // forward_list
    ASSERT(!(teg::reversible_container<std::unordered_set<int>>));                         // unordered_set
    ASSERT(!(teg::reversible_container<std::unordered_map<int, valid_elem>>));             // unordered_map
    ASSERT(!(teg::reversible_container<std::unordered_multiset<int>>));                    // unordered_multiset
    ASSERT(!(teg::reversible_container<std::unordered_multimap<int, valid_elem>>));        // unordered_multimap
}

TEST_CASE("Concept teg::sequence_container") {
    // sequence containers
    ASSERT((teg::sequence_container<std::array<int, 10>>));                             // array*
    ASSERT((teg::sequence_container<std::vector<valid_elem>>));                         // vector
    ASSERT((teg::sequence_container<std::deque<valid_elem>>));                          // deque
    ASSERT((teg::sequence_container<std::forward_list<valid_elem>>));                   // forward_list
    ASSERT((teg::sequence_container<std::list<valid_elem>>));                           // list    
    ASSERT((teg::sequence_container<std::basic_string<char>>));                         // basic string*
    // non sequence containers
    ASSERT(!(teg::sequence_container<std::map<int, valid_elem>>));                       // map
    ASSERT(!(teg::sequence_container<std::multimap<int, valid_elem>>));                  // multimap
    ASSERT(!(teg::sequence_container<std::set<valid_elem>>));                            // set
    ASSERT(!(teg::sequence_container<std::multiset<valid_elem>>));                       // multiset
    ASSERT(!(teg::sequence_container<std::unordered_set<int>>));                         // unordered_set
    ASSERT(!(teg::sequence_container<std::unordered_map<int, valid_elem>>));             // unordered_map
    ASSERT(!(teg::sequence_container<std::unordered_multiset<int>>));                    // unordered_multiset
    ASSERT(!(teg::sequence_container<std::unordered_multimap<int, valid_elem>>));        // unordered_multimap
}

TEST_CASE("Concept teg::fixed_size_container") {
    ASSERT((teg::fixed_size_container<std::array<int, 10>>));
}

template <typename T> requires teg::fixed_size_container<T>
auto overload_resolution(T const& c) {
    return "fixed_size_container";
}

template <typename T> requires teg::container<T>
auto overload_resolution(T const& c) {
    return "container";
}

template <typename T>
auto overload_resolution(T const& c) {
    return "auto";
}

TEST_CASE("Overload resolution with concepts") {
    std::array<int, 3> a0 = std::array<int, 3>{1, 2, 3};
    std::vector<int> v0 = std::vector<int>{1, 2, 3};
    std::vector<int> const& v1 = std::vector<int>{1, 2, 3};
    
    ASSERT_EQ(overload_resolution(a0), "fixed_size_container");
    ASSERT_EQ(overload_resolution(v0), "container");
    ASSERT_EQ(overload_resolution(v1), "container");
    ASSERT_EQ(overload_resolution(int{}), "auto");
}