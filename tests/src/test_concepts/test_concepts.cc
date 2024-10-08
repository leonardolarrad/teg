#include <any>
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

struct valid_elem {};

TEST_CASE("Optional types") {
    ASSERT((teg::optional<std::optional<int>>));
    ASSERT((teg::optional<std::optional<int> const&>));
}

TEST_CASE("Owning pointers") {
    ASSERT((teg::owning_pointer<std::unique_ptr<valid_elem>>));
    ASSERT((teg::owning_pointer<std::shared_ptr<valid_elem>>));
    ASSERT(!(teg::owning_pointer<std::weak_ptr<valid_elem>>));
    ASSERT(!(teg::owning_pointer<valid_elem*>));
    ASSERT(!(teg::owning_pointer<valid_elem const*>));
}

TEST_CASE("Variants") {
    ASSERT((teg::variant<std::variant<int, float>>));
    ASSERT(!(teg::variant<std::any>));
}

TEST_CASE("Tuples") {
    ASSERT((teg::tuple<std::tuple<int, float, char>>));
    // Pairs are special cases of tuples.
    ASSERT((teg::tuple<std::pair<int, float>>));
    // Arrays share the tuple interface as well.
    ASSERT((teg::tuple<std::array<int, 10>>));
}

TEST_CASE("Containers") {
    ASSERT((teg::container<std::array<int, 10>>));                                                    // array  
    ASSERT((teg::container<std::vector<valid_elem>>));                                                // vector
    ASSERT((teg::container<std::deque<valid_elem>>));                                                 // deque
    ASSERT((teg::container<std::list<valid_elem>>));                                                  // list
    ASSERT((teg::container<std::map<int, valid_elem>>));                                              // map
    ASSERT((teg::container<std::multimap<int, valid_elem>>));                                         // multimap
    ASSERT((teg::container<std::set<valid_elem>>));                                                   // set
    ASSERT((teg::container<std::multiset<valid_elem>>));                                              // multiset
    ASSERT((teg::container<std::basic_string<char>>));                                                // basic_string
    ASSERT((teg::container<std::forward_list<valid_elem>>));                                          // forward_list
    ASSERT((teg::container<std::unordered_set<int>>));                                                // unordered_set
    ASSERT((teg::container<std::unordered_map<int, valid_elem>>));                                    // unordered_map
    ASSERT((teg::container<std::unordered_multiset<int>>));                                           // unordered_multiset
    ASSERT((teg::container<std::unordered_multimap<int, valid_elem>>));                               // unordered_multimap
    
    // Containers adaptors.
    ASSERT(!(teg::container<std::stack<valid_elem>>));                                                // !stack
    ASSERT(!(teg::container<std::queue<valid_elem>>));                                                // !queue
    ASSERT(!(teg::container<std::priority_queue<valid_elem>>));                                       // !priority_queue

    // Compile-time string.
    ASSERT((teg::container<teg::basic_fixed_string<char, 10>>));                                      // basic_fixed_string                             
}

TEST_CASE("Reversible containers") {
    // Reversible containers.
    ASSERT((teg::reversible_container<std::array<int, 10>>));                                          // array  
    ASSERT((teg::reversible_container<std::vector<valid_elem>>));                                      // vector
    ASSERT((teg::reversible_container<std::deque<valid_elem>>));                                       // deque
    ASSERT((teg::reversible_container<std::list<valid_elem>>));                                        // list
    ASSERT((teg::reversible_container<std::map<int, valid_elem>>));                                    // map
    ASSERT((teg::reversible_container<std::multimap<int, valid_elem>>));                               // multimap
    ASSERT((teg::reversible_container<std::set<valid_elem>>));                                         // set
    ASSERT((teg::reversible_container<std::multiset<valid_elem>>));                                    // multiset
    ASSERT((teg::reversible_container<std::basic_string<char>>));                                      // basic_string
    ASSERT((teg::reversible_container<teg::basic_fixed_string<char, 10>>));                            // basic_fixed_string
    // Non reversible containers.
    ASSERT(!(teg::reversible_container<std::forward_list<valid_elem>>));                               // !forward_list
    ASSERT(!(teg::reversible_container<std::unordered_set<int>>));                                     // !unordered_set
    ASSERT(!(teg::reversible_container<std::unordered_map<int, valid_elem>>));                         // !unordered_map
    ASSERT(!(teg::reversible_container<std::unordered_multiset<int>>));                                // !unordered_multiset
    ASSERT(!(teg::reversible_container<std::unordered_multimap<int, valid_elem>>));                    // !unordered_multimap
}

