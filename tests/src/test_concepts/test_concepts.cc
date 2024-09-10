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

struct valid_elem {};

TEST_CASE("Concept teg::container") {
    ASSERT((teg::container<std::array<int, 10>>));                                                    // array  
    ASSERT((teg::container<std::vector<valid_elem>>));                                                // vector
    ASSERT((teg::container<std::deque<valid_elem>>));                                                 // deque
    ASSERT((teg::container<std::list<valid_elem>>));                                                  // list
    ASSERT((teg::container<std::map<int, valid_elem>>));                                              // map
    ASSERT((teg::container<std::multimap<int, valid_elem>>));                                         // multimap
    ASSERT((teg::container<std::set<valid_elem>>));                                                   // set
    ASSERT((teg::container<std::multiset<valid_elem>>));                                              // multiset
    ASSERT((teg::container<std::basic_string<char>>));                                                // basic string
    ASSERT((teg::container<std::forward_list<valid_elem>>));                                          // forward_list
    ASSERT((teg::container<std::unordered_set<int>>));                                                // unordered_set
    ASSERT((teg::container<std::unordered_map<int, valid_elem>>));                                    // unordered_map
    ASSERT((teg::container<std::unordered_multiset<int>>));                                           // unordered_multiset
    ASSERT((teg::container<std::unordered_multimap<int, valid_elem>>));                               // unordered_multimap
    
    // Containers adaptors.
    ASSERT(!(teg::container<std::stack<valid_elem>>));                                                // stack
    ASSERT(!(teg::container<std::queue<valid_elem>>));                                                // queue
    ASSERT(!(teg::container<std::priority_queue<valid_elem>>));                                       // priority_queue
}


TEST_CASE("Concept teg::reversible_container") {
    // Reversible containers.
    ASSERT((teg::reversible_container<std::array<int, 10>>));                                          // array  
    ASSERT((teg::reversible_container<std::vector<valid_elem>>));                                      // vector
    ASSERT((teg::reversible_container<std::deque<valid_elem>>));                                       // deque
    ASSERT((teg::reversible_container<std::list<valid_elem>>));                                        // list
    ASSERT((teg::reversible_container<std::map<int, valid_elem>>));                                    // map
    ASSERT((teg::reversible_container<std::multimap<int, valid_elem>>));                               // multimap
    ASSERT((teg::reversible_container<std::set<valid_elem>>));                                         // set
    ASSERT((teg::reversible_container<std::multiset<valid_elem>>));                                    // multiset
    ASSERT((teg::reversible_container<std::basic_string<char>>));                                      // basic string
    // Non reversible containers.
    ASSERT(!(teg::reversible_container<std::forward_list<valid_elem>>));                               // forward_list
    ASSERT(!(teg::reversible_container<std::unordered_set<int>>));                                     // unordered_set
    ASSERT(!(teg::reversible_container<std::unordered_map<int, valid_elem>>));                         // unordered_map
    ASSERT(!(teg::reversible_container<std::unordered_multiset<int>>));                                // unordered_multiset
    ASSERT(!(teg::reversible_container<std::unordered_multimap<int, valid_elem>>));                    // unordered_multimap
}

TEST_CASE("Concept teg::random_access_container") {
    // Random access containers.
    ASSERT((teg::random_access_container<std::array<int, 10>>));                                       // array
    ASSERT((teg::random_access_container<std::vector<valid_elem>>));                                   // vector
    ASSERT((teg::random_access_container<std::deque<valid_elem>>));                                    // deque
    ASSERT((teg::random_access_container<std::basic_string<char>>));                                   // basic string
    // Non random access containe.
    ASSERT(!(teg::random_access_container<std::forward_list<valid_elem>>));                            // forward_list
    ASSERT(!(teg::random_access_container<std::list<valid_elem>>));                                    // list    
    ASSERT(!(teg::random_access_container<std::map<int, valid_elem>>));                                // map
    ASSERT(!(teg::random_access_container<std::multimap<int, valid_elem>>));                           // multimap
    ASSERT(!(teg::random_access_container<std::set<valid_elem>>));                                     // set
    ASSERT(!(teg::random_access_container<std::multiset<valid_elem>>));                                // multiset
    ASSERT(!(teg::random_access_container<std::unordered_set<int>>));                                  // unordered_set
    ASSERT(!(teg::random_access_container<std::unordered_map<int, valid_elem>>));                      // unordered_map
    ASSERT(!(teg::random_access_container<std::unordered_multiset<int>>));                             // unordered_multiset
    ASSERT(!(teg::random_access_container<std::unordered_multimap<int, valid_elem>>));                 // unordered_multimap
    ASSERT(!(teg::random_access_container<std::stack<valid_elem>>));                                   // stack
    ASSERT(!(teg::random_access_container<std::queue<valid_elem>>));                                   // queue
    ASSERT(!(teg::random_access_container<std::priority_queue<valid_elem>>));                          // priority_queue
}

