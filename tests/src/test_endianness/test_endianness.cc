#include <array>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <bit>
#include <array>
#include <bitset>
#include <optional>
#include <variant>
#include <concepts>
#include <string>
#include <tuple>
#include <forward_list>

#include "teg/teg.h"
#include "test/test.h"

#include "zppbits/zpp_bits.h"

TEST_CASE("Check fundamental encoding with big-endian and little-endian") {
    SECTION("8-bit integer") {
        teg::byte_buffer buffer_b{};
        teg::byte_buffer buffer_l{};
        constexpr auto big_endian = teg::options::big_endian;
        constexpr auto little_endian = teg::options::little_endian;

        int8_t i0 = 123;
        teg::serialize<big_endian>(buffer_b, i0).or_throw();
        teg::serialize<little_endian>(buffer_l, i0).or_throw();

        ASSERT(std::equal(std::begin(buffer_b), std::end(buffer_b), std::rbegin(buffer_l)));

        int8_t i1;
        int8_t i2;
        teg::deserialize<big_endian>(buffer_b, i1).or_throw();
        teg::deserialize<little_endian>(buffer_l, i2).or_throw();

        ASSERT_EQ(i0, i1);
        ASSERT_EQ(i0, i2);
        ASSERT_EQ(i1, i2);
    }
    SECTION("32-bit integer") {
        teg::byte_buffer buffer_b{};
        teg::byte_buffer buffer_l{};
        constexpr auto big_endian = teg::options::big_endian;
        constexpr auto little_endian = teg::options::little_endian;

        int32_t i0 = 1234567891;
        teg::serialize<big_endian>(buffer_b, i0).or_throw();
        teg::serialize<little_endian>(buffer_l, i0).or_throw();

        ASSERT(std::equal(std::begin(buffer_b), std::end(buffer_b), std::rbegin(buffer_l)));

        int32_t i1;
        int32_t i2;
        teg::deserialize<big_endian>(buffer_b, i1).or_throw();
        teg::deserialize<little_endian>(buffer_l, i2).or_throw();

        ASSERT_EQ(i0, i1);
        ASSERT_EQ(i0, i2);
        ASSERT_EQ(i1, i2);
    }
    SECTION("64-bit integer") {
        teg::byte_buffer buffer_b{};
        teg::byte_buffer buffer_l{};
        constexpr auto big_endian = teg::options::big_endian;
        constexpr auto little_endian = teg::options::little_endian;

        COMPTIME_ASSERT((teg::concepts::endian_swapping_required<int64_t, big_endian>));
        COMPTIME_ASSERT(!(teg::concepts::endian_swapping_required<int64_t, little_endian>));

        int64_t i0 = 1234567891;
        teg::serialize<big_endian>(buffer_b, i0).or_throw();
        teg::serialize<little_endian>(buffer_l, i0).or_throw();

        ASSERT(std::equal(std::begin(buffer_b), std::end(buffer_b), std::rbegin(buffer_l)));

        int64_t i1;
        int64_t i2;
        teg::deserialize<big_endian>(buffer_b, i1).or_throw();
        teg::deserialize<little_endian>(buffer_l, i2).or_throw();

        ASSERT_EQ(i0, i1);
        ASSERT_EQ(i0, i2);
        ASSERT_EQ(i1, i2);
    }
}

TEST_CASE("A 1-byte size element fixed contiguous container") {
    SECTION("De/serialize a c-str") {
        teg::byte_buffer buffer_b{};
        teg::byte_buffer buffer_l{};
        constexpr auto big_endian = teg::options::big_endian;
        constexpr auto little_endian = teg::options::little_endian;

        //COMPTIME_ASSERT(!(teg::concepts::endian_swapping_required<const char[13], big_endian>));
        //COMPTIME_ASSERT(!(teg::concepts::endian_swapping_required<const char[13], little_endian>));

        const char s0[] = "Hello World!";
        teg::serialize<big_endian>(buffer_b, s0).or_throw();
        teg::serialize<little_endian>(buffer_l, s0).or_throw();

        ASSERT(buffer_b == buffer_l);

        char s1[13];
        char s2[13];
        teg::deserialize<big_endian>(buffer_b, s1).or_throw();
        teg::deserialize<little_endian>(buffer_l, s2).or_throw();

        ASSERT_EQ(std::string(s0), std::string(s1));
        ASSERT_EQ(std::string(s0), std::string(s2));
        ASSERT_EQ(std::string(s1), std::string(s2));
    }
    SECTION("Fixed array") {
        teg::byte_buffer buffer_b{};
        teg::byte_buffer buffer_l{};
        constexpr auto big_endian = teg::options::big_endian;
        constexpr auto little_endian = teg::options::little_endian;

        COMPTIME_ASSERT(!(teg::concepts::endian_swapping_required<std::array<int8_t, 8>, big_endian>));
        COMPTIME_ASSERT(!(teg::concepts::endian_swapping_required<std::array<int8_t, 8>, little_endian>));

        std::array<int8_t, 8> a0 = { 1, 2, 3, 4, 5, 6, 7, 8 };
        teg::serialize<big_endian>(buffer_b, a0).or_throw();
        teg::serialize<little_endian>(buffer_l, a0).or_throw();

        ASSERT(buffer_b == buffer_l);

        std::array<int8_t, 8> a1;
        std::array<int8_t, 8> a2;
        teg::deserialize<big_endian>(buffer_b, a1).or_throw();
        teg::deserialize<little_endian>(buffer_l, a2).or_throw();

        ASSERT(a0 == a1);
        ASSERT(a0 == a2);
        ASSERT(a1 == a2);
    }
    SECTION("Fixed string") {
        teg::byte_buffer buffer_b{};
        teg::byte_buffer buffer_l{};
        constexpr auto big_endian = teg::options::big_endian;
        constexpr auto little_endian = teg::options::little_endian;

        COMPTIME_ASSERT(!(teg::concepts::endian_swapping_required<teg::fixed_string<12>, big_endian>));
        COMPTIME_ASSERT(!(teg::concepts::endian_swapping_required<teg::fixed_string<12>, little_endian>));
        
        constexpr auto s0 = teg::make_fixed_string("Hello World!");
        teg::serialize<big_endian>(buffer_b, s0).or_throw();
        teg::serialize<little_endian>(buffer_l, s0).or_throw();

        ASSERT(buffer_b == buffer_l);

        teg::fixed_string<12> s1;
        teg::fixed_string<12> s2;
        teg::deserialize<big_endian>(buffer_b, s1).or_throw();
        teg::deserialize<little_endian>(buffer_l, s2).or_throw();

        ASSERT(s0 == s1);
        ASSERT(s0 == s2);
        ASSERT(s1 == s2);
    }    
}