TEST_CASE("Random access containers") {
    // Random access containers.
    ASSERT((teg::random_access_container<std::array<int, 10>>));                                       // array
    ASSERT((teg::random_access_container<std::vector<valid_elem>>));                                   // vector
    ASSERT((teg::random_access_container<std::deque<valid_elem>>));                                    // deque
    ASSERT((teg::random_access_container<std::basic_string<char>>));                                   // basic_string
    ASSERT((teg::random_access_container<teg::basic_fixed_string<char, 10>>));                         // basic_fixed_string
    // Non random access containers.
    ASSERT(!(teg::random_access_container<std::forward_list<valid_elem>>));                            // !forward_list
    ASSERT(!(teg::random_access_container<std::list<valid_elem>>));                                    // !list    
    ASSERT(!(teg::random_access_container<std::map<int, valid_elem>>));                                // !map
    ASSERT(!(teg::random_access_container<std::multimap<int, valid_elem>>));                           // !multimap
    ASSERT(!(teg::random_access_container<std::set<valid_elem>>));                                     // !set
    ASSERT(!(teg::random_access_container<std::multiset<valid_elem>>));                                // !multiset
    ASSERT(!(teg::random_access_container<std::unordered_set<int>>));                                  // !unordered_set
    ASSERT(!(teg::random_access_container<std::unordered_map<int, valid_elem>>));                      // !unordered_map
    ASSERT(!(teg::random_access_container<std::unordered_multiset<int>>));                             // !unordered_multiset
    ASSERT(!(teg::random_access_container<std::unordered_multimap<int, valid_elem>>));                 // !unordered_multimap
    ASSERT(!(teg::random_access_container<std::stack<valid_elem>>));                                   // !stack
    ASSERT(!(teg::random_access_container<std::queue<valid_elem>>));                                   // !queue
    ASSERT(!(teg::random_access_container<std::priority_queue<valid_elem>>));                          // !priority_queue
}

TEST_CASE("Contiguous containers") {
    // Random access containers.
    ASSERT((teg::contiguous_container<std::array<int, 10>>));                                          // array
    ASSERT((teg::contiguous_container<std::vector<valid_elem>>));                                      // vector
    ASSERT((teg::contiguous_container<std::basic_string<char>>));                                      // basic_string
    ASSERT((teg::contiguous_container<teg::basic_fixed_string<char, 10>>));                            // basic_fixed_string
    
    // Non random access containers.
    ASSERT(!(teg::contiguous_container<std::deque<valid_elem>>));                                      // !deque
    ASSERT(!(teg::contiguous_container<std::forward_list<valid_elem>>));                               // !forward_list
    ASSERT(!(teg::contiguous_container<std::list<valid_elem>>));                                       // !list    
    ASSERT(!(teg::contiguous_container<std::map<int, valid_elem>>));                                   // !map
    ASSERT(!(teg::contiguous_container<std::multimap<int, valid_elem>>));                              // !multimap
    ASSERT(!(teg::contiguous_container<std::set<valid_elem>>));                                        // !set
    ASSERT(!(teg::contiguous_container<std::multiset<valid_elem>>));                                   // !multiset
    ASSERT(!(teg::contiguous_container<std::unordered_set<int>>));                                     // !unordered_set
    ASSERT(!(teg::contiguous_container<std::unordered_map<int, valid_elem>>));                         // !unordered_map
    ASSERT(!(teg::contiguous_container<std::unordered_multiset<int>>));                                // !unordered_multiset
    ASSERT(!(teg::contiguous_container<std::unordered_multimap<int, valid_elem>>));                    // !unordered_multimap
    ASSERT(!(teg::contiguous_container<std::stack<valid_elem>>));                                      // !stack
    ASSERT(!(teg::contiguous_container<std::queue<valid_elem>>));                                      // !queue
    ASSERT(!(teg::contiguous_container<std::priority_queue<valid_elem>>));                             // !priority_queue
}

