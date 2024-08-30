#include <array>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "teg/teg.h"
#include "test/test.h"

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
