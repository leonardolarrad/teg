#include <vector>
#include <string>
#include <array>
#include <variant>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Debug") {    
    std::variant<int, float> v0 = 100;
    std::variant<int, float> v1 = 100.0f;

    ASSERT(std::holds_alternative<int>(v0));
}
