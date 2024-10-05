#include <cstdint>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Compile-time check md5 hashes") {
    SECTION("Test 1") {
        constexpr auto msg = teg::make_fixed_string("message digest");
        
        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg);
        static_assert(hash_32 == 0xF96B697Dul);
        ASSERT_EQ(hash_32, 0xF96B697Dul);

        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        static_assert(hash_64 == 0xF96B697D7CB7938Dull);
        ASSERT_EQ(hash_64, 0xF96B697D7CB7938Dull);
    }
    SECTION("Test 2") {
        constexpr auto msg = teg::make_fixed_string("The quick brown fox jumps over the lazy dog");

        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg);
        static_assert(hash_32 == 0x9E107D9Dul);
        ASSERT_EQ(hash_32, 0x9E107D9Dul);

        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        static_assert(hash_64 == 0x9E107D9D372BB682ull);
        ASSERT_EQ(hash_64, 0x9E107D9D372BB682ull);
    }
    SECTION ("Test 3") {
        constexpr auto msg = teg::make_fixed_string("It's over Anakin, I have the high ground");

        constexpr std::uint32_t hash_32 = teg::md5_hash_u32(msg);
        static_assert(hash_32 == 0xD3F8915Eul);
        ASSERT_EQ(hash_32, 0xD3F8915Eul);

        constexpr std::uint64_t hash_64 = teg::md5_hash_u64(msg);
        static_assert(hash_64 == 0xD3F8915ED0982084ull);
        ASSERT_EQ(hash_64, 0xD3F8915ED0982084ull);
    }
}