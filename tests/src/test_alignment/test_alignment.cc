#include <array>
#include <cstring>
#include <cstdint>
#include <iostream>

#include "teg/teg.h"
#include "test/test.h"

#include "zppbits/zpp_bits.h"

#pragma pack(push, 1)
struct struct_a {
    int b;
    char a;
};
#pragma pack(pop)

template <class T>
constexpr auto info() -> void {
    std::cout << sizeof(T) << std::endl;
    std::cout << alignof(T) << std::endl;
}

TEST_CASE("Get knowledge about alignment") {

    SECTION("Struct A") {
        info<struct_a>();
        auto [data, in, out] = zpp::bits::data_in_out();
        
        auto s0 = struct_a{ 1, 'a' };
        out(s0).or_throw();

        for (auto b : data) {
            std::cout << (int)b << " ";
        }
        std::cout << std::endl;

        teg::byte_buffer buffer;
        teg::serialize(buffer, s0).or_throw();
        
        for (auto b : buffer) {
            std::cout << (int)b << " ";
        }
        std::cout << std::endl;
    }

}

  