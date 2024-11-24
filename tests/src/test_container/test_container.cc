#include <array>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Check concepts: containers") {
    ASSERT((teg::concepts::container<std::array<int, 10>>));                                                     // array  
    ASSERT((teg::concepts::container<std::vector<std::string>>));                                                // vector
    ASSERT((teg::concepts::container<std::deque<std::string>>));                                                 // deque
    ASSERT((teg::concepts::container<std::list<std::string>>));                                                  // list
    ASSERT((teg::concepts::container<std::map<int, std::string>>));                                              // map
    ASSERT((teg::concepts::container<std::multimap<int, std::string>>));                                         // multimap
    ASSERT((teg::concepts::container<std::set<std::string>>));                                                   // set
    ASSERT((teg::concepts::container<std::multiset<std::string>>));                                              // multiset
    ASSERT((teg::concepts::container<std::basic_string<char>>));                                                 // basic_string
    ASSERT((teg::concepts::container<std::forward_list<std::string>>));                                          // forward_list
    ASSERT((teg::concepts::container<std::unordered_set<int>>));                                                 // unordered_set
    ASSERT((teg::concepts::container<std::unordered_map<int, std::string>>));                                    // unordered_map
    ASSERT((teg::concepts::container<std::unordered_multiset<int>>));                                            // unordered_multiset
    ASSERT((teg::concepts::container<std::unordered_multimap<int, std::string>>));                               // unordered_multimap
    
    // Containers adaptors.
    ASSERT(!(teg::concepts::container<std::stack<std::string>>));                                                // !stack
    ASSERT(!(teg::concepts::container<std::queue<std::string>>));                                                // !queue
    ASSERT(!(teg::concepts::container<std::priority_queue<std::string>>));                                       // !priority_queue

    // Compile-time string.
    ASSERT((teg::concepts::container<teg::basic_fixed_string<char, 10>>));                                       // basic_fixed_string                             
}

TEST_CASE("Check concepts: reversible containers") {
    // Reversible containers.
    ASSERT((teg::concepts::reversible_container<std::array<int, 10>>));                                          // array  
    ASSERT((teg::concepts::reversible_container<std::vector<std::string>>));                                     // vector
    ASSERT((teg::concepts::reversible_container<std::deque<std::string>>));                                      // deque
    ASSERT((teg::concepts::reversible_container<std::list<std::string>>));                                       // list
    ASSERT((teg::concepts::reversible_container<std::map<int, std::string>>));                                   // map
    ASSERT((teg::concepts::reversible_container<std::multimap<int, std::string>>));                              // multimap
    ASSERT((teg::concepts::reversible_container<std::set<std::string>>));                                        // set
    ASSERT((teg::concepts::reversible_container<std::multiset<std::string>>));                                   // multiset
    ASSERT((teg::concepts::reversible_container<std::basic_string<char>>));                                      // basic_string
    ASSERT((teg::concepts::reversible_container<teg::basic_fixed_string<char, 10>>));                            // basic_fixed_string
    // Non reversible containers.
    ASSERT(!(teg::concepts::reversible_container<std::forward_list<std::string>>));                              // !forward_list
    ASSERT(!(teg::concepts::reversible_container<std::unordered_set<int>>));                                     // !unordered_set
    ASSERT(!(teg::concepts::reversible_container<std::unordered_map<int, std::string>>));                        // !unordered_map
    ASSERT(!(teg::concepts::reversible_container<std::unordered_multiset<int>>));                                // !unordered_multiset
    ASSERT(!(teg::concepts::reversible_container<std::unordered_multimap<int, std::string>>));                   // !unordered_multimap
}

TEST_CASE("Check concepts: random access containers") {
    // Random access containers.
    ASSERT((teg::concepts::random_access_container<std::array<int, 10>>));                                       // array
    ASSERT((teg::concepts::random_access_container<std::vector<std::string>>));                                  // vector
    ASSERT((teg::concepts::random_access_container<std::deque<std::string>>));                                   // deque
    ASSERT((teg::concepts::random_access_container<std::basic_string<char>>));                                   // basic_string
    ASSERT((teg::concepts::random_access_container<teg::basic_fixed_string<char, 10>>));                         // basic_fixed_string
    // Non random access containers.
    ASSERT(!(teg::concepts::random_access_container<std::forward_list<std::string>>));                           // !forward_list
    ASSERT(!(teg::concepts::random_access_container<std::list<std::string>>));                                   // !list    
    ASSERT(!(teg::concepts::random_access_container<std::map<int, std::string>>));                               // !map
    ASSERT(!(teg::concepts::random_access_container<std::multimap<int, std::string>>));                          // !multimap
    ASSERT(!(teg::concepts::random_access_container<std::set<std::string>>));                                    // !set
    ASSERT(!(teg::concepts::random_access_container<std::multiset<std::string>>));                               // !multiset
    ASSERT(!(teg::concepts::random_access_container<std::unordered_set<int>>));                                  // !unordered_set
    ASSERT(!(teg::concepts::random_access_container<std::unordered_map<int, std::string>>));                     // !unordered_map
    ASSERT(!(teg::concepts::random_access_container<std::unordered_multiset<int>>));                             // !unordered_multiset
    ASSERT(!(teg::concepts::random_access_container<std::unordered_multimap<int, std::string>>));                // !unordered_multimap
    ASSERT(!(teg::concepts::random_access_container<std::stack<std::string>>));                                  // !stack
    ASSERT(!(teg::concepts::random_access_container<std::queue<std::string>>));                                  // !queue
    ASSERT(!(teg::concepts::random_access_container<std::priority_queue<std::string>>));                         // !priority_queue
}

