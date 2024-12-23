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

TEST_CASE("A varint should include definitions for all integer operations.") {
    teg::varint<teg::u64> v; // Uninitialized.
    
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

static constexpr std::array<teg::i64, 9> i64_values = {
    -(128ll - 1ll), // - (2^7 - 1)
    -(16'384ll - 1ll), // - (2^14 - 1)
    -(2'097'152ll - 1ll), // - (2^21 - 1)
    -(268'435'456ll - 1ll), // - (2^28 - 1)
    -(34'359'738'368ll - 1ll), // - (2^35 - 1)
    -(4'398'046'511'104ll - 1ll), // - (2^42 - 1)
    -(562'949'953'421'312ll - 1ll), // - (2^49 - 1)
    -(72'057'594'037'927'936ll - 1ll), // - (2^56 - 1)
    -(9'223'372'036'854'775'807ll), // - (2^63 - 1)
};

TEST_CASE("ZigZag encoding") {
    SECTION("Test") {
        teg::i8 v = 127;

        teg::u8 e = teg::zigzag::encode(v);
        teg::i8 d = teg::zigzag::decode(e);

        ASSERT_EQ(e, 254);
        ASSERT_EQ(v, d);
    }

    SECTION("Odd numbers") {
        for (teg::i64 const v : i64_values) {
            teg::u64 const e = teg::zigzag::encode(v);
            teg::i64 const d = teg::zigzag::decode(e);
            
            ASSERT_EQ(v, d);
        }
    }
    SECTION("Even numbers") {
        for (teg::i64 const v : i64_values) {
            teg::u64 const e = teg::zigzag::encode(v + 1);
            teg::i64 const d = teg::zigzag::decode(e);
            
            ASSERT_EQ(v + 1, d);
        }
    }
}

TEST_CASE("De/encode ULEB-128") {
    COMPTIME_ASSERT(teg::uleb128::encoded_size(std::numeric_limits<teg::u64>::max()) == 10);
    COMPTIME_ASSERT(teg::uleb128::encoded_size(std::numeric_limits<teg::u64>::min()) == 1);
    
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
    SECTION("Error case: not enough buffer space") {
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

TEST_CASE("De/serialize varints") {
    SECTION("Variant unsigned 64-bit") {
        COMPTIME_ASSERT(teg::concepts::user_defined_serialization<teg::vuint64>);
        COMPTIME_ASSERT(!(teg::concepts::trivially_serializable<teg::vuint64, teg::default_mode>));

        for (teg::u8 i = 0; i < u64_values.size(); ++i) {
            teg::byte_array b{};
            
            teg::vuint64 v0 = u64_values[i];
            teg::serialize(b, v0).or_throw();

            teg::vuint64 v1;
            teg::deserialize(b, v1).or_throw();
            
            ASSERT_EQ((teg::u64)v0, (teg::u64)v1);
        }
    }
    SECTION("Variant signed 64-bit") {
        COMPTIME_ASSERT(teg::concepts::user_defined_serialization<teg::vint64>);
        COMPTIME_ASSERT(!(teg::concepts::trivially_serializable<teg::vint64, teg::default_mode>));

        for (teg::u8 i = 0; i < u64_values.size(); ++i) {
            teg::byte_array b{};
            
            teg::vint64 v0 = u64_values[i];
            teg::serialize(b, v0).or_throw();

            teg::vint64 v1;
            teg::deserialize(b, v1).or_throw();
            
            ASSERT_EQ((teg::u64)v0, (teg::u64)v1);
        }
    }
}

TEST_CASE("Force varint de/encoding") {
   
    SECTION("Simple struct") {
        struct t1 {
            teg::u32 x;
            teg::u32 y;
        };

        constexpr auto mode0 = teg::default_mode | teg::options::force_varint;
        constexpr auto mode1 = teg::default_mode;

        teg::byte_array b0{};
        teg::byte_array b1{}; 

        t1 v0 { 1, 2 };
        teg::serialize<mode0>(b0, v0).or_throw();
        teg::serialize<mode1>(b1, v0).or_throw();

        ASSERT_LT(b0.size(), b1.size());

        t1 v1;
        teg::deserialize<mode0>(b0, v1).or_throw();

        ASSERT_EQ(v0.x, v1.x);
        ASSERT_EQ(v0.y, v1.y);

        teg::deserialize<mode1>(b1, v1).or_throw();
        ASSERT_EQ(v0.x, v1.x);
        ASSERT_EQ(v0.y, v1.y);
    }
    SECTION("Nested struct") {
        struct t1 {
            teg::u32 x;
            teg::u32 y;
        };

        struct t2 {
            t1 v0;
            t1 v1;
        };

        constexpr auto mode0 = teg::default_mode | teg::options::force_varint;
        constexpr auto mode1 = teg::default_mode;

        teg::byte_array b0{};
        teg::byte_array b1{};

        t2 v0 { { 1, 2 }, { 3, 4 } };
        teg::serialize<mode0>(b0, v0).or_throw();
        teg::serialize<mode1>(b1, v0).or_throw();

        ASSERT_LT(b0.size(), b1.size());

        t2 v1;
        teg::deserialize<mode0>(b0, v1).or_throw();

        ASSERT_EQ(v0.v0.x, v1.v0.x);
        ASSERT_EQ(v0.v0.y, v1.v0.y);
        ASSERT_EQ(v0.v1.x, v1.v1.x);
        ASSERT_EQ(v0.v1.y, v1.v1.y);

        teg::deserialize<mode1>(b1, v1).or_throw();
        ASSERT_EQ(v0.v0.x, v1.v0.x);
        ASSERT_EQ(v0.v0.y, v1.v0.y);
        ASSERT_EQ(v0.v1.x, v1.v1.x);
        ASSERT_EQ(v0.v1.y, v1.v1.y);
    }
    SECTION("Struct with containers") {
        struct l1 {
            std::vector<teg::u32> v0;
            std::vector<teg::u64> v1;
        };

        constexpr auto mode0 = teg::default_mode | teg::options::force_varint;
        constexpr auto mode1 = teg::default_mode;

        teg::byte_array b0{};
        teg::byte_array b1{};

        l1 v0 { { 213213, 1}, {3232133, 123123} };
        teg::serialize<mode0>(b0, v0).or_throw();
        teg::serialize<mode1>(b1, v0).or_throw();

        ASSERT_LT(b0.size(), b1.size());

        l1 v1;
        teg::deserialize<mode0>(b0, v1).or_throw();

        ASSERT_EQ(v0.v0.size(), v1.v0.size());
        ASSERT_EQ(v0.v1.size(), v1.v1.size());
        ASSERT_EQ(v0.v0[0], v1.v0[0]);
        ASSERT_EQ(v0.v0[1], v1.v0[1]);
        ASSERT_EQ(v0.v1[0], v1.v1[0]);
        ASSERT_EQ(v0.v1[1], v1.v1[1]);

        teg::deserialize<mode1>(b1, v1).or_throw();
        ASSERT_EQ(v0.v0.size(), v1.v0.size());
        ASSERT_EQ(v0.v1.size(), v1.v1.size());
        ASSERT_EQ(v0.v0[0], v1.v0[0]);
        ASSERT_EQ(v0.v0[1], v1.v0[1]);
        ASSERT_EQ(v0.v1[0], v1.v1[0]);
        ASSERT_EQ(v0.v1[1], v1.v1[1]);
    }
}
