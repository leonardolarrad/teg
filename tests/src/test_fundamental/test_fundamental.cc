#include <cstdint>
#include <concepts>
#include <array>
#include "larrad/larrad.h"
#include "test/test.h"

namespace teg = larrad;

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