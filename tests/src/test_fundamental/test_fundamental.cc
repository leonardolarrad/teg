#include <array>
#include <concepts>
#include <cstdint>

#include "teg/teg.h"
#include "test/test.h"

template <std::integral T>
constexpr std::array<T, 4> unsigned_values = {
    std::numeric_limits<T>::min(), 
    std::numeric_limits<T>::max(),
    0, 1
};

template <std::integral T>
constexpr std::array<T, 5> signed_values = {
    std::numeric_limits<T>::min(), 
    std::numeric_limits<T>::max(),
    -1, 0, 1
};

TEST_CASE("Serialize and deserialize integral types") {
    SECTION("Booleans") {
        for (auto b0 : std::array<bool, 2>{false, true}) {
            teg::buffer b;
            teg::serialize(b, b0).or_throw();
            
            bool b1;
            teg::deserialize(b, b1).or_throw();
            ASSERT_EQ(b0, b1);
        }
    }
    SECTION("Characters") {
        for (auto c0 : std::array<char, 4>{'A', 'a', 'Z', 'z'}) {
            teg::buffer b;
            teg::serialize(b, c0).or_throw();
            
            char c1;
            teg::deserialize(b, c1).or_throw();
            ASSERT_EQ(c0, c1);
        }        
    }
    SECTION("8-bits signed integers") {
        for (auto i0 : signed_values<std::int8_t>) {
            teg::buffer b;
            teg::serialize(b, i0).or_throw();
            
            std::int8_t i1;
            teg::deserialize(b, i1).or_throw();
            ASSERT_EQ(i0, i1);
        }
    }
    SECTION("16-bits signed integers") {
        for (auto i0 : signed_values<std::int16_t>) {
            teg::buffer b;
            teg::serialize(b, i0).or_throw();
            
            std::int16_t i1;
            teg::deserialize(b, i1).or_throw();
            ASSERT_EQ(i0, i1);
        }
    }
    SECTION("32-bits signed integers") {
        for (auto i0 : signed_values<std::int32_t>) {
            teg::buffer b;
            teg::serialize(b, i0).or_throw();
            
            std::int32_t i1;
            teg::deserialize(b, i1).or_throw();
            ASSERT_EQ(i0, i1);
        }        
    }
    SECTION("64-bits signed integers") {
        for (auto i0 : signed_values<std::int64_t>) {
            teg::buffer b;
            teg::serialize(b, i0).or_throw();
            
            std::int64_t i1;
            teg::deserialize(b, i1).or_throw();
            ASSERT_EQ(i0, i1);
        }        
    }
    SECTION("8-bits unsigned integers") {
        for (auto i0 : unsigned_values<std::uint8_t>) {
            teg::buffer b;
            teg::serialize(b, i0).or_throw();
            
            std::uint8_t i1;
            teg::deserialize(b, i1).or_throw();
            ASSERT_EQ(i0, i1);
        }        
    }  
    SECTION("16-bits unsigned integers") {
        for (auto i0 : unsigned_values<std::uint16_t>) {
            teg::buffer b;
            teg::serialize(b, i0).or_throw();
            
            std::uint16_t i1;
            teg::deserialize(b, i1).or_throw();
            ASSERT_EQ(i0, i1);
        }
    }
    SECTION("32-bits unsigned integers") {
        for (auto i0 : unsigned_values<std::uint32_t>) {
            teg::buffer b;
            teg::serialize(b, i0).or_throw();
            
            std::uint32_t i1;
            teg::deserialize(b, i1).or_throw();
            ASSERT_EQ(i0, i1);
        }
    }
    SECTION("64-bits unsigned integers") {
        for (auto i0 : unsigned_values<std::uint64_t>) {
            teg::buffer b;
            teg::serialize(b, i0).or_throw();
            
            std::uint64_t i1;
            teg::deserialize(b, i1).or_throw();
            ASSERT_EQ(i0, i1);
        }
    }
}

template <std::floating_point T>
constexpr std::array<T, 6> float_values = {
    std::numeric_limits<T>::min(), 
    std::numeric_limits<T>::max(),
    std::numeric_limits<T>::infinity(),
    -1, 0, 1
};

TEST_CASE("Serialize and deserialize floating point types") {  
    // float
    for (auto f0 : float_values<float>) {
        teg::buffer b;
        teg::serialize(b, f0).or_throw();
        
        float f1;
        teg::deserialize(b, f1).or_throw();
        ASSERT_EQ(f0, f1);
    }
    // double
    for (auto f0 : float_values<double>) {
        teg::buffer b;
        teg::serialize(b, f0).or_throw();
        
        double f1;
        teg::deserialize(b, f1).or_throw();
        ASSERT_EQ(f0, f1);
    }
}

enum class enum_8 : std::uint8_t {
    a = 0, b = 1, c = 2, d = 3
};

enum class enum_16 : std::uint16_t {
    a = 0, b = 1, c = 2, d = 3
};

enum class enum_32 : std::uint32_t {
    a = 0, b = 1, c = 2, d = 3
};

enum class enum_64 : std::uint64_t {
    a = 0, b = 1, c = 2, d = 3
};

TEST_CASE("Serialize and deserialize enum types") {
    SECTION("8 bits enum")
    {
        teg::buffer b;
        enum_8 e0 = enum_8::a;
        teg::serialize(b, e0).or_throw();
        
        enum_8 e1;
        teg::deserialize(b, e1).or_throw();
        ASSERT(e0 == e1);
    }
    SECTION("16 bits enum")
    {
        teg::buffer b;
        enum_16 e0 = enum_16::b;
        teg::serialize(b, e0).or_throw();
        
        enum_16 e1;
        teg::deserialize(b, e1).or_throw();
        ASSERT(e0 == e1);
    }
    SECTION("32 bits enum")
    {
        teg::buffer b;
        enum_32 e0 = enum_32::c;
        teg::serialize(b, e0).or_throw();
        
        enum_32 e1;
        teg::deserialize(b, e1).or_throw();
        ASSERT(e0 == e1);
    }
    SECTION("64 bits enum")
    {
        teg::buffer b;
        enum_64 e0 = enum_64::d;
        teg::serialize(b, e0).or_throw();
        
        enum_64 e1;
        teg::deserialize(b, e1).or_throw();
        ASSERT(e0 == e1);
    }
}