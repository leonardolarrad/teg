#include "teg/teg.h"

class User {
public:
    uint64_t id;
    std::string username;
    std::string email;
    uint64_t created_at;
    uint64_t modified_at;
};

#if false 

constexpr teg::fixed_byte_array<17> serialize_number(teg::i32 number) {
    teg::fixed_byte_array<17> buffer{};
    teg::serialize(buffer, number).or_throw();
    
    return buffer;
}

int main() {
    auto buffer = serialize_number(99);   

    return 0;
}

#endif


#if false 

int main() {
    auto user = User{ 123, "Juan Carlos", "juanc99@gmail.com" };
    const std::string& user_email = teg::get_member<2>(user);

    return 0;
}

#endif

#if false

#include <iostream>

template <class T>
void print(T const& t) {
    std::cout << t << " ";
}


int main() {
    auto user = User { 1,  "Juan Carlos", "jnc99@gmail.com", 1738714000, 1738742800 };

teg::visit_members(
    [](auto const&... members) {
        (print(members), ...);
    },
    user
);

    return 0;
}

#endif

#if false

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */



class Vector3 {
public:
    float x, y, z;
};

int main() {
    srand(time(NULL));
    auto v1 = Vector3{ float(rand() % 10 + 1), float(rand() % 10 + 1), float(rand() % 10 + 1) };
    auto v2 = Vector3{ float(rand() % 10 + 1), float(rand() % 10 + 1), float(rand() % 10 + 1) };

bool t = teg::memberwise_equal(v1, v2);

    return t;
}

#endif

#if false

#include <tuple>
#include <string>
#include <iostream>

template <class T>
void print(T const& t) {
    std::cout << t << " ";
}

template <class T, class U>
void print(T const& t, U const& u) {
    std::cout << t << " " << u;
}

uint64_t time() {
    return 1738749123;
}

int main() {
auto user = User{ 1,  "Juan Carlos", "jnc99@gmail.com", 1738714000, 1738742800 };

teg::get_member<2>(user) = "juanc99@gmail.com";
teg::get_member<4>(user) = time();

print(user.email, user.modified_at);

    return 0;
}

#endif 

#if true 

int main() {
int64_t n = -200;

uint64_t z = teg::zigzag::encode(n);
       

std::vector<uint8_t> buffer;
buffer.resize(teg::uleb128::encoded_size(z));

teg::uleb128::encode(buffer, z);

    

    return z;
}

#endif 