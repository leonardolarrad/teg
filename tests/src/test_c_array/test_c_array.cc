#include <array>
#include <cstring>
#include <cstdint>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Assert concepts") {    
    ASSERT(teg::c_array<int[5]>);                        // OK: One dimensional c-array
    ASSERT(teg::c_array<int[5][5]>);                     // OK: Two dimensional c-array
    ASSERT(teg::c_array<int[5][5][5]>);                  // OK: Three dimensional c-array

    ASSERT(!teg::c_array<int[]>);                        // Failed: Empty c-arrays are not allowed.
    ASSERT(!teg::c_array<const int*>);                   // Failed: Pointers are not c-arrays.
}

TEST_CASE("De/serialize c-arrays") {
    SECTION("Bool c-array") {
        teg::buffer b;
        
        bool arr0[5] = { true, false, true, false, true };
        teg::serialize(b, arr0).or_throw();

        bool arr1[5];
        teg::deserialize(b, arr1).or_throw();
        
        ASSERT_EQ(std::memcmp(arr0, arr1, sizeof(arr0)), 0);
    }
    SECTION("Int c-array") {
        teg::buffer b;
        
        int arr0[5] = { 1, 2, 3, 4, 5 };
        teg::serialize(b, arr0).or_throw();

        
        int arr1[5];
        teg::deserialize(b, arr1).or_throw();
        
        ASSERT_EQ(std::memcmp(arr0, arr1, sizeof(arr0)), 0);
    }
    SECTION("Char c-array") {
        teg::buffer b;
        
        char arr0[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };        
        teg::serialize(b, arr0).or_throw();

        char arr1[10];
        teg::deserialize(b, arr1).or_throw();
        
        ASSERT_EQ(std::memcmp(arr0, arr1, sizeof(arr0)), 0);
    }
    SECTION("Float c-array") {
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

TEST_CASE("De/serialize multidimensional c-arrays") {
    SECTION("Two dimensional c-array") {
        using md_int_2_t = int[3][5];
        teg::buffer b;

        md_int_2_t md0 = { { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 } };
        teg::serialize(b, md0).or_throw();

        md_int_2_t md1;
        teg::deserialize(b, md1).or_throw();
        
        ASSERT_EQ(std::memcmp(md0, md1, sizeof(md0)), 0);
    }
    SECTION("Three dimensional c-array") {
        using md_int_3_t = int[3][4][5];
        teg::buffer b;

        md_int_3_t md0 = { 
            { { 1, 2, 3, 4, 5 }, { 6, 7, 8, 9, 10 }, { 11, 12, 13, 14, 15 }, { 16, 17, 18, 19, 20 } },
            { { 21, 22, 23, 24, 25 }, { 26, 27, 28, 29, 30 }, { 31, 32, 33, 34, 35 }, { 36, 37, 38, 39, 40 } },
            { { 41, 42, 43, 44, 45 }, { 46, 47, 48, 49, 50 }, { 51, 52, 53, 54, 55 }, { 56, 57, 58, 59, 60 } }
        };

        teg::serialize(b, md0).or_throw();

        md_int_3_t md1;
        teg::deserialize(b, md1).or_throw();

        ASSERT_EQ(std::memcmp(md0, md1, sizeof(md0)), 0);
    }
}

TEST_CASE("De/serialize trivially copyable c-arrays") {
    SECTION("128 bytes c-array") {
        struct int_2_16_t {
            int a[16];
            int b[16];
        };

        int_2_16_t arr0 = { 
            {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16 }, 
            { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32 } 
        };
        
        teg::buffer b;
        teg::serialize(b, arr0).or_throw();

        int_2_16_t arr1;
        teg::deserialize(b, arr1).or_throw();

        ASSERT_EQ(std::memcmp(arr0.a, arr1.a, sizeof(arr0.a)), 0);
        ASSERT_EQ(std::memcmp(arr0.b, arr1.b, sizeof(arr0.b)), 0);
    }
    SECTION("64 bytes c-array") {
        struct char_2_32_t {
            char a[32];
            char b[32];
        };

        char_2_32_t arr0 = { 
            {
                'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F'
            },
            {
                'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'
            }
        };
        
        teg::buffer b;
        teg::serialize(b, arr0).or_throw();

        char_2_32_t arr1;
        teg::deserialize(b, arr1).or_throw();

        ASSERT_EQ(std::memcmp(arr0.a, arr1.a, sizeof(arr0.a)), 0);
        ASSERT_EQ(std::memcmp(arr0.b, arr1.b, sizeof(arr0.b)), 0);
    }
}

TEST_CASE("De/serialize multiple c-arrays in the same buffer") {
    bool arr0[5] = { true, false, true, false, true };
    int arr1[5] = { 1, 2, 3, 4, 5 };
    char arr2[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' };
    float arr3[30] = { 
        01.f, 02.f, 03.f, 04.f, 05.f, 06.f, 07.f, 08.f, 09.f, 10.f, 
        11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f, 19.f, 20.f,
        21.f, 22.f, 23.f, 24.f, 25.f, 26.f, 27.f, 28.f, 29.f, 30.f 
    };

    teg::buffer b;
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

TEST_CASE("De/serialize non-trivially copyable c-arrays") {    
    std::string a0[5] = { "word0", "word1", "word2", "word3", "word4" };

    teg::buffer b;
    teg::serialize(b, a0).or_throw();

    std::string a1[5];
    teg::deserialize(b, a1).or_throw();

    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(a0[i], a1[i]);
    }    
}

TEST_CASE("De/serialize aggregates with c-arrays members") {
    struct aggregate_t {
        std::string str0;
        char str1[10];
        std::string str2;
        char str3[10];

        constexpr bool operator==(aggregate_t const&) const = default;
    };

    teg::buffer b;
    
    aggregate_t agg0 = {
        "Text 0",
        { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j' },
        "Text 1",
        { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' }
    };
    teg::serialize(b, agg0).or_throw();

    aggregate_t agg1;
    teg::deserialize(b, agg1).or_throw();

    ASSERT(agg0 == agg1);
}