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

/// Small test to prove where we should use std::remove_cvref_t
/// or not.

template<typename T>
concept badconcept = requires(T t) {
    typename T::value_type;
};

template <typename T>
concept goodconcept = requires(T t) {
    typename std::remove_cvref_t<T>::value_type;
};

template <typename T> 
std::string puretype_overload(T&& t) requires badconcept<T> 
{
    return "badconcept";
};

template <typename T>
std::string puretype_overload(T&& t) requires goodconcept<T> {
    return "goodconcept";
};

template <typename T>
std::string puretype_overload(T&& t) {
    return "p_auto";
};

TEST_CASE("Prove that we should std::remove_cvref_t concepts with member types") {
    SECTION("auto") {
        ASSERT_EQ(puretype_overload(int{}), "p_auto");
    }
    SECTION("badconcept") {
        std::vector<int> v;
        ASSERT_NE(puretype_overload(v), "badconcept");
    }
    SECTION("goodconcept") {
        std::vector<int> v;
        ASSERT_EQ(puretype_overload(v), "goodconcept");
    }
}

/// Library tests.
///


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
std::string overload(T&& t) requires teg::variant<T> {
    return "variant";
}

template <typename T>
std::string overload(T&& t) {
    return "auto";
}

TEST_CASE("Overload resolution with concepts") {
    SECTION("auto") {
        int i;
        ASSERT_EQ(overload(i), "auto");
    }
    SECTION("optional") {
        std::optional<int> opt0;
        ASSERT_EQ(overload(opt0), "optional");
    }
    SECTION("container") {
        std::forward_list<int> fl;
        ASSERT_EQ(overload(fl), "container");
    }
    SECTION("cv-qualified container") {
        using const_vector = std::vector<int> const;
        const_vector v;
        ASSERT_EQ(overload(v), "contiguous_container");

        std::string s0 = "hello";

        using const_string = std::string const;
        const_string s1 = s0;
        ASSERT_EQ(overload(s1), "contiguous_container");
    }
    SECTION("owning pointer") {
        std::unique_ptr<int> up = std::make_unique<int>(0);
        ASSERT_EQ(overload(up), "owning_pointer");
    }
    SECTION("variant") {
        std::variant<int, std::string> v0 = 0;
        ASSERT_EQ(overload(v0), "variant");
    }
}