TEST_CASE("Concept teg::contiguous_container") {
    // Random access containers.
    ASSERT((teg::contiguous_container<std::array<int, 10>>));                                          // array
    ASSERT((teg::contiguous_container<std::vector<valid_elem>>));                                      // vector
    ASSERT((teg::contiguous_container<std::basic_string<char>>));                                      // basic string
    // Non random access containers.
    ASSERT(!(teg::contiguous_container<std::deque<valid_elem>>));                                      // deque
    ASSERT(!(teg::contiguous_container<std::forward_list<valid_elem>>));                               // forward_list
    ASSERT(!(teg::contiguous_container<std::list<valid_elem>>));                                       // list    
    ASSERT(!(teg::contiguous_container<std::map<int, valid_elem>>));                                   // map
    ASSERT(!(teg::contiguous_container<std::multimap<int, valid_elem>>));                              // multimap
    ASSERT(!(teg::contiguous_container<std::set<valid_elem>>));                                        // set
    ASSERT(!(teg::contiguous_container<std::multiset<valid_elem>>));                                   // multiset
    ASSERT(!(teg::contiguous_container<std::unordered_set<int>>));                                     // unordered_set
    ASSERT(!(teg::contiguous_container<std::unordered_map<int, valid_elem>>));                         // unordered_map
    ASSERT(!(teg::contiguous_container<std::unordered_multiset<int>>));                                // unordered_multiset
    ASSERT(!(teg::contiguous_container<std::unordered_multimap<int, valid_elem>>));                    // unordered_multimap
    ASSERT(!(teg::contiguous_container<std::stack<valid_elem>>));                                      // stack
    ASSERT(!(teg::contiguous_container<std::queue<valid_elem>>));                                      // queue
    ASSERT(!(teg::contiguous_container<std::priority_queue<valid_elem>>));                             // priority_queue
}

TEST_CASE("Concept teg::inplace_constructing_container") {
    // Inplace constructing containers.
    ASSERT((teg::inplace_constructing_container<std::set<valid_elem>>));                               // set
    ASSERT((teg::inplace_constructing_container<std::map<int, valid_elem>>));                          // map
    ASSERT((teg::inplace_constructing_container<std::multiset<valid_elem>>));                          // multiset
    ASSERT((teg::inplace_constructing_container<std::multimap<int, valid_elem>>));                     // multimap
    ASSERT((teg::inplace_constructing_container<std::unordered_set<int>>));                            // unordered_set
    ASSERT((teg::inplace_constructing_container<std::unordered_map<int, valid_elem>>));                // unordered_map
    ASSERT((teg::inplace_constructing_container<std::unordered_multiset<int>>));                       // unordered_multiset
    ASSERT((teg::inplace_constructing_container<std::unordered_multimap<int, valid_elem>>));           // unordered_multimap
    // Non inplace constructing containers.
    ASSERT(!(teg::inplace_constructing_container<std::array<int, 10>>));                               // array
    ASSERT(!(teg::inplace_constructing_container<std::vector<valid_elem>>));                           // vector
    ASSERT(!(teg::inplace_constructing_container<std::basic_string<char>>));                           // basic string
    ASSERT(!(teg::inplace_constructing_container<std::deque<valid_elem>>));                            // deque
    ASSERT(!(teg::inplace_constructing_container<std::forward_list<valid_elem>>));                     // forward_list
    ASSERT(!(teg::inplace_constructing_container<std::list<valid_elem>>));                             // list    
}

TEST_CASE("Concept teg::back_inplace_constructing_container") {
    // Back inplace constructing containers.
    ASSERT((teg::back_inplace_constructing_container<std::vector<valid_elem>>));                       // vector
    ASSERT((teg::back_inplace_constructing_container<std::deque<valid_elem>>));                        // deque
    ASSERT((teg::back_inplace_constructing_container<std::list<valid_elem>>));                         // list    
    // Non back inplace constructing containers.
    ASSERT(!(teg::back_inplace_constructing_container<std::array<int, 10>>));                          // array
    ASSERT(!(teg::back_inplace_constructing_container<std::basic_string<char>>));                      // basic string
    ASSERT(!(teg::back_inplace_constructing_container<std::forward_list<valid_elem>>));                // forward_list
    ASSERT(!(teg::back_inplace_constructing_container<std::set<valid_elem>>));                         // set
    ASSERT(!(teg::back_inplace_constructing_container<std::map<int, valid_elem>>));                    // map
    ASSERT(!(teg::back_inplace_constructing_container<std::multiset<valid_elem>>));                    // multiset
    ASSERT(!(teg::back_inplace_constructing_container<std::multimap<int, valid_elem>>));               // multimap
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_set<int>>));                      // unordered_set
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_map<int, valid_elem>>));          // unordered_map
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_multiset<int>>));                 // unordered_multiset
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_multimap<int, valid_elem>>));     // unordered_multimap
}

