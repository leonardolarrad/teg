#include <array>
#include <vector>
#include <string>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Container concepts") {
    ASSERT((teg::container<std::array<int, 10>>));
    ASSERT((teg::fixed_size_container<std::array<int, 10>>));

    ASSERT(teg::container<std::vector<int>>);
    ASSERT(teg::container<std::string>);    
}