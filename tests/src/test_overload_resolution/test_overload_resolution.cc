#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "teg/teg.h"
#include "test/test.h"

template <typename T> 
std::string overload(T&& t) requires teg::fixed_size_container<T> {
    return "fixed_size_container";
}

template <typename T> 
std::string overload(T&& t) requires teg::contiguous_container<T> {
    return "contiguous_container";
}

template <typename T> 
std::string overload(T&& t) requires teg::container<T> {
    return "container";
}

template <typename T>
std::string overload(T&& t) requires teg::optional<T> {
    return "optional";
}

template <typename T>
std::string overload(T&& t) requires teg::owning_pointer<T> {
    return "owning_pointer";
}

template <typename T>
std::string overload(T&& t) requires teg::tuple<T> && (!teg::container<T>) {
    return "tuple";
}

template <typename T>
std::string overload(T&& t) requires teg::variant<T> {
    return "variant";
}

template <typename T>
std::string overload(T&& t) {
    return "auto";
}

TEST_CASE("Overload resolution with concepts") {
    SECTION("Auto") {
        int i;
        ASSERT_EQ(overload(i), "auto");
    }
    SECTION("Optional") {
        std::optional<int> opt0;
        ASSERT_EQ(overload(opt0), "optional");
    }
    SECTION("Container") {
        std::forward_list<int> fl;
        ASSERT_EQ(overload(fl), "container");
    }
    SECTION("Fixed size container") {
        std::array<int, 10> a;
        ASSERT_EQ(overload(a), "fixed_size_container");
    }
    SECTION("CV-Qualified container") {
        using const_vector = std::vector<int> const;
        const_vector v;
        ASSERT_EQ(overload(v), "contiguous_container");

        std::string s0 = "hello";

        using const_string = std::string const;
        const_string s1 = s0;
        ASSERT_EQ(overload(s1), "contiguous_container");
    }
    SECTION("Owning pointer") {
        std::unique_ptr<int> up = std::make_unique<int>(0);
        ASSERT_EQ(overload(up), "owning_pointer");
    }
    SECTION("Tuple") {
        std::tuple<int, std::string> t0 = { 0, "hello" };
        ASSERT_EQ(overload(t0), "tuple");
    }
    SECTION("Variant") {
        std::variant<int, std::string> v0 = 0;
        ASSERT_EQ(overload(v0), "variant");
    }
}