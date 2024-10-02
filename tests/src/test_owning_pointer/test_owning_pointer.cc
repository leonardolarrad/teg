#include <array>
#include <string>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Unique pointer de/serialization") {    
    SECTION("Trivial de/serialization") {
        teg::buffer b;
        std::unique_ptr<int> p0 = std::make_unique<int>(100);
        teg::serialize(b, p0).or_throw();

        std::unique_ptr<int> p1;
        teg::deserialize(b, p1).or_throw();
        ASSERT(*p0 == *p1);        
    }
    SECTION("Aggregate de/serialization") {
        teg::buffer b;
        std::unique_ptr<std::vector<int>> p0 = 
            std::make_unique<std::vector<int>>(std::vector<int>{ 1, 2, 3, 4, 5 });
        teg::serialize(b, p0).or_throw();

        std::unique_ptr<std::vector<int>> p1;
        teg::deserialize(b, p1).or_throw();
        ASSERT(*p0 == *p1);
    }
    SECTION("Unique pointer to unique pointer") {
        teg::buffer b;
        std::unique_ptr<std::unique_ptr<int>> p0 = 
            std::make_unique<std::unique_ptr<int>>(std::make_unique<int>(100));
        teg::serialize(b, p0).or_throw();

        std::unique_ptr<std::unique_ptr<int>> p1;
        teg::deserialize(b, p1).or_throw();
        ASSERT(**p0 == **p1);
    }
}

TEST_CASE("Shared pointer de/serialization") {
    SECTION("Trivial") {
        teg::buffer b;
        std::shared_ptr<int> p0 = std::make_shared<int>(100);
        teg::serialize(b, p0).or_throw();

        std::shared_ptr<int> p1;
        teg::deserialize(b, p1).or_throw();
        ASSERT_EQ(*p0, *p1);    
    }
    SECTION("Non-trivial") {
        teg::buffer b;
        std::shared_ptr<std::vector<int>> p0 = 
            std::make_shared<std::vector<int>>(std::vector<int>{ 1, 2, 3, 4, 5 });
        teg::serialize(b, p0).or_throw();

        std::shared_ptr<std::vector<int>> p1;
        teg::deserialize(b, p1).or_throw();
        ASSERT(*p0 == *p1);
    }
    SECTION("Shared pointer to shared pointer") {
        teg::buffer b;
        std::shared_ptr<std::shared_ptr<int>> p0 = 
            std::make_shared<std::shared_ptr<int>>(std::make_shared<int>(100));
        teg::serialize(b, p0).or_throw();

        std::shared_ptr<std::shared_ptr<int>> p1;
        teg::deserialize(b, p1).or_throw();
        ASSERT(**p0 == **p1);
    }
}

