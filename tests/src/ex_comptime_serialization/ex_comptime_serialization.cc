#include "teg/teg.h"
#include <iostream>

class User {
public:
    uint64_t id;
    std::string username;
    std::string email;
    uint64_t created_at;
    uint64_t modified_at;
    std::optional<std::array<uint8_t, 6>> sms_code;
};

template <class T>
void print(T const& t) {
    std::cout << t;
}

template <class T>
void hex_print(T const& t) {
    std::cout << std::hex << (int)t << " ";
}

struct image_data_v1 {
    std::string id;
    std::string content_base64;
};

struct image_data_v2 {
    std::string id;
    std::string content_base64;
    teg::compatible<std::string, 2> alt_text;
};

struct NonPacked {
    uint32_t a;
    uint8_t b;
};

    #pragma pack(push, 1)
    struct Packed {
        uint32_t a;
        uint8_t b;
    };
    #pragma pack(pop)


int main() {
    constexpr bool is_packed = teg::concepts::packed_layout<Packed>;

    try {
        teg::fixed_byte_array<1> buffer; // A buffer with just 1 byte of space
        auto result = teg::serialize(buffer, "Just an ordinary string");
        print(result.code);
    }
    catch (std::exception e) {
        print(e.what()); // system_error 
    }


    teg::fixed_string<34> const schema = teg::schema<1, User>();
    
    for (auto x : schema) {
        print(x);
    }
    
    return 0;
}