TEST_CASE("Check concepts: contiguous containers") {
    // Random access containers.
    ASSERT((teg::concepts::contiguous_container<std::array<int, 10>>));                                          // array
    ASSERT((teg::concepts::contiguous_container<std::vector<std::string>>));                                     // vector
    ASSERT((teg::concepts::contiguous_container<std::basic_string<char>>));                                      // basic_string
    ASSERT((teg::concepts::contiguous_container<teg::basic_fixed_string<char, 10>>));                            // basic_fixed_string
    
    // Non random access containers.
    ASSERT(!(teg::concepts::contiguous_container<std::deque<std::string>>));                                     // !deque
    ASSERT(!(teg::concepts::contiguous_container<std::forward_list<std::string>>));                              // !forward_list
    ASSERT(!(teg::concepts::contiguous_container<std::list<std::string>>));                                      // !list    
    ASSERT(!(teg::concepts::contiguous_container<std::map<int, std::string>>));                                  // !map
    ASSERT(!(teg::concepts::contiguous_container<std::multimap<int, std::string>>));                             // !multimap
    ASSERT(!(teg::concepts::contiguous_container<std::set<std::string>>));                                       // !set
    ASSERT(!(teg::concepts::contiguous_container<std::multiset<std::string>>));                                  // !multiset
    ASSERT(!(teg::concepts::contiguous_container<std::unordered_set<int>>));                                     // !unordered_set
    ASSERT(!(teg::concepts::contiguous_container<std::unordered_map<int, std::string>>));                        // !unordered_map
    ASSERT(!(teg::concepts::contiguous_container<std::unordered_multiset<int>>));                                // !unordered_multiset
    ASSERT(!(teg::concepts::contiguous_container<std::unordered_multimap<int, std::string>>));                   // !unordered_multimap
    ASSERT(!(teg::concepts::contiguous_container<std::stack<std::string>>));                                     // !stack
    ASSERT(!(teg::concepts::contiguous_container<std::queue<std::string>>));                                     // !queue
    ASSERT(!(teg::concepts::contiguous_container<std::priority_queue<std::string>>));                            // !priority_queue
}

TEST_CASE("Check concepts: in-place constructing containers") {
    // In-place constructing containers.
    ASSERT((teg::concepts::inplace_constructing_container<std::set<std::string>>));                              // set
    ASSERT((teg::concepts::inplace_constructing_container<std::multiset<std::string>>));                         // multiset
    ASSERT((teg::concepts::inplace_constructing_container<std::unordered_set<int>>));                            // unordered_set
    ASSERT((teg::concepts::inplace_constructing_container<std::map<int, std::string>>));                         // map
    ASSERT((teg::concepts::inplace_constructing_container<std::multimap<int, std::string>>));                    // multimap
    ASSERT((teg::concepts::inplace_constructing_container<std::unordered_map<int, std::string>>));               // unordered_map
    ASSERT((teg::concepts::inplace_constructing_container<std::unordered_multiset<int>>));                       // unordered_multiset
    ASSERT((teg::concepts::inplace_constructing_container<std::unordered_multimap<int, std::string>>));          // unordered_multimap
    // Non in-place constructing containers.
    ASSERT(!(teg::concepts::inplace_constructing_container<std::array<int, 10>>));                               // !array
    ASSERT(!(teg::concepts::inplace_constructing_container<std::vector<std::string>>));                          // !vector
    ASSERT(!(teg::concepts::inplace_constructing_container<std::basic_string<char>>));                           // !basic_string
    ASSERT(!(teg::concepts::inplace_constructing_container<std::deque<std::string>>));                           // !deque
    ASSERT(!(teg::concepts::inplace_constructing_container<std::forward_list<std::string>>));                    // !forward_list
    ASSERT(!(teg::concepts::inplace_constructing_container<std::list<std::string>>));                            // !list    
}

