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

namespace teg {	

class test_fixture {
public:
    test_fixture() 
        : m_next_test(nullptr) 
    {
        if (!m_first_test) {
            m_first_test = this;
        } else {
            m_last_test->m_next_test = this;
        }
        m_last_test = this;
    }

    virtual bool run() {
        setup();
        test();
        teardown();
        return true;
    }

    virtual void setup() {}
    virtual void test() = 0;
    virtual void teardown() {}
    virtual ~test_fixture() = default;

    static test_fixture* first_test() { 
        return m_first_test; 
    }

    test_fixture* next_test() { 
        return m_next_test; 
    }

    virtual const char* name() const = 0;
    virtual const char* group() const = 0;

protected:
    static test_fixture* m_first_test;
    static test_fixture* m_last_test;
    test_fixture* m_next_test;
};

test_fixture* test_fixture::m_first_test = nullptr;
test_fixture* test_fixture::m_last_test = nullptr;

bool execute_all_tests() {
    bool result = true;
    for (auto test = test_fixture::first_test(); test; test = test->next_test()) {
        result &= test->run();
    }
    return result;
}

} // namespace teg

#define TEG_CAT_(a, b) a ## _ ## b
#define TEG_CAT(a, b) TEG_CAT_(a, b)

#define TEG_BASE_TEST_FIXTURE() teg::test_fixture

#define TEG_UNIQUE_TEST_NAME_(name, count) TEG_CAT(name, count)
#define TEG_UNIQUE_TEST_NAME() TEG_UNIQUE_TEST_NAME_(test, __COUNTER__)

#define TEG_DEFINE_TEST_CASE(test_name, test_group, test_fixture)          \
    class TEG_CAT(test_group, test_name) final : public test_fixture {     \
    public:                                                                \
        const char* name() const override {                                \
            return #test_name;                                             \
        }                                                                  \
        const char* group() const override {                               \
            return #test_group;                                            \
        }                                                                  \
        void test() override;                                              \
    } TEG_CAT(TEG_CAT(test_group, test_name), instance);                   \
    void TEG_CAT(test_group, test_name)::test()                            \


#define TEG_TEST_CASE(name)                    TEG_DEFINE_TEST_CASE(TEG_UNIQUE_TEST_NAME(), global, TEG_BASE_TEST_FIXTURE())
#define TEG_TEST_CASE_F(name, fixture)         TEG_DEFINE_TEST_CASE(TEG_UNIQUE_TEST_NAME(), global, fixture)
#define TEG_TEST_CASE_G(name, group)           TEG_DEFINE_TEST_CASE(TEG_UNIQUE_TEST_NAME(), group, TEG_BASE_TEST_FIXTURE())
#define TEG_TEST_CASE_GF(name, group, fixture) TEG_DEFINE_TEST_CASE(TEG_UNIQUE_TEST_NAME(), group, fixture)


#if !defined(TEG_MANUAL_TEST_MAIN_ENTRY) && !defined(TEG_AUTO_TEST_MAIN_ENTRY)
#define TEG_AUTO_TEST_MAIN_ENTRY

int main() {                                                           
    return teg::execute_all_tests() ? 0 : 1;                           
}
                     
#endif