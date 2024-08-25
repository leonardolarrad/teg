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
#include <cstddef>
#include <format>
#include <iostream>

namespace test {	

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
        m_asserts_count = 0;
        m_errors_count = 0;        
        m_curr_test = this;
        setup();
        test();
        teardown();
        m_curr_test = nullptr;
        return m_errors_count == 0;
    }

    virtual void eval(bool condition, const char* source, 
        const char* file, int line, const char* expansion) 
    {
        ++m_asserts_count;
        if (!condition) {
            ++m_errors_count;
            std::cerr << std::format("{}:{}: ", file, line) << "\033[1;31m" << "failed:\033[0m\n";
            std::cerr << std::format("  {}\n", source);
            std::cerr << "with expansion:\n";
            std::cerr << std::format("  {}\n", expansion);
        }
        else {
            ++m_passes_count;
            std::cout << std::format("{}:{}: ", file, line) << "\033[1;32m" << "passed\033[0m\n";
        }
    }

    virtual void setup() {}
    virtual void test() = 0;
    virtual void teardown() {}
    virtual ~test_fixture() = default;

    static test_fixture* first_test() { 
        return m_first_test;
    }

    static test_fixture* current_test() { 
        return m_curr_test;
    }

    test_fixture* next_test() { 
        return m_next_test;
    }

    std::size_t asserts_count() const { 
        return m_asserts_count;
    }

    std::size_t passes_count() const { 
        return m_passes_count;
    }

    std::size_t errors_count() const { 
        return m_errors_count;
    }

    virtual const char* name() const = 0;
    virtual const char* group() const = 0;

protected:
    static test_fixture* m_first_test;
    static test_fixture* m_last_test;
    static test_fixture* m_curr_test;
    test_fixture* m_next_test;

    std::size_t m_asserts_count = 0;
    std::size_t m_passes_count = 0;
    std::size_t m_errors_count = 0;
};

test_fixture* test_fixture::m_first_test = nullptr;
test_fixture* test_fixture::m_last_test = nullptr;
test_fixture* test_fixture::m_curr_test = nullptr;

int execute_all_tests() {
    int asserts_count = 0;
    int passes_count = 0;
    int errors_count = 0;

    for (auto test = test_fixture::first_test(); test; test = test->next_test()) {        
        test->run();
        asserts_count += test->asserts_count();
        passes_count += test->passes_count();
        errors_count += test->errors_count();
    }

    std::cout << "\nSummary:\n\n";
    std::cout << "Ok:                 " << passes_count << " / " << asserts_count << "\n";
    std::cout << "Fail:               " << errors_count << " / " << asserts_count << "\n";

    return errors_count;
}

} // namespace teg  