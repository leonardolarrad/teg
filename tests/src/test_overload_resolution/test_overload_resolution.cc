#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <stack>
#include <queue>
#include <string>
#include <iostream>

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
std::string overload(T&& t) requires teg::contiguous_container<T>
{
    return "contiguous_container";
}

template <typename T> 
std::string overload(T&& t) requires teg::container<T>
{
    return "container";
}

template <typename T>
std::string overload(T&& t) {
    return "auto";
}

TEST_CASE("Overload resolution with concepts") {
    SECTION("auto") {
        int i;
        ASSERT(overload(i) == "auto");
    }
    SECTION("container") {
        std::forward_list<int> fl;
        ASSERT(overload(fl) == "container");
    }
    SECTION("cv-qualified container") {
        using const_vector = std::vector<int> const;
        const_vector v;
        ASSERT(overload(v) == "contiguous_container");

        std::string s0 = "hello";

        using const_string = std::string const;
        const_string s1 = s0;
        ASSERT(overload(s1) == "contiguous_container");
    }
}