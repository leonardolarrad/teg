#include <array>
#include <bitset>
#include <cstdint>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Reset buffer") {
    teg::byte_buffer buffer{};
    teg::binary_serializer serializer{buffer};

    std::string str0 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    for (auto i = 0; i < 100000; ++i) {
        serializer.reset();
        serializer.serialize(str0).or_throw();
    }

    ASSERT(buffer.size() == serializer.encoding_size(str0));
}