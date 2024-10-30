#include <any>
#include <array>
#include <bitset>
#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include <limits>

#include "teg/teg.h"
#include "test/test.h"


TEST_CASE("`teg::uleb128` should include definitions for all uint64_t operations.") {
    teg::uleb128 v; // uninitialized
    
    v = 777ull;
    ASSERT(v == 777ull);

    v += 1ull;
    ASSERT(v == 778ull);

    v -= 1ull;
    ASSERT(v == 777ull);

    v *= 2ull;
    ASSERT(v == 1554ull);

    v /= 2ull;
    ASSERT(v == 777ull);

    v %= 2ull;
    ASSERT(v == 1ull);

    v <<= 1ull;
    ASSERT(v == 2ull);

    v >>= 1ull;
    ASSERT(v == 1ull);

    v &= 1ull;
    ASSERT(v == 1ull);

    v |= 1ull;
    ASSERT(v == 1ull);

    v ^= 1ull;
    ASSERT(v == 0ull);
}

TEST_CASE("Debug") {
    teg::u8 data[10] = {};

    teg::uleb128::encode(data, 777ull);
    
    for (auto i = 0; i < 10; ++i) {
        std::cout << std::bitset<8>(data[i]) << std::endl;
    }
}

static constexpr std::array<teg::u64, 9> u64_values = {
    128ull - 1ull, // 2^7 - 1
    16'384ull - 1ull, // 2^14 - 1
    2'097'152ull - 1ull, // 2^21 - 1
    268'435'456ull - 1ull, // 2^28 - 1
    34'359'738'368ull - 1ull, // 2^35 - 1
    4'398'046'511'104ull - 1ull, // 2^42 - 1
    562'949'953'421'312ull - 1ull, // 2^49 - 1
    72'057'594'037'927'936ull - 1ull, // 2^56 - 1
    9'223'372'036'854'775'808ull - 1ull, // 2^63 - 1    
};

TEST_CASE("Encode and decode unsigned LEB128 varint") {
    COMPTIME_ASSERT(teg::uleb128::size(std::numeric_limits<teg::u64>::max()) == 10);
    COMPTIME_ASSERT(teg::uleb128::size(std::numeric_limits<teg::u64>::min()) == 1);
    
    SECTION("N-byte buffer") {
        teg::u8 data[10] = {};

        for (teg::u8 i = 0; i < u64_values.size(); ++i) {
            teg::u64 v0 = u64_values[i];
            auto const s0 = teg::uleb128::encode(data, v0);
            
            teg::u64 v1;
            auto const s1 = teg::uleb128::decode(data, v1);
            
            ASSERT_EQ(s0, i+1);
            ASSERT_EQ(s0, s1);
            ASSERT_EQ(v0, v1);
        }
    }
    SECTION("Error case") {
        teg::u8 data[1] = {};

        for (teg::u8 i = 1; i < u64_values.size(); ++i) {
            teg::u64 v0 = u64_values[i] + 1;
            auto const s0 = teg::uleb128::encode(data, v0);

            teg::u64 v1;
            auto const s1 = teg::uleb128::decode(data, v1);

            ASSERT_EQ(s0, 0);
            ASSERT_EQ(s0, s1);
            ASSERT_NE(v0, v1);
        }
    }
}
