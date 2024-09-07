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
    ASSERT(!(teg::container<std::stack<valid_elem>>));                                                // stack
    ASSERT(!(teg::container<std::queue<valid_elem>>));                                                // queue
    ASSERT(!(teg::container<std::priority_queue<valid_elem>>));                                       // priority_queue
}


TEST_CASE("Concept teg::reversible_container") {
    // reversible containers
    ASSERT((teg::reversible_container<std::array<int, 10>>));                                          // array  
    ASSERT((teg::reversible_container<std::vector<valid_elem>>));                                      // vector
    ASSERT((teg::reversible_container<std::deque<valid_elem>>));                                       // deque
    ASSERT((teg::reversible_container<std::list<valid_elem>>));                                        // list
    ASSERT((teg::reversible_container<std::map<int, valid_elem>>));                                    // map
    ASSERT((teg::reversible_container<std::multimap<int, valid_elem>>));                               // multimap
    ASSERT((teg::reversible_container<std::set<valid_elem>>));                                         // set
    ASSERT((teg::reversible_container<std::multiset<valid_elem>>));                                    // multiset
    ASSERT((teg::reversible_container<std::basic_string<char>>));                                      // basic string
    // non reversible containers
    ASSERT(!(teg::reversible_container<std::forward_list<valid_elem>>));                               // forward_list
    ASSERT(!(teg::reversible_container<std::unordered_set<int>>));                                     // unordered_set
    ASSERT(!(teg::reversible_container<std::unordered_map<int, valid_elem>>));                         // unordered_map
    ASSERT(!(teg::reversible_container<std::unordered_multiset<int>>));                                // unordered_multiset
    ASSERT(!(teg::reversible_container<std::unordered_multimap<int, valid_elem>>));                    // unordered_multimap
}

TEST_CASE("Concept teg::sequence_container") {
    // sequence containers
    ASSERT((teg::sequence_container<std::array<int, 10>>));                                            // array*
    ASSERT((teg::sequence_container<std::vector<valid_elem>>));                                        // vector
    ASSERT((teg::sequence_container<std::deque<valid_elem>>));                                         // deque
    ASSERT((teg::sequence_container<std::forward_list<valid_elem>>));                                  // forward_list
    ASSERT((teg::sequence_container<std::list<valid_elem>>));                                          // list    
    ASSERT((teg::sequence_container<std::basic_string<char>>));                                        // basic string*
    // non sequence containers   
    ASSERT(!(teg::sequence_container<std::map<int, valid_elem>>));                                     // map
    ASSERT(!(teg::sequence_container<std::multimap<int, valid_elem>>));                                // multimap
    ASSERT(!(teg::sequence_container<std::set<valid_elem>>));                                          // set
    ASSERT(!(teg::sequence_container<std::multiset<valid_elem>>));                                     // multiset
    ASSERT(!(teg::sequence_container<std::unordered_set<int>>));                                       // unordered_set
    ASSERT(!(teg::sequence_container<std::unordered_map<int, valid_elem>>));                           // unordered_map
    ASSERT(!(teg::sequence_container<std::unordered_multiset<int>>));                                  // unordered_multiset
    ASSERT(!(teg::sequence_container<std::unordered_multimap<int, valid_elem>>));                      // unordered_multimap
}

