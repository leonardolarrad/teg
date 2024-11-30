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
#include <ostream>
#include <istream>
#include <fstream>
#include <system_error>

#include "teg/teg.h"
#include "test/test.h"

namespace tteg {

enum class error_code {
    error = 1
};

class serialization_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class result {
public:
    constexpr result(error_code code = {}) noexcept
        : m_code(code) {}
    
    constexpr error_code code() const noexcept
    {
        return m_code;
    }

    constexpr void or_throw() const
    {
        if (m_code != error_code{}) [[unlikely]] {
            throw serialization_error("Serialization error");
        }
    }

private:
    error_code m_code;
};

constexpr auto success(result r) -> bool {
    return r.code() == error_code{};
}

constexpr auto failure(result r) -> bool {
    return r.code() != error_code{};
}

class stream_writer {
public:
    constexpr stream_writer(std::ostream& stream) noexcept
        : m_stream(stream) {}
    
    auto write_bytes(std::span<std::byte const> bytes) -> result {
        m_stream.get().write(reinterpret_cast<char const*>(bytes.data()), bytes.size());

        if (m_stream.get().fail()) {
            return error_code::error;
        }

        return {};
    }

private:
    std::reference_wrapper<std::ostream> m_stream;
};

template <class B>
class buffer_writer {
public:
    constexpr buffer_writer() = delete;
    constexpr buffer_writer(B& buffer) noexcept
        : m_buffer(buffer), m_position(0) {}

    constexpr auto write_bytes(std::span<std::byte const> const bytes) -> result {
        if (bytes.size() > m_buffer.get().size() - m_position) {
            m_buffer.get().resize(m_position + bytes.size());
        }

        if (std::is_constant_evaluated()) {
            for (decltype(bytes.size()) i = 0; i < bytes.size(); ++i) {
                m_buffer.get()[m_position + i] = bytes[i];
            }
            m_position += bytes.size();
            return {};
        }
        else {
            std::memcpy(
                m_buffer.get().data() + m_position,
                bytes.data(), 
                bytes.size());
            m_position += bytes.size();
            return {};
        }
    }

public:
    std::reference_wrapper<B> m_buffer;
    uint64_t m_position;
};

template <class W>
class encoder {
public:
    constexpr encoder() = delete;
    constexpr encoder(W& writer) noexcept
        : m_writer(writer) {}
    constexpr encoder(W&& writer) noexcept
        : m_writer(std::move(writer)) {}

    template <class... T>
    constexpr auto encode(T const&... objs) -> result {
        return encode_many(objs...);
    }

private:
    template <class T0, class... TN>
    constexpr auto encode_many(T0 const& first_obj, TN const&... remaining_objs) -> result {        
        if (auto const result = encode_one(first_obj); failure(result)) [[unlikely]] {
            return result;
        }

        return encode_many(remaining_objs...);
    }

    constexpr auto encode_many() -> result {
        return {};
    }

    template <class T> requires (std::integral<T>)
    constexpr auto encode_one(T const& integer) -> result {
        if (std::is_constant_evaluated()) {
            m_writer.write_bytes(
                std::bit_cast<std::array<std::byte, sizeof(T)>>(integer));
            return {};
        }
        else {
            m_writer.write_bytes(
                std::span<std::byte const>{
                    reinterpret_cast<std::byte const*>(&integer), sizeof(T)});
            return {};
        }
    }

    W m_writer;
};

} // namespace tteg


TEST_CASE("Stream") {
    std::ofstream out("test.bin", std::ios::binary | std::ios::trunc);
    tteg::stream_writer writer {out};
    tteg::encoder encoder { writer };
    encoder.encode(1, 2, 3);
    encoder.encode(1, 2, 3);
}

TEST_CASE("Buffer") {
    std::vector<std::byte> buffer{};
    tteg::buffer_writer writer {buffer};
    tteg::encoder encoder { writer };
    encoder.encode(1, 2, 3);
    encoder.encode(4, 5, 6);

    for (auto b : buffer) {
        std::cout << static_cast<int>(b) << " ";
    }
    std::cout << std::endl;
}

constexpr auto cx_encode() {
    std::vector<std::byte> buffer{};
    tteg::buffer_writer writer {buffer};
    tteg::encoder encoder { writer };
    encoder.encode(1, 2, 3);
    encoder.encode(4, 5, 6);

    return buffer[0];
}

TEST_CASE("Comptime buffer") {
    constexpr auto c0 = cx_encode();
    ASSERT(c0 == std::byte{1});
}


TEST_CASE("File stream: Fundamentals") {
    std::ofstream out("fundamentals.bin", std::ios::binary | std::ios::trunc);
    teg::serialize(out, 11, 5, 99).or_throw();
}
