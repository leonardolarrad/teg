// Copyright (c) 2024 Adrian & Leonardo Larrad.
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and mu    st not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once
#include "test_fixture.h"

#define BASE_TEST_FIXTURE() test::test_fixture


#define _CAT(a, b) a ## _ ## b
#define CAT(a, b) _CAT(a, b)


#define _UNIQUE_TEST_NAME(name, count) CAT(name, count)
#define UNIQUE_TEST_NAME() _UNIQUE_TEST_NAME(test, __COUNTER__)


#define DEFINE_TEST_CASE(test_name, test_group, test_fixture)          \
    class CAT(test_group, test_name) final : public test_fixture {     \
    public:                                                            \
        const char* name() const override {                            \
            return #test_name;                                         \
        }                                                              \
        const char* group() const override {                           \
            return #test_group;                                        \
        }                                                              \
        void test() override;                                          \
    } CAT(CAT(test_group, test_name), instance);                       \
    void CAT(test_group, test_name)::test()                            \

#define TEST_CASE(name)                     DEFINE_TEST_CASE(UNIQUE_TEST_NAME(), global, BASE_TEST_FIXTURE())
#define TEST_CASE_F(name, fixture)          DEFINE_TEST_CASE(UNIQUE_TEST_NAME(), global, fixture)
#define TEST_CASE_G(name, group)            DEFINE_TEST_CASE(UNIQUE_TEST_NAME(), group, BASE_TEST_FIXTURE())
#define TEST_CASE_GF(name, group, fixture)  DEFINE_TEST_CASE(UNIQUE_TEST_NAME(), group, fixture)


#define _ASSERT_BINARY(source, a, b, op)                                      \
    do {                                                                      \
        const auto& val_a = a;                                                \
        const auto& val_b = b;                                                \
        auto current_test = test::test_fixture::current_test();               \
        current_test->eval((val_a) op (val_b), source, __FILE__, __LINE__,    \
            std::format("{} {} {}", val_a, #op, val_b).c_str());              \
    } while (false);                                                             

#define _ASSERT_UNARY(source, cond)                                           \
    do {                                                                      \
        auto current_test = test::test_fixture::current_test();               \
        current_test->eval(cond, source, __FILE__, __LINE__,                  \
            std::format("{}", cond).c_str());                                 \
    } while (false);                                                             

#define ASSERT_EQ(a, b)  _ASSERT_BINARY("ASSERT_EQ(" #a ", " #b ")", a, b, ==)
#define ASSERT_NE(a, b)  _ASSERT_BINARY("ASSERT_NE(" #a ", " #b ")", a, b, !=)
#define ASSERT_LT(a, b)  _ASSERT_BINARY("ASSERT_LT(" #a ", " #b ")", a, b, <)
#define ASSERT_LE(a, b)  _ASSERT_BINARY("ASSERT_LE(" #a ", " #b ")", a, b, <=)
#define ASSERT_GT(a, b)  _ASSERT_BINARY("ASSERT_GT(" #a ", " #b ")", a, b, >)
#define ASSERT_GE(a, b)  _ASSERT_BINARY("ASSERT_GE(" #a ", " #b ")", a, b, >=)
#define ASSERT(cond)     _ASSERT_UNARY("ASSERT(" #cond ")", cond)


#if !defined(MANUAL_TEST_MAIN_ENTRY) && !defined(AUTO_TEST_MAIN_ENTRY)
#define AUTO_TEST_MAIN_ENTRY

int main() {                                                           
    return test::execute_all_tests();                           
}
                     
#endif