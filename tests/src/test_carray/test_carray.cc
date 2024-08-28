#include <cstdint>
#include <concepts>
#include <array>
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

TEST_CASE("Serialize and deserialize c arrays") {
    // bool
    for (auto b0 : std::array<bool, 2>{false, true}) {
        teg::buffer b;
        teg::serialize(b, b0).or_throw();
        
        bool b1;
        teg::deserialize(b, b1).or_throw();
        ASSERT_EQ(b0, b1);
    }
    // char
    for (auto c0 : std::array<char, 4>{'A', 'a', 'Z', 'z'}) {
        teg::buffer b;
        teg::serialize(b, c0).or_throw();
        
        char c1;
        teg::deserialize(b, c1).or_throw();
        ASSERT_EQ(c0, c1);
    }
    // int8_t    
    for (auto i0 : signed_values<std::int8_t>) {
        teg::buffer b;
        teg::serialize(b, i0).or_throw();
        
        std::int8_t i1;
        teg::deserialize(b, i1).or_throw();
        ASSERT_EQ(i0, i1);
    }
    // int16_t
    for (auto i0 : signed_values<std::int16_t>) {
        teg::buffer b;
        teg::serialize(b, i0).or_throw();
        
        std::int16_t i1;
        teg::deserialize(b, i1).or_throw();
        ASSERT_EQ(i0, i1);
    }
    // int32_t    
    for (auto i0 : signed_values<std::int32_t>) {
        teg::buffer b;
        teg::serialize(b, i0).or_throw();
        
        std::int32_t i1;
        teg::deserialize(b, i1).or_throw();
        ASSERT_EQ(i0, i1);
    }
    // int64_t
    for (auto i0 : signed_values<std::int64_t>) {
        teg::buffer b;
        teg::serialize(b, i0).or_throw();
        
        std::int64_t i1;
        teg::deserialize(b, i1).or_throw();
        ASSERT_EQ(i0, i1);
    }
    // uint8_t    
    for (auto i0 : unsigned_values<std::uint8_t>) {
        teg::buffer b;
        teg::serialize(b, i0).or_throw();
        
        std::uint8_t i1;
        teg::deserialize(b, i1).or_throw();
        ASSERT_EQ(i0, i1);
    }
    // uint16_t
    for (auto i0 : unsigned_values<std::uint16_t>) {
        teg::buffer b;
        teg::serialize(b, i0).or_throw();
        
        std::uint16_t i1;
        teg::deserialize(b, i1).or_throw();
        ASSERT_EQ(i0, i1);
    }
    // uint32_t    
    for (auto i0 : unsigned_values<std::uint32_t>) {
        teg::buffer b;
        teg::serialize(b, i0).or_throw();
        
        std::uint32_t i1;
        teg::deserialize(b, i1).or_throw();
        ASSERT_EQ(i0, i1);
    }
    // uint64_t
    for (auto i0 : unsigned_values<std::uint64_t>) {
        teg::buffer b;
        teg::serialize(b, i0).or_throw();
        
        std::uint64_t i1;
        teg::deserialize(b, i1).or_throw();
        ASSERT_EQ(i0, i1);
    }   
}