TEST_CASE("In-place constructing containers") {
    // In-place constructing containers.
    ASSERT((teg::inplace_constructing_container<std::set<valid_elem>>));                               // set
    ASSERT((teg::inplace_constructing_container<std::multiset<valid_elem>>));                          // multiset
    ASSERT((teg::inplace_constructing_container<std::unordered_set<int>>));                            // unordered_set
    ASSERT((teg::inplace_constructing_container<std::map<int, valid_elem>>));                          // map
    ASSERT((teg::inplace_constructing_container<std::multimap<int, valid_elem>>));                     // multimap
    ASSERT((teg::inplace_constructing_container<std::unordered_map<int, valid_elem>>));                // unordered_map
    ASSERT((teg::inplace_constructing_container<std::unordered_multiset<int>>));                       // unordered_multiset
    ASSERT((teg::inplace_constructing_container<std::unordered_multimap<int, valid_elem>>));           // unordered_multimap
    // Non in-place constructing containers.
    ASSERT(!(teg::inplace_constructing_container<std::array<int, 10>>));                               // !array
    ASSERT(!(teg::inplace_constructing_container<std::vector<valid_elem>>));                           // !vector
    ASSERT(!(teg::inplace_constructing_container<std::basic_string<char>>));                           // !basic_string
    ASSERT(!(teg::inplace_constructing_container<std::deque<valid_elem>>));                            // !deque
    ASSERT(!(teg::inplace_constructing_container<std::forward_list<valid_elem>>));                     // !forward_list
    ASSERT(!(teg::inplace_constructing_container<std::list<valid_elem>>));                             // !list    
}

TEST_CASE("Back in-place constructing containers") {
    // Back inplace constructing containers.
    ASSERT((teg::back_inplace_constructing_container<std::vector<valid_elem>>));                       // vector
    ASSERT((teg::back_inplace_constructing_container<std::deque<valid_elem>>));                        // deque
    ASSERT((teg::back_inplace_constructing_container<std::list<valid_elem>>));                         // list    
    // Non back inplace constructing containers.
    ASSERT(!(teg::back_inplace_constructing_container<std::array<int, 10>>));                          // !array
    ASSERT(!(teg::back_inplace_constructing_container<std::basic_string<char>>));                      // !basic_string
    ASSERT(!(teg::back_inplace_constructing_container<std::forward_list<valid_elem>>));                // !forward_list
    ASSERT(!(teg::back_inplace_constructing_container<std::set<valid_elem>>));                         // !set
    ASSERT(!(teg::back_inplace_constructing_container<std::map<int, valid_elem>>));                    // !map
    ASSERT(!(teg::back_inplace_constructing_container<std::multiset<valid_elem>>));                    // !multiset
    ASSERT(!(teg::back_inplace_constructing_container<std::multimap<int, valid_elem>>));               // !multimap
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_set<int>>));                      // !unordered_set
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_map<int, valid_elem>>));          // !unordered_map
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_multiset<int>>));                 // !unordered_multiset
    ASSERT(!(teg::back_inplace_constructing_container<std::unordered_multimap<int, valid_elem>>));     // !unordered_multimap
}

