#include <optional>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Compatible interface should implement 'std::optional'") {
    SECTION("Assert concepts") {
        constexpr bool cx0 = teg::concepts::compatible<teg::compatible<std::string, 2>>;
        constexpr bool cx1 = teg::concepts::compatible<std::optional<std::string>>;
        COMPTIME_ASSERT(cx0);
        COMPTIME_ASSERT(!cx1);
    }
    SECTION("Value <T> constructor") {
        std::optional<std::string> opt0 = "Hello";
        teg::compatible<std::string, 2> comp0 = "Hello";
        ASSERT(*opt0 == *comp0);
    }
    SECTION("Move semantics") {
        std::optional<std::string> opt0 = "Hello";
        teg::compatible<std::string, 2> comp0 = opt0;
        teg::compatible<std::string, 2> comp1 = std::move(comp0);

        ASSERT(*opt0 != *comp0);
        ASSERT(*opt0 == *comp1);
    }
    SECTION("Null option") {
        std::optional<std::string> opt0;
        teg::compatible<std::string, 2> comp0 = opt0;
        teg::compatible<std::string, 3> comp1 = std::nullopt;
        ASSERT(!comp0.has_value());
        ASSERT(!comp1.has_value());
    }
    SECTION("Compare compatible objects") {
        std::optional<std::string> opt0 = "Hello";
        teg::compatible<std::string, 2> comp0 = "Hello";
        teg::compatible<std::string, 3> comp1 = "Hello";
        ASSERT(*opt0 == *comp0);
        ASSERT(*opt0 == *comp1);
        ASSERT(comp0 != comp1); // Different versions
        
        teg::compatible<std::string, 10> comp2 = "there";
        teg::compatible<std::string, 10> comp3 = "there";
        ASSERT(comp2 == comp3); // Same version
    }
}

TEST_CASE("De/serialization of version compatible objects") {
    SECTION("Plain de/serialization") {
        teg::byte_array b;

        teg::compatible<std::string, 2> comp0 = "Str test";
        teg::serialize(b, comp0).or_throw();

        teg::compatible<std::string, 2> comp1;
        teg::deserialize(b, comp1).or_throw();
        ASSERT(*comp0 == *comp1);
    }
    SECTION("Forward compatibility [Two versions]") {
        teg::byte_array b;

        struct v1 { teg::i32 x; teg::i32 y; };
        struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };

        v1 v0 { 1, 2 };
        teg::serialize(b, v0).or_throw();

        v2 v1;
        teg::deserialize(b, v1).or_throw();
        ASSERT(v0.x == v1.x);
        ASSERT(v0.y == v1.y);
        ASSERT(!v1.z.has_value());
    }
    SECTION("Backward compatibility [Two versions]") {
        teg::byte_array b;

        struct v1 { teg::i32 x; teg::i32 y; };
        struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };

        v2 v0 { 1, 2, 3.14 };
        teg::serialize(b, v0).or_throw();

        v1 v1;
        teg::deserialize(b, v1).or_throw();
        ASSERT(v0.x == v1.x);
        ASSERT(v0.y == v1.y);
    }
    SECTION("Forward compatibility [Three versions]") {
        teg::byte_array b;

        struct v1 { teg::i32 x; teg::i32 y; };
        struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };
        struct v3 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<teg::f64, 3> w; };

        v1 v0 { 1, 2 };
        teg::serialize(b, v0).or_throw();

        v2 v1;
        teg::deserialize(b, v1).or_throw();
        ASSERT(v0.x == v1.x);
        ASSERT(v0.y == v1.y);
        ASSERT(!v1.z.has_value());

        v3 v2;
        teg::deserialize(b, v2).or_throw();
        ASSERT(v0.x == v2.x);
        ASSERT(v0.y == v2.y);
        ASSERT(!v2.z.has_value());
        ASSERT(!v2.w.has_value());
    }
    SECTION("Backward compatibility [Three versions]") {
        teg::byte_array b;

        struct v1 { teg::i32 x; teg::i32 y; };
        struct v2 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; };
        struct v3 { teg::i32 x; teg::i32 y; teg::compatible<teg::f64, 2> z; teg::compatible<teg::f64, 3> w; };

        v3 v0 { 1, 2, 3.14, 4.0 };
        teg::serialize(b, v0).or_throw();
        
        v1 v1;
        teg::deserialize(b, v1).or_throw();
        ASSERT(v0.x == v1.x);
        ASSERT(v0.y == v1.y);
        
        v2 v2;
        teg::deserialize(b, v2).or_throw();
        ASSERT(v0.x == v2.x);
        ASSERT(v0.y == v2.y);
    }
}

