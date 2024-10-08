#include <cstdint>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Compile-time test MD5 hashes") {
    SECTION("Test 1") {
        constexpr auto msg = teg::make_fixed_string("I don't like sand");
        
        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg);
        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        constexpr teg::fixed_string<32> hash = teg::md5_hash(msg);

        COMPTIME_ASSERT_EQ(hash_32, 0x01252fe0ul);
        COMPTIME_ASSERT_EQ(hash_64, 0x01252fe0a5e1ca37ull);
        COMPTIME_ASSERT_EQ(hash, "01252fe0a5e1ca37430b22ecba121134");
    }
    SECTION("Test 2") {
        constexpr auto msg = teg::make_fixed_string("Only a Sith deals in absolutes");

        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg);
        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        constexpr teg::fixed_string<32> hash = teg::md5_hash(msg);

        COMPTIME_ASSERT_EQ(hash_32, 0x624c89bbul);
        COMPTIME_ASSERT_EQ(hash_64, 0x624c89bb6f2573d7ull);
        COMPTIME_ASSERT_EQ(hash, "624c89bb6f2573d7624e14209de8f492");
    }
    SECTION ("Test 3") {
        constexpr auto msg = teg::make_fixed_string("It's over Anakin, I have the high ground");

        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg); 
        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        constexpr teg::fixed_string<32> hash = teg::md5_hash(msg);

        COMPTIME_ASSERT_EQ(hash_32, 0xd3f8915eul);
        COMPTIME_ASSERT_EQ(hash_64, 0xd3f8915ed0982084ull);
        COMPTIME_ASSERT_EQ(hash, "d3f8915ed09820841add8cdb39387700");
    }
}