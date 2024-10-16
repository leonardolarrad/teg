#include <vector>
#include <array>
#include <concepts>
#include <type_traits>
#include <cstddef>
#include <bit>
#include <iostream>
#include <span>
#include <cstring>

template <class T>
concept resizable = requires(T t) {
    t.resize(1);
};

template <class B>
class serializer {
public:
    constexpr serializer() = default;
    constexpr serializer(B & buffer) : m_buffer(buffer) {}
    constexpr serializer(B && buffer) : m_buffer(buffer) {}

    using span_type = decltype(std::span{std::declval<B&>()});
    using buffer_type = std::conditional_t<resizable<B>, B&, span_type>;

    template <class T>
    constexpr inline int serialize(T const& obj) {
        if constexpr (resizable<B>) {
            m_buffer.resize(sizeof(obj));
        }

        using type = std::remove_cvref_t<decltype(obj)>;

        if constexpr (std::is_fundamental_v<type>) {
            if (std::is_constant_evaluated()) {
                auto byte_arr = std::bit_cast<std::array<std::byte, sizeof(obj)>>(obj);
                std::copy(byte_arr.begin(), byte_arr.end(), m_buffer.begin());
                
                return (int)m_buffer[0];
            }
            else {
                std::memcpy(
                    m_buffer.data(), &obj, sizeof(obj)
                );
                return 4;
            }
        }   

        return 2;
    }

    void wtf() {
        m_buffer.push_back(99);
    }

private:
    buffer_type m_buffer;
};


template <class B>
constexpr serializer<B> make_serializer(B& buffer) {
    return serializer(buffer);
}

constexpr int ser() {
    std::vector<std::byte> v{};
    serializer s(v);
    
    return s.serialize(99);;
}

constexpr void test() {
    constexpr auto rx = ser();
}

int main() {
    constexpr auto rx = ser();
    return rx;
}