TEST_CASE("Concept teg::random_access_container") {
    // random access containers
    ASSERT((teg::random_access_container<std::array<int, 10>>));                                       // array
    ASSERT((teg::random_access_container<std::vector<valid_elem>>));                                   // vector
    ASSERT((teg::random_access_container<std::deque<valid_elem>>));                                    // deque
    ASSERT((teg::random_access_container<std::basic_string<char>>));                                   // basic string
    // non random access containe
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
    // random access containers
    ASSERT((teg::contiguous_container<std::array<int, 10>>));                                          // array
    ASSERT((teg::contiguous_container<std::vector<valid_elem>>));                                      // vector
    ASSERT((teg::contiguous_container<std::basic_string<char>>));                                      // basic string
    // non random access containers
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
    // inplace constructing containers
    ASSERT((teg::inplace_constructing_container<std::set<valid_elem>>));                               // set
    ASSERT((teg::inplace_constructing_container<std::map<int, valid_elem>>));                          // map
    ASSERT((teg::inplace_constructing_container<std::multiset<valid_elem>>));                          // multiset
    ASSERT((teg::inplace_constructing_container<std::multimap<int, valid_elem>>));                     // multimap
    ASSERT((teg::inplace_constructing_container<std::unordered_set<int>>));                            // unordered_set
    ASSERT((teg::inplace_constructing_container<std::unordered_map<int, valid_elem>>));                // unordered_map
    ASSERT((teg::inplace_constructing_container<std::unordered_multiset<int>>));                       // unordered_multiset
    ASSERT((teg::inplace_constructing_container<std::unordered_multimap<int, valid_elem>>));           // unordered_multimap
    // non inplace constructing containers
    ASSERT(!(teg::inplace_constructing_container<std::array<int, 10>>));                               // array
    ASSERT(!(teg::inplace_constructing_container<std::vector<valid_elem>>));                           // vector
    ASSERT(!(teg::inplace_constructing_container<std::basic_string<char>>));                           // basic string
    ASSERT(!(teg::inplace_constructing_container<std::deque<valid_elem>>));                            // deque
    ASSERT(!(teg::inplace_constructing_container<std::forward_list<valid_elem>>));                     // forward_list
    ASSERT(!(teg::inplace_constructing_container<std::list<valid_elem>>));                             // list    
}

TEST_CASE("Concept teg::back_inplace_constructing_container") {
    // back inplace constructing containers
    ASSERT((teg::back_inplace_constructing_container<std::vector<valid_elem>>));                       // vector
    ASSERT((teg::back_inplace_constructing_container<std::deque<valid_elem>>));                        // deque
    ASSERT((teg::back_inplace_constructing_container<std::list<valid_elem>>));                         // list    
    // non back inplace constructing containers
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
    // back inplace constructing containers
    ASSERT((teg::back_inplace_constructing_container<std::vector<valid_elem>>));                       // vector
    ASSERT((teg::back_inplace_constructing_container<std::deque<valid_elem>>));                        // deque
    ASSERT((teg::back_inplace_constructing_container<std::list<valid_elem>>));                         // list    
    // non back inplace constructing containers
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
    // back inplace constructing containers
    ASSERT((teg::front_inplace_constructing_container<std::deque<valid_elem>>));                        // deque
    ASSERT((teg::front_inplace_constructing_container<std::list<valid_elem>>));                         // list    
    ASSERT((teg::front_inplace_constructing_container<std::forward_list<valid_elem>>));                 // forward_list
    // non back inplace constructing containers
    ASSERT(!(teg::front_inplace_constructing_container<std::vector<valid_elem>>));                       // vector
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

template<teg::fixed_size_container T>
constexpr std::string overload_resolution(T const& c) {
    return "fixed_size_container";
}

template<teg::container T>
constexpr std::string overload_resolution(T const& c) {
    return "container";
}

template<typename T>
constexpr std::string overload_resolution(T const& c) {
    return "auto";
}

TEST_CASE("Overload resolution with concepts") {
    constexpr std::array<int, 3> a0 = std::array<int, 3>{1, 2, 3};
    std::vector<int> v0 = std::vector<int>{1, 2, 3};
    std::vector<int> const& v1 = std::vector<int>{1, 2, 3};
    v0.emplace(v0.end(), 4);
    ASSERT_EQ(overload_resolution(a0), "fixed_size_container");
    ASSERT_EQ(overload_resolution(v0), "container");
    ASSERT_EQ(overload_resolution(v1), "container");
    ASSERT_EQ(overload_resolution(int{}), "auto");
}