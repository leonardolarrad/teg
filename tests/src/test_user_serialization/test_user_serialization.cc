#include <array>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "teg/teg.h"
#include "test/test.h"

class vec3 {
public:
    constexpr vec3() : x(0), y(0), z(0) {}
    constexpr vec3(teg::u32 x, teg::u32 y, teg::u32 z) : x(x), y(y), z(z) {}

    constexpr teg::u32 get_x() const { return x; }
    constexpr teg::u32 get_y() const { return y; }
    constexpr teg::u32 get_z() const { return z; }

    constexpr void set_x(teg::u32 x) { this->x = x; }
    constexpr void set_y(teg::u32 y) { this->y = y; }
    constexpr void set_z(teg::u32 z) { this->z = z; }

    constexpr bool operator==(vec3 const&) const = default;

private:
    teg::u32 x, y, z;
};

template <class F>
TEG_NODISCARD TEG_INLINE constexpr auto usr_serialized_size(F&& serialized_size, vec3 const& vec) -> teg::u64 {
    return serialized_size(vec.get_x(), vec.get_y(), vec.get_z());
}

template <class F>
TEG_NODISCARD TEG_INLINE constexpr auto usr_serialize(F&& encode, vec3 const& vec) -> teg::error {
    return encode(vec.get_x(), vec.get_y(), vec.get_z());
}

template <class F>
TEG_NODISCARD TEG_INLINE constexpr auto usr_deserialize(F&& decode, vec3 & vec) -> teg::error {
    std::tuple<teg::u32, teg::u32, teg::u32> values;
    
    if (auto const result = decode(values); teg::failure(result)) [[unlikely]] {
        return result;
    }

    vec.set_x(std::get<0>(values));
    vec.set_y(std::get<1>(values));
    vec.set_z(std::get<2>(values));
    return {};
}

TEST_CASE("Check concepts") {
    constexpr auto c0 = teg::concepts::user_defined_serialization<vec3>;

    COMPTIME_ASSERT(c0);
}

TEST_CASE("User-defined serialization") {
    teg::byte_array buffer{};

    vec3 v0{1, 2, 3};

    teg::serialize(buffer, v0).or_throw();

    for (auto b : buffer) {
        std::cout << static_cast<int>(b) << " ";
    }
    std::cout << std::endl;

    vec3 v1;

    teg::deserialize(buffer, v1).or_throw();

    ASSERT(v0 == v1);
}