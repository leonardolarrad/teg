#include <array>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("debug") {
    class c_elem {
    public:
        c_elem() = default;
        ~c_elem() = default;
        c_elem(const c_elem&) = default;
        c_elem(c_elem&&) = default;
        c_elem& operator=(const c_elem&) = delete;
        c_elem& operator=(c_elem&&) = delete;
    };

    std::vector<c_elem> v0;
}

TEST_CASE("De/serialize std::array") {
    {
        teg::buffer b;
        std::array<int, 2> arr0 = { 1, 2 };
        teg::serialize(b, arr0).or_throw();

        std::array<int, 2> arr1;
        teg::deserialize(b, arr1).or_throw();
        ASSERT_EQ(std::memcmp(arr0.data(), arr1.data(), sizeof(arr0)), 0);
    }
}

TEST_CASE("De/serialize std::vector") {
    {
        teg::buffer b;
        std::vector<int> v0 = { 1, 2, 3, 4, 5 };
        teg::serialize(b, v0).or_throw();

        std::vector<int> v1;
        teg::deserialize(b, v1).or_throw();
        ASSERT_EQ(std::memcmp(v0.data(), v1.data(), sizeof(v0)), 0);
    }
}

//TEST_CASE("De/serialize std::string") {
//    {
//        teg::buffer b;
//        std::string s0 = "hello";
//        teg::serialize(b, s0).or_throw();
//
//        std::string s1;
//        teg::deserialize(b, s1).or_throw();
//        ASSERT_EQ(s0, s1);
//    }
//}