TEST_CASE("Concept teg::back_inplace_constructing_container") {
    // Back inplace constructing containers.
    ASSERT((teg::back_inplace_constructing_container<std::vector<valid_elem>>));                       // vector
    ASSERT((teg::back_inplace_constructing_container<std::deque<valid_elem>>));                        // deque
    ASSERT((teg::back_inplace_constructing_container<std::list<valid_elem>>));                         // list    
    // Non back inplace constructing containers.
    ASSERT(!(teg::back_inplace_constructing_container<std::array<int, 10>>));                          // array
    ASSERT(!(teg::back_inplace_constructing_container<std::basic_string<char>>));                      // basic string
    ASSERT(!(teg::back_inplace_constructing_container<std::forward_list<valid_elem>>));                // forward_list
    ASSERT(!(teg::back_inplace_constructing_container<std::set<valid_elem>>));                         // set
    ASSERT(!(teg::back_inplace_constructing_container<std::map<int, valid_elem>>));                    // map
    ASSERT(!(teg::back_inplace_constructing_container<std::multiset<valid_elem>>));                    // multiset
    ASSERT(!(teg::back_inplace_constructing_container<std::multimap<int, valid_elem>>));               // multimap
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_set<int>>));                      // unordered_set
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_map<int, valid_elem>>));          // unordered_map
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_multiset<int>>));                 // unordered_multiset
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_multimap<int, valid_elem>>));     // unordered_multimap
}

TEST_CASE("Concept teg::front_inplace_constructing_container") {
    // Back inplace constructing containers.
    ASSERT((teg::front_inplace_constructing_container<std::deque<valid_elem>>));                        // deque
    ASSERT((teg::front_inplace_constructing_container<std::list<valid_elem>>));                         // list    
    ASSERT((teg::front_inplace_constructing_container<std::forward_list<valid_elem>>));                 // forward_list
    // Non back inplace constructing containers.
    ASSERT(!(teg::front_inplace_constructing_container<std::vector<valid_elem>>));                      // vector
    ASSERT(!(teg::front_inplace_constructing_container<std::array<int, 10>>));                          // array
    ASSERT(!(teg::front_inplace_constructing_container<std::basic_string<char>>));                      // basic string
    ASSERT(!(teg::front_inplace_constructing_container<std::set<valid_elem>>));                         // set
    ASSERT(!(teg::front_inplace_constructing_container<std::map<int, valid_elem>>));                    // map
    ASSERT(!(teg::front_inplace_constructing_container<std::multiset<valid_elem>>));                    // multiset
    ASSERT(!(teg::front_inplace_constructing_container<std::multimap<int, valid_elem>>));               // multimap
    ASSERT(!(teg::front_inplace_constructing_container<std::unordered_set<int>>));                      // unordered_set
    ASSERT(!(teg::front_inplace_constructing_container<std::unordered_map<int, valid_elem>>));          // unordered_map
    ASSERT(!(teg::front_inplace_constructing_container<std::unordered_multiset<int>>));                 // unordered_multiset
    ASSERT(!(teg::front_inplace_constructing_container<std::unordered_multimap<int, valid_elem>>));     // unordered_multimap
}

TEST_CASE("Concept teg::fixed_size_container") {
    ASSERT((teg::fixed_size_container<std::array<int, 10>>));
}

template <typename T>
concept dynamic_contiguous_container = teg::contiguous_container<T> && (!teg::fixed_size_container<T>);


constexpr std::string overload_resolution(dynamic_contiguous_container auto const& c) {
    return "contiguous_container";
}

constexpr std::string overload_resolution(teg::random_access_container auto const& c) {
    return "random_access_container";
}

constexpr std::string overload_resolution(teg::container auto const& c) {
    return "container";
}

constexpr std::string overload_resolution(auto const& c) {
    return "auto";
}

TEST_CASE("Overload resolution with concepts") {
    constexpr std::array<int, 3> a0 = std::array<int, 3>{1, 2, 3};
    std::vector<int> v0 = std::vector<int>{1, 2, 3};
    std::vector<int> const& v1 = std::vector<int>{1, 2, 3};
    std::string s0 = "Hello, World!";
    ASSERT_EQ(overload_resolution(a0), "random_access_container");
    ASSERT_EQ(overload_resolution(v0), "contiguous_container");
    ASSERT_EQ(overload_resolution(v1), "contiguous_container");
    ASSERT_EQ(overload_resolution(s0), "contiguous_container");
    ASSERT_EQ(overload_resolution(int{}), "auto");
}