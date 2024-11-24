#include <array>
#include <bitset>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

struct aggregate0 {
    // C-arrays.
    float ca0[10];
    double ca1[10];

    // Fixed size containers.
    std::array<std::uint8_t, 10> a0;
    teg::fixed_string<10> fs0;

    // Contiguous containers.
    std::vector<std::uint16_t> v0;
    std::deque<std::uint32_t> dq0;

    // Associative containers.
    std::set<std::uint64_t> s0;
    std::map<std::int8_t, std::string> m0;

    // Containers.
    std::forward_list<std::int16_t> fl0;
    std::list<std::int32_t> l0;

    // Variants and tuples.
    std::variant<std::int64_t, std::string> v1;
    std::tuple<std::string, std::string, std::string> t0;

    // Optional.
    std::optional<std::string> opt0;

    // Owning pointers.
    std::shared_ptr<std::uint64_t> sp0;
    std::optional<std::unique_ptr<std::uint64_t>> opt1;

    bool operator==(const aggregate0& other) const {
        return std::equal(std::begin(ca0), std::end(ca0), std::begin(other.ca0))
            && std::equal(std::begin(ca1), std::end(ca1), std::begin(other.ca1))
            && std::equal(std::begin(a0), std::end(a0), std::begin(other.a0))
            && fs0 == other.fs0 && v0 == other.v0 && dq0 == other.dq0 && s0 == other.s0
            && m0 == other.m0 && fl0 == other.fl0 && l0 == other.l0 && v1 == other.v1
            && t0 == other.t0 && opt0 == other.opt0 && *sp0 == *other.sp0
            && ((opt1.has_value() && other.opt1.has_value()) 
                ? *(opt1.value()) == *(other.opt1.value())
                : !opt1.has_value() && !other.opt1.has_value());
    }
};

auto make_aggregate() -> aggregate0 {
    return aggregate0 {
        .ca0 = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f },
        .ca1 = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 },
        .a0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        .fs0 = "0123456789",
        .v0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        .dq0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        .s0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        .m0 = { { 1, "a" }, { 2, "b" }, { 3, "c" }, { 4, "d" }, { 5, "e" } },
        .fl0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        .l0 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        .v1 = 100,
        .t0 = { "a", "b", "c" },
        .opt0 = "hello",
        .sp0 = std::make_shared<std::uint64_t>(99),
        .opt1 = std::make_unique<std::uint64_t>(999)
    };
}


TEST_CASE("De/serialize aggregate0") {
    teg::byte_array b;
    
    aggregate0 agg0 = make_aggregate();
    teg::serialize(b, agg0).or_throw();
    
    aggregate0 agg1;
    teg::deserialize(b, agg1).or_throw();
    
    ASSERT(agg0 == agg1);
}