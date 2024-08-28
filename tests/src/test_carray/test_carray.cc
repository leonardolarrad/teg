#include <cstring>
#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Serialize and deserialize c arrays") {
    // array of bool
    {
        teg::buffer b;
        bool arr0[5] = { true, false, true, false, true };
        teg::serialize(b, arr0).or_throw();

        bool arr1[5];
        teg::deserialize(b, arr1).or_throw();
        ASSERT_EQ(std::memcmp(arr0, arr1, sizeof(arr0)), 0);
    }
    // array of int
    {
        teg::buffer b;
        int arr0[5] = { 1, 2, 3, 4, 5 };
        teg::serialize(b, arr0).or_throw();

        int arr1[5];
        teg::deserialize(b, arr1).or_throw();
        ASSERT_EQ(std::memcmp(arr0, arr1, sizeof(arr0)), 0);
    }
    // array of char
    {
        teg::buffer b;
        char arr0[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
        teg::serialize(b, arr0).or_throw();

        char arr1[10];
        teg::deserialize(b, arr1).or_throw();
        ASSERT_EQ(std::memcmp(arr0, arr1, sizeof(arr0)), 0);
    }
    // array of float
    {
        teg::buffer b;
        float arr0[30] = { 
            01.f, 02.f, 03.f, 04.f, 05.f, 06.f, 07.f, 08.f, 09.f, 10.f, 
            11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f,
            21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f, 30.f };
        teg::serialize(b, arr0).or_throw();

        float arr1[30];
        teg::deserialize(b, arr1).or_throw();
        ASSERT_EQ(std::memcmp(arr0, arr1, sizeof(arr0)), 0);
    }    
}

TEST_CASE("Serialize and deserialize multiple c arrays in the same buffer") {
    teg::buffer b;

    bool arr0[5] = { true, false, true, false, true };
    int arr1[5] = { 1, 2, 3, 4, 5 };
    char arr2[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
    float arr3[30] = { 
        01.f, 02.f, 03.f, 04.f, 05.f, 06.f, 07.f, 08.f, 09.f, 10.f, 
        11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f,
        21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f, 30.f 
    };
    teg::serialize(b, arr0, arr1, arr2, arr3).or_throw();

    bool arr4[5];
    int arr5[5];
    char arr6[10];
    float arr7[30];
    teg::deserialize(b, arr4, arr5, arr6, arr7).or_throw();

    ASSERT_EQ(std::memcmp(arr0, arr4, sizeof(arr0)), 0);
    ASSERT_EQ(std::memcmp(arr1, arr5, sizeof(arr1)), 0);
    ASSERT_EQ(std::memcmp(arr2, arr6, sizeof(arr2)), 0);
    ASSERT_EQ(std::memcmp(arr3, arr7, sizeof(arr3)), 0);
}

TEST_CASE("Serialize and deserialize c arrays members") {
    {
        struct int_arr_2_16 {
            int a[16];
            int b[16];
        };

        teg::buffer b;
        int_arr_2_16 arr0 = { 
            {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16 }, 
            { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 } 
        };
        teg::serialize(b, arr0).or_throw();

        int_arr_2_16 arr1;
        teg::deserialize(b, arr1).or_throw();

        ASSERT_EQ(std::memcmp(arr0.a, arr1.a, sizeof(arr0.a)), 0);
        ASSERT_EQ(std::memcmp(arr0.b, arr1.b, sizeof(arr0.b)), 0);
    }
    {
        struct char_arr_2_32 {
            char a[32];
            char b[32];
        };

        teg::buffer b;
        char_arr_2_32 arr0 = { 
            {
                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F'
            },
            {
                'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'
            }
        };
        teg::serialize(b, arr0).or_throw();

        char_arr_2_32 arr1;
        teg::deserialize(b, arr1).or_throw();

        ASSERT_EQ(std::memcmp(arr0.a, arr1.a, sizeof(arr0.a)), 0);
        ASSERT_EQ(std::memcmp(arr0.b, arr1.b, sizeof(arr0.b)), 0);
    }
}