TEST_CASE("Front in-place constructing containers") {
    // Back inplace constructing containers.
    ASSERT((teg::front_inplace_constructing_container<std::deque<valid_elem>>));                       // deque
    ASSERT((teg::front_inplace_constructing_container<std::list<valid_elem>>));                        // list    
    ASSERT((teg::front_inplace_constructing_container<std::forward_list<valid_elem>>));                // forward_list
    // Non back inplace constructing containers
    ASSERT(!(teg::front_inplace_constructing_container<std::vector<valid_elem>>));                     // !vector
    ASSERT(!(teg::front_inplace_constructing_container<std::array<int, 10>>));                         // !array
    ASSERT(!(teg::front_inplace_constructing_container<std::basic_string<char>>));                     // !basic_string
    ASSERT(!(teg::front_inplace_constructing_container<std::set<valid_elem>>));                        // !set
    ASSERT(!(teg::front_inplace_constructing_container<std::map<int, valid_elem>>));                   // !map
    ASSERT(!(teg::front_inplace_constructing_container<std::multiset<valid_elem>>));                   // !multiset
    ASSERT(!(teg::front_inplace_constructing_container<std::multimap<int, valid_elem>>));              // !multimap
    ASSERT(!(teg::front_inplace_constructing_container<std::unordered_set<int>>));                     // !unordered_set
    ASSERT(!(teg::front_inplace_constructing_container<std::unordered_map<int, valid_elem>>));         // !unordered_map
    ASSERT(!(teg::front_inplace_constructing_container<std::unordered_multiset<int>>));                // !unordered_multiset
    ASSERT(!(teg::front_inplace_constructing_container<std::unordered_multimap<int, valid_elem>>));    // !unordered_multimap
}

TEST_CASE("Fixed size containers") {
    ASSERT((teg::fixed_size_container<std::array<int, 10>>));                                          // array
    ASSERT((teg::fixed_size_container<teg::basic_fixed_string<char, 10>>));                            // basic_fixed_string
}

TEST_CASE("Associative containers") {
    // Associative containers.
    ASSERT((teg::associative_container<std::set<valid_elem>>));                                        // set
    ASSERT((teg::associative_container<std::map<int, valid_elem>>));                                   // map
    ASSERT((teg::associative_container<std::multiset<valid_elem>>));                                   // multiset
    ASSERT((teg::associative_container<std::multimap<int, valid_elem>>));                              // multimap
    ASSERT((teg::associative_container<std::unordered_set<int>>));                                     // unordered_set
    ASSERT((teg::associative_container<std::unordered_map<int, valid_elem>>));                         // unordered_map
    ASSERT((teg::associative_container<std::unordered_multiset<int>>));                                // unordered_multiset
    ASSERT((teg::associative_container<std::unordered_multimap<int, valid_elem>>));                    // unordered_multimap
    // Associative containers.
    ASSERT(!(teg::associative_container<std::vector<valid_elem>>));                                    // !vector
    ASSERT(!(teg::associative_container<std::array<int, 10>>));                                        // !array
    ASSERT(!(teg::associative_container<std::deque<valid_elem>>));                                     // !deque
    ASSERT(!(teg::associative_container<std::list<valid_elem>>));                                      // !list    
    ASSERT(!(teg::associative_container<std::forward_list<valid_elem>>));                              // !forward_list
    ASSERT(!(teg::associative_container<std::basic_string<char>>));                                    // !basic_string
}

TEST_CASE("Set containers") {
    // Set containers.
    ASSERT((teg::set_container<std::set<valid_elem>>));                                                // set
    ASSERT((teg::set_container<std::multiset<valid_elem>>));                                           // multiset
    ASSERT((teg::set_container<std::unordered_set<int>>));                                             // unordered_set
    ASSERT((teg::set_container<std::unordered_multiset<int>>));                                        // unordered_multiset
    // Non set containers.                  
    ASSERT(!(teg::set_container<std::map<int, valid_elem>>));                                          // !map
    ASSERT(!(teg::set_container<std::multimap<int, valid_elem>>));                                     // !multimap
    ASSERT(!(teg::set_container<std::unordered_map<int, valid_elem>>));                                // !unordered_map
    ASSERT(!(teg::set_container<std::unordered_multimap<int, valid_elem>>));                           // !unordered_multimap
    ASSERT(!(teg::set_container<std::vector<valid_elem>>));                                            // !vector
    ASSERT(!(teg::set_container<std::array<int, 10>>));                                                // !array
    ASSERT(!(teg::set_container<std::deque<valid_elem>>));                                             // !deque
    ASSERT(!(teg::set_container<std::list<valid_elem>>));                                              // !list    
    ASSERT(!(teg::set_container<std::forward_list<valid_elem>>));                                      // !forward_list
    ASSERT(!(teg::set_container<std::basic_string<char>>));                                            // !basic_string
}
