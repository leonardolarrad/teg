#include "teg/teg.h"
#include <iostream>
#include <map>
#include <cstdio>

using namespace std;
using namespace teg;


int main() {
    struct user_data {
        u64 id;
        string username;
        string email;
        bool is_active;
        vector<string> roles;
        map<string, variant<string, i32>> preferences;
    };

    auto const user = user_data{
        .id = 9283700, .username = "brandon", .email = "brandom@asoiaf.com",
        .is_active = true, .roles = {"admin", "user"},
        .preferences = { {"theme", "dark"}, {"language", "en-US"},},
    };

    teg::byte_array archive{};
    teg::serialize<teg::compact_mode>(archive, user).or_throw();

    // print byte by byte

	for (const auto& byte : archive) {
		cout << std::hex << static_cast<int>(byte) << " ";
	}

    return 0;
}
