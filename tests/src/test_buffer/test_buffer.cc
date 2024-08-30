#include <cstdint>
#include <vector>
#include <array>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Compute trivial buffer size") {    
    ASSERT_EQ(teg::buffer_size(char{}), sizeof(char));
    ASSERT_EQ(teg::buffer_size(int{}), sizeof(int));
    ASSERT_EQ(teg::buffer_size(double{}), sizeof(double));
}

TEST_CASE("Compute container buffer size") {    
    // Fixed size containers
    {
        std::array<char, 5> a0;
        std::array<int, 50> a1;
        std::array<double, 99> a2;
        
        ASSERT_EQ(teg::buffer_size(a0), sizeof(char) * 5);
        ASSERT_EQ(teg::buffer_size(a1), sizeof(int) * 50);
        ASSERT_EQ(teg::buffer_size(a2), sizeof(double) * 99);

        std::array<std::array<char, 5>, 5> a3;
        ASSERT_EQ(teg::buffer_size(a3), sizeof(char) * 5 * 5);

        std::array<std::array<char, 100>, 99> a4;
        ASSERT_EQ(teg::buffer_size(a4), sizeof(char) * 100 * 99);

    }
    // Contiguos containers
    {
        std::vector<char> v0;
        std::vector<int> v1 = { 1, 2, 3 };
        std::vector<double> v2 = { 1.0, 2.0, 3.0, 4.0 };

        ASSERT_EQ(teg::buffer_size(v0), sizeof(std::size_t));
        ASSERT_EQ(teg::buffer_size(v1), sizeof(std::size_t) + sizeof(int) * 3);
        ASSERT_EQ(teg::buffer_size(v2), sizeof(std::size_t) + sizeof(double) * 4);
    }

}