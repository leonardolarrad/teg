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
#include <string>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Detect packed layout structures") {
    struct pad {
        teg::u64 a;
        teg::u32 b;
    };

    struct nopad {
        teg::u64 a;
        teg::u64 b;
    };

    COMPTIME_ASSERT(!teg::concepts::packed_layout<pad>);
    COMPTIME_ASSERT(teg::concepts::packed_layout<nopad>);

    struct packed {
        nopad a;
        nopad b;
        nopad c;
    };

    struct nopacked {
        nopad a;
        pad b;
    };

    COMPTIME_ASSERT(teg::concepts::packed_layout<packed>);
    COMPTIME_ASSERT(!teg::concepts::packed_layout<nopacked>);

    SECTION("Non trivially copyable or standard layour or aggregate") {
        struct aggregate_but_not_standard_layout {
            teg::u64 a;
            std::string b;
        };

        struct not_standard_layout {
        public:
            teg::u64 a;
        private:
            teg::u64 b;
        };

        COMPTIME_ASSERT(!teg::concepts::packed_layout<std::string>);
        COMPTIME_ASSERT(!teg::concepts::packed_layout<aggregate_but_not_standard_layout>);
        COMPTIME_ASSERT(!teg::concepts::packed_layout<not_standard_layout>);
    }
    SECTION("Language packed layouts") {
        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::u8>));
        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::u16>));
        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::u32>));
        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::u64>));
        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::i8>));
        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::i16>));
        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::i32>));
        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::i64>));

        COMPTIME_ASSERT((teg::concepts::packed_layout<teg::i64[100]>));
        COMPTIME_ASSERT((teg::concepts::packed_layout<packed[100]>));
        COMPTIME_ASSERT(!(teg::concepts::packed_layout<nopacked[100]>));
    }
    SECTION("STL packed layouts") {
        COMPTIME_ASSERT((teg::concepts::packed_layout<std::array<teg::u8, 10>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_layout<std::pair<teg::u8, teg::u8>>));
    }
}