TEST_CASE("Check concepts: back in-place constructing containers") {
    // Back inplace constructing containers.
    ASSERT((teg::concepts::back_inplace_constructing_container<std::vector<std::string>>));                      // vector
    ASSERT((teg::concepts::back_inplace_constructing_container<std::deque<std::string>>));                       // deque
    ASSERT((teg::concepts::back_inplace_constructing_container<std::list<std::string>>));                        // list    
    // Non back inplace constructing containers.
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::array<int, 10>>));                          // !array
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::basic_string<char>>));                      // !basic_string
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::forward_list<std::string>>));               // !forward_list
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::set<std::string>>));                        // !set
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::map<int, std::string>>));                   // !map
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::multiset<std::string>>));                   // !multiset
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::multimap<int, std::string>>));              // !multimap
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::unordered_set<int>>));                      // !unordered_set
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::unordered_map<int, std::string>>));         // !unordered_map
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::unordered_multiset<int>>));                 // !unordered_multiset
    ASSERT(!(teg::concepts::back_inplace_constructing_container<std::unordered_multimap<int, std::string>>));    // !unordered_multimap
}

TEST_CASE("Check concepts: front in-place constructing containers") {
    // Back inplace constructing containers.
    ASSERT((teg::concepts::front_inplace_constructing_container<std::deque<std::string>>));                      // deque
    ASSERT((teg::concepts::front_inplace_constructing_container<std::list<std::string>>));                       // list    
    ASSERT((teg::concepts::front_inplace_constructing_container<std::forward_list<std::string>>));               // forward_list
    // Non back inplace constructing containers
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::vector<std::string>>));                    // !vector
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::array<int, 10>>));                         // !array
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::basic_string<char>>));                     // !basic_string
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::set<std::string>>));                       // !set
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::map<int, std::string>>));                  // !map
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::multiset<std::string>>));                  // !multiset
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::multimap<int, std::string>>));             // !multimap
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::unordered_set<int>>));                     // !unordered_set
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::unordered_map<int, std::string>>));        // !unordered_map
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::unordered_multiset<int>>));                // !unordered_multiset
    ASSERT(!(teg::concepts::front_inplace_constructing_container<std::unordered_multimap<int, std::string>>));   // !unordered_multimap
}

TEST_CASE("Check concepts: fixed size containers") {
    ASSERT((teg::concepts::fixed_size_container<std::array<int, 10>>));                                          // array
    ASSERT((teg::concepts::fixed_size_container<teg::basic_fixed_string<char, 10>>));                            // basic_fixed_string
}

TEST_CASE("Check concepts: associative containers") {
    // Associative containers.
    ASSERT((teg::concepts::associative_container<std::set<std::string>>));                                       // set
    ASSERT((teg::concepts::associative_container<std::map<int, std::string>>));                                  // map
    ASSERT((teg::concepts::associative_container<std::multiset<std::string>>));                                  // multiset
    ASSERT((teg::concepts::associative_container<std::multimap<int, std::string>>));                             // multimap
    ASSERT((teg::concepts::associative_container<std::unordered_set<int>>));                                     // unordered_set
    ASSERT((teg::concepts::associative_container<std::unordered_map<int, std::string>>));                        // unordered_map
    ASSERT((teg::concepts::associative_container<std::unordered_multiset<int>>));                                // unordered_multiset
    ASSERT((teg::concepts::associative_container<std::unordered_multimap<int, std::string>>));                   // unordered_multimap
    // Associative containers.
    ASSERT(!(teg::concepts::associative_container<std::vector<std::string>>));                                   // !vector
    ASSERT(!(teg::concepts::associative_container<std::array<int, 10>>));                                        // !array
    ASSERT(!(teg::concepts::associative_container<std::deque<std::string>>));                                    // !deque
    ASSERT(!(teg::concepts::associative_container<std::list<std::string>>));                                     // !list    
    ASSERT(!(teg::concepts::associative_container<std::forward_list<std::string>>));                             // !forward_list
    ASSERT(!(teg::concepts::associative_container<std::basic_string<char>>));                                    // !basic_string
}

