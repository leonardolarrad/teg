#include <cstdint>
#include <tuple>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Compile-time test MD5 hashes") {
    SECTION("Test 1") {
        constexpr auto msg = teg::make_fixed_string("I don't like sand");
        
        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg);
        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        constexpr teg::md5_digest hash_128 = teg::md5_hash_u128(msg);
        constexpr teg::fixed_string<32> hash = teg::md5_hash(msg);

        COMPTIME_ASSERT_EQ(hash_32, 0x01252fe0ul);
        COMPTIME_ASSERT_EQ(hash_64, 0x01252fe0a5e1ca37ull);
        COMPTIME_ASSERT_EQ(hash_128, std::make_tuple(0x01252fe0, 0xa5e1ca37, 0x430b22ec, 0xba121134));
        COMPTIME_ASSERT_EQ(hash, "01252fe0a5e1ca37430b22ecba121134");
    }
    SECTION("Test 2") {
        constexpr auto msg = teg::make_fixed_string("Only a Sith deals in absolutes");

        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg);
        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        constexpr teg::md5_digest hash_128 = teg::md5_hash_u128(msg);
        constexpr teg::fixed_string<32> hash = teg::md5_hash(msg);

        COMPTIME_ASSERT_EQ(hash_32, 0x624c89bbul);
        COMPTIME_ASSERT_EQ(hash_64, 0x624c89bb6f2573d7ull);
        COMPTIME_ASSERT_EQ(hash_128, std::make_tuple(0x624c89bb, 0x6f2573d7, 0x624e1420, 0x9de8f492));
        COMPTIME_ASSERT_EQ(hash, "624c89bb6f2573d7624e14209de8f492");
    }
    SECTION ("Test 3") {
        constexpr auto msg = teg::make_fixed_string("This is outrageous! It's unfair!");

        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg);
        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        constexpr teg::md5_digest hash_128 = teg::md5_hash_u128(msg);
        constexpr teg::fixed_string<32> hash = teg::md5_hash(msg);

        COMPTIME_ASSERT_EQ(hash_32, 0xc9e04138ul);
        COMPTIME_ASSERT_EQ(hash_64, 0xc9e04138e5f3abb6ull);
        COMPTIME_ASSERT_EQ(hash_128, std::make_tuple(0xc9e04138, 0xe5f3abb6, 0x18a7a124, 0xc9a92ab2));
        COMPTIME_ASSERT_EQ(hash, "c9e04138e5f3abb618a7a124c9a92ab2");
    }
    SECTION ("Test 4") {
        constexpr auto msg = teg::make_fixed_string("It's over Anakin, I have the high ground");

        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg); 
        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        constexpr teg::md5_digest hash_128 = teg::md5_hash_u128(msg);
        constexpr teg::fixed_string<32> hash = teg::md5_hash(msg);

        COMPTIME_ASSERT_EQ(hash_32, 0xd3f8915eul);
        COMPTIME_ASSERT_EQ(hash_64, 0xd3f8915ed0982084ull);
        COMPTIME_ASSERT_EQ(hash_128, std::make_tuple(0xd3f8915e, 0xd0982084, 0x1add8cdb, 0x39387700));
        COMPTIME_ASSERT_EQ(hash, "d3f8915ed09820841add8cdb39387700");
    }
}