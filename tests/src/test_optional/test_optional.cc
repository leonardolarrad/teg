#include <array>
#include <map>
#include <optional>
#include <string>
#include <vector>
#include <memory>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Check concepts") {
    ASSERT((teg::concepts::optional<std::optional<int>>));
    ASSERT(!(teg::concepts::optional<std::nullopt_t>));
}

TEST_CASE("Trivial de/serialization") {    
    SECTION("Optional with value") {
        teg::byte_array b;
        std::optional<int> opt0 = 100;
        teg::serialize(b, opt0).or_throw();

        std::optional<int> opt1;
        teg::deserialize(b, opt1).or_throw();
        ASSERT(opt0 == opt1);
    }
    SECTION("Optional without value") {
        teg::byte_array b;
        std::optional<int> opt0;
        teg::serialize(b, opt0).or_throw();

        std::optional<int> opt1;
        teg::deserialize(b, opt1).or_throw();
        ASSERT(opt0 == opt1);
    }    
}

TEST_CASE("Aggregate de/serialization") {
    SECTION("Optional string") {
        teg::byte_array b;
        std::optional<std::string> opt0 = "hello";
        teg::serialize(b, opt0).or_throw();

        std::optional<std::string> opt1;
        teg::deserialize(b, opt1).or_throw();
        ASSERT(opt0 == opt1);
    }
    SECTION("Optional vector") {
        teg::byte_array b;
        std::optional<std::vector<int>> opt0 = std::vector<int>{ 1, 2, 3, 4, 5 };
        teg::serialize(b, opt0).or_throw();

        std::optional<std::vector<int>> opt1;
        teg::deserialize(b, opt1).or_throw();
        ASSERT(opt0 == opt1);
    }
    SECTION("Optional map") {
        teg::byte_array b;
        std::optional<std::map<std::string, std::string>> opt0 = 
            std::map<std::string, std::string> {
                { "a", "a" }, { "b", "b" }, 
                { "c", "c" }, { "d", "d" },
                { "e", "e" } 
            };
        teg::serialize(b, opt0).or_throw();

        std::optional<std::map<std::string, std::string>> opt1;
        teg::deserialize(b, opt1).or_throw();
        ASSERT(opt0 == opt1);
    }
}

TEST_CASE("Special case: optional owning pointer") {
    SECTION("Unique pointer") {
        teg::byte_array b;
        std::optional<std::unique_ptr<int>> opt0 = std::make_unique<int>(99);
        teg::serialize(b, opt0).or_throw();

        std::optional<std::unique_ptr<int>> opt1;
        teg::deserialize(b, opt1).or_throw();
        
        ASSERT(opt1.has_value());
        ASSERT_EQ(*(opt0.value()), *(opt1.value()));
    }
    SECTION("Shared pointer") {
        teg::byte_array b;
        std::optional<std::shared_ptr<int>> opt0 = std::make_shared<int>(99);
        teg::serialize(b, opt0).or_throw();

        std::optional<std::shared_ptr<int>> opt1;
        teg::deserialize(b, opt1).or_throw();
        
        ASSERT(opt1.has_value());
        ASSERT_EQ(*(opt0.value()), *(opt1.value()));
    }
}