TEST_CASE("Check concepts: set containers") {
    // Set containers.
    ASSERT((teg::concepts::set_container<std::set<std::string>>));                                               // set
    ASSERT((teg::concepts::set_container<std::multiset<std::string>>));                                          // multiset
    ASSERT((teg::concepts::set_container<std::unordered_set<int>>));                                             // unordered_set
    ASSERT((teg::concepts::set_container<std::unordered_multiset<int>>));                                        // unordered_multiset
    // Non set containers.                  
    ASSERT(!(teg::concepts::set_container<std::map<int, std::string>>));                                         // !map
    ASSERT(!(teg::concepts::set_container<std::multimap<int, std::string>>));                                    // !multimap
    ASSERT(!(teg::concepts::set_container<std::unordered_map<int, std::string>>));                               // !unordered_map
    ASSERT(!(teg::concepts::set_container<std::unordered_multimap<int, std::string>>));                          // !unordered_multimap
    ASSERT(!(teg::concepts::set_container<std::vector<std::string>>));                                           // !vector
    ASSERT(!(teg::concepts::set_container<std::array<int, 10>>));                                                // !array
    ASSERT(!(teg::concepts::set_container<std::deque<std::string>>));                                            // !deque
    ASSERT(!(teg::concepts::set_container<std::list<std::string>>));                                             // !list    
    ASSERT(!(teg::concepts::set_container<std::forward_list<std::string>>));                                     // !forward_list
    ASSERT(!(teg::concepts::set_container<std::basic_string<char>>));                                            // !basic_string
}

TEST_CASE("De/serialize std::array") {    
    teg::byte_array b;
    std::array<int, 2> arr0 = { 1, 2 };
    teg::serialize(b, arr0).or_throw();

    std::array<int, 2> arr1;
    teg::deserialize(b, arr1).or_throw();
    ASSERT_EQ(std::memcmp(arr0.data(), arr1.data(), sizeof(arr0)), 0);    
}

TEST_CASE("De/serialize std::vector") {    
    teg::byte_array b;
    std::vector<int> v0 = { 1, 2, 3, 4, 5 };
    teg::serialize(b, v0).or_throw();

    std::vector<int> v1;
    teg::deserialize(b, v1).or_throw();
    
    ASSERT_EQ(std::memcmp(v0.data(), v1.data(), v0.size() * sizeof(int)), 0);
    ASSERT(v0 == v1);    
}

TEST_CASE("De/serialize std::string") {    
    teg::byte_array b;
    std::string s0 = "Hello World!";
    teg::serialize(b, s0).or_throw();

    std::string s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT_EQ(s0, s1);    
}

TEST_CASE("De/serialize std::deque") {    
    teg::byte_array b;
    std::deque<int> d0 = { 1, 2, 3, 4, 5, 6 };
    teg::serialize(b, d0).or_throw();

    std::deque<int> d1;
    teg::deserialize(b, d1).or_throw();
    ASSERT(d0 == d1);    
}

TEST_CASE("De/serialize std::list") {    
    teg::byte_array b;
    std::list<int> l0 = { 1, 2, 3, 4, 5, 6 };
    teg::serialize(b, l0).or_throw();

    std::list<int> l1;
    teg::deserialize(b, l1).or_throw();
    ASSERT(l0 == l1);    
}

TEST_CASE("De/serialize std::forward_list") {    
    teg::byte_array b;
    std::forward_list<int> fl0 = std::forward_list<int>({ 1, 2, 3, 4, 5, 6 });
    teg::serialize(b, fl0).or_throw();

    std::forward_list<int> fl1;
    teg::deserialize(b, fl1).or_throw();
    ASSERT(fl0 == fl1);    
}

TEST_CASE("Force allocation limit") {
    SECTION("1-byte container size limit") {
        teg::byte_array buffer0{};
        teg::byte_array buffer1{};

        constexpr teg::options opt0 = teg::options::container_size_1b;
        constexpr teg::options opt1 = teg::options::container_size_2b;

        std::vector<uint8_t> v0{};
        for (auto i = 0; i < 255 * 2; ++i) {
            v0.push_back(static_cast<uint8_t>(i));
        }

        auto r0 = teg::serialize<opt0>(buffer0, v0);
        auto r1 = teg::serialize<opt1>(buffer1, v0);

        ASSERT(buffer0.size() == 0);
        ASSERT(teg::failure(r0));
        ASSERT(teg::success(r1));
    }
    SECTION("2-byte container size limit") {
        teg::byte_array buffer0{};
        teg::byte_array buffer1{};

        constexpr teg::options opt0 = teg::options::container_size_2b;
        constexpr teg::options opt1 = teg::options::container_size_4b;

        std::vector<uint8_t> v0{};
        for (auto i = 0; i < 65536 * 2; ++i) {
            v0.push_back(static_cast<uint8_t>(i));
        }

        auto r0 = teg::serialize<opt0>(buffer0, v0);
        auto r1 = teg::serialize<opt1>(buffer1, v0);

        ASSERT(buffer0.size() == 0);
        ASSERT(teg::failure(r0));
        ASSERT(teg::success(r1));
    }
}