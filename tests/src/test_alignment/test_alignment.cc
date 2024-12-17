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

struct alignment_info {
    std::size_t size;
    std::size_t alignment;
    bool has_padding_bits;

    constexpr bool operator==(alignment_info const&) const = default;
};

template <class T>
constexpr auto info() -> alignment_info {
    return alignment_info {
        .size = sizeof(T),
        .alignment = alignof(T),
        .has_padding_bits = teg::has_padding_bits<T>(),
    };
}

TEST_CASE("Alignas") {
    struct alignas(16) struct_alignas_16 {
        int32_t a;
        int32_t b;
    };

    constexpr auto cx0 = sizeof(struct_alignas_16);
    constexpr auto cx1 = teg::packed_sizeof<struct_alignas_16>();
    
    std::cout << "cx0 = " << cx0 << ", cx1 = " << cx1 << std::endl;
}

TEST_CASE("Use reflection to detect whether a type has padding") {
    SECTION("Test 1: A struct expected to have padding") {
        struct unpacked_struct_0 {
            int8_t a;
            int32_t b;
        };

        #pragma pack(push, 1)
        struct packed_struct_0 {
            int8_t a;
            int32_t b;
        };
        #pragma pack(pop)

        constexpr auto packed = info<unpacked_struct_0>();
        constexpr auto unpacked = info<packed_struct_0>();

        COMPTIME_ASSERT(packed.size != unpacked.size);
        COMPTIME_ASSERT(packed.alignment != unpacked.alignment);
        COMPTIME_ASSERT(packed.has_padding_bits);
        COMPTIME_ASSERT(!unpacked.has_padding_bits);
        
        COMPTIME_ASSERT(!teg::concepts::packed_standard_layout<unpacked_struct_0>);
        COMPTIME_ASSERT(teg::concepts::packed_standard_layout<packed_struct_0>);
    }
    SECTION("Test 2: A struct expected to be always packed") {  
        struct unpacked_struct_1 {
            int32_t a;
            int32_t b;
        };
        
        #pragma pack(push, 1)
        struct packed_struct_1 {
            int32_t a;
            int32_t b;
        };
        #pragma pack(pop)

        constexpr auto packed = info<unpacked_struct_1>();
        constexpr auto unpacked = info<packed_struct_1>();

        COMPTIME_ASSERT(packed.size == unpacked.size);
        COMPTIME_ASSERT(packed.alignment != unpacked.alignment); // They still have different alignments.
        COMPTIME_ASSERT(!packed.has_padding_bits);
        COMPTIME_ASSERT(!unpacked.has_padding_bits);
        
        // And both of them are packed layout types.
        COMPTIME_ASSERT(teg::concepts::packed_standard_layout<unpacked_struct_1>);
        COMPTIME_ASSERT(teg::concepts::packed_standard_layout<packed_struct_1>);
    }
}

TEST_CASE("C-arrays are packed if their element type is packed") {
    struct unpacked_struct_2 {
        int8_t a;
        int32_t b;
    };

    #pragma pack(push, 1)
    struct packed_struct_2 {
        int8_t a;
        int32_t b;
    };
    #pragma pack(pop)

    SECTION("Bounded c-arrays") {
        constexpr auto packed = info<unpacked_struct_2[17]>();
        constexpr auto unpacked = info<packed_struct_2[17]>();

        COMPTIME_ASSERT(packed.size != unpacked.size);
        COMPTIME_ASSERT(packed.alignment != unpacked.alignment);
        COMPTIME_ASSERT(packed.has_padding_bits);
        COMPTIME_ASSERT(!unpacked.has_padding_bits);        
        
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<unpacked_struct_2[17]>));
        COMPTIME_ASSERT((teg::concepts::packed_standard_layout<packed_struct_2[17]>));
    }
    SECTION("Unbounded c-arrays") {
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<unpacked_struct_2[]>));
        COMPTIME_ASSERT((teg::concepts::packed_standard_layout<packed_struct_2[]>));
    }
}

TEST_CASE("Check wheter tuple-like types model `packed_standard_layout`") {
    SECTION("`std::tuple` type does not model `trivially_copyable` nor `standard_layout`") {
        COMPTIME_ASSERT(!(teg::concepts::trivially_copyable<std::tuple<int64_t, int64_t>>));
        COMPTIME_ASSERT(!(teg::concepts::standard_layout<std::tuple<int64_t, int64_t>>));
    }
    SECTION("Some `std::pair` types does model `trivially_copyable` and `standard_layout`") {
        COMPTIME_ASSERT(!(teg::concepts::trivially_copyable<std::pair<int64_t, int64_t>>));
        COMPTIME_ASSERT((teg::concepts::standard_layout<std::pair<int64_t, int64_t>>));

        COMPTIME_ASSERT((teg::concepts::trivially_copyable<std::pair<const int64_t, int64_t>>));
        COMPTIME_ASSERT((teg::concepts::standard_layout<std::pair<const int64_t, int64_t>>));
    }
    SECTION("`std::tuple` types do not model `packed_standard_layout`") {
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::tuple<char>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::tuple<int8_t, int8_t>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::tuple<int32_t, int64_t>>));
    }
    SECTION("Some `std::pair` types do model `packed_standard_layout`") {
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::pair<char, char>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::pair<int32_t, int32_t>>));

        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::pair<int8_t, int64_t>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::pair<int32_t, std::string>>));
    }
}

TEST_CASE("Check wheter optional types model `packed_standard_layout`") {    
    SECTION("Optional types model `trivially_copyable` and `standard_layout`") {
        COMPTIME_ASSERT(teg::concepts::trivially_copyable<std::optional<int64_t>>);
        COMPTIME_ASSERT(teg::concepts::standard_layout<std::optional<int64_t>>);
    }
    SECTION("Optional types (by themselves) do not model `packed_standard_layout`") {
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::optional<int8_t>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::optional<int16_t>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::optional<int32_t>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::optional<int64_t>>));
    }
    SECTION("Neither aggregates containing optionals cannot model `packed_standard_layout`") {
        struct aggregate_with_optionals {
            uint8_t a;
            uint8_t b;
            std::optional<int8_t> c;
        };
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<aggregate_with_optionals>));
    }
}

TEST_CASE("Check wheter variant types model `packed_standard_layout`") {
    SECTION("Variant types model `trivially_copyable` and `standard_layout`") {
        COMPTIME_ASSERT(teg::concepts::trivially_copyable<std::variant<int64_t>>);
        COMPTIME_ASSERT(teg::concepts::standard_layout<std::variant<int64_t>>);
    }    
    SECTION("Variant types (by themselves) does not model `packed_standard_layout`") {
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::variant<int8_t>>));
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<std::variant<const int8_t>>));
    }
    SECTION("Neither aggregates containing variants cannot model `packed_standard_layout`") {
        struct aggregate_with_variants {
            uint8_t a;
            int16_t b;
            std::variant<int8_t, int16_t> c;
        };
        COMPTIME_ASSERT(!(teg::concepts::packed_standard_layout<aggregate_with_variants>));
    }    
}

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