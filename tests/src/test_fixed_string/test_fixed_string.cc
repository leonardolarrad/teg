#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Fixed string interface") {
    constexpr teg::fixed_string<5> str0 = "Hello";
    constexpr teg::fixed_string<6> str1 = "World!";
    constexpr teg::fixed_string<9> str2 {"Moonlight"};
    
    SECTION("Constructors") {
        constexpr teg::fixed_string<1> str3 = "7";
        ASSERT(str3 == "7");
    }
    SECTION("Capacity") {
        ASSERT(str0.size() == 5);
        ASSERT(str1.size() == 6);
        ASSERT(str2.size() == 9);

        ASSERT(str0.length() == 5);
        ASSERT(str1.length() == 6);
        ASSERT(str2.length() == 9);

        ASSERT(str0.max_size() == 5);
        ASSERT(str1.max_size() == 6);
        ASSERT(str2.max_size() == 9);
    }
    SECTION("Comparison") {
        ASSERT("Hello" == str0);
        ASSERT("World!" == str1);
        ASSERT("Moonlight!" != str2);
        ASSERT(str0 != str1);
        ASSERT(str0 < str1);
        ASSERT(str1 > str0);
        ASSERT(str0 <= str0);
        ASSERT(str0 >= str0);
        ASSERT(str1 >= str0);
    }
    SECTION("Concatenation") {
        constexpr auto str3 = str0 + str1;
        ASSERT(str3 == "HelloWorld!");
    }
    SECTION("Concatenation of comptime fixed strings with runtime fixed strings") {
        teg::fixed_string<5> str5 = "Hello";
        constexpr teg::fixed_string<6> str6 = "World!";
        auto str7 = str5 + str6;
        auto str8 = str6 + teg::fixed_string<1> {{(char)33}};

        auto l = [](int i) {
            return teg::fixed_string<1> {{(char)i}};
        };

        auto str9 = str6 + l(33) + l(33);

        ASSERT(str5 == "Hello");
        ASSERT(str6 == "World!");
        ASSERT(str7 == "HelloWorld!");
        ASSERT(str8 == "World!!");
        ASSERT(str9 == "World!!!");
    }
}

TEST_CASE("De/serialize fixed string") {
    teg::byte_buffer b;
    constexpr teg::fixed_string<5> str0 = "Hello";
    teg::serialize(b, str0).or_throw();

    teg::fixed_string<5> str1;
    teg::deserialize(b, str1).or_throw();
    ASSERT(str0 == str1);
}

TEST_CASE("De/serialize fixed u8string") {
    teg::byte_buffer b;
    constexpr teg::fixed_u8string<900> s0 = 
        u8"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus nec " 
        u8"finibus tortor, at egestas mi. Nam blandit lectus scelerisque neque posuere "
        u8"porta. Sed dignissim erat id mollis porttitor. Nam finibus velit at neque "
        u8"mattis viverra at vitae leo. Vestibulum ante ipsum primis in faucibus orci "
        u8"luctus et ultrices posuere cubilia curae; Morbi non erat feugiat, posuere justo "
        u8"et, pulvinar eros. Proin eget ipsum euismod, dapibus tellus vitae, ultricies "
        u8"ligula. Suspendisse potenti. Cras eleifend lacus sit amet tempor egestas. Nulla "
        u8"ac efficitur dui, a bibendum lectus. Cras risus nulla, sodales eu lorem at, "
        u8"sollicitudin posuere lectus. Fusce gravida rhoncus nisl, eget malesuada dui tincidunt "
        u8"quis. Phasellus dictum molestie cursus. Mauris maximus, magna a tempor vehicula, neque "
        u8"turpis interdum tellus, nec semper libero sem a metus. Nam tristique felis non felis "
        u8"rutrum, non consequat arcu porta.";
    teg::serialize(b, s0).or_throw();

    teg::fixed_u8string<900> s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}

TEST_CASE("De/serialize fixed u16string") {
    teg::byte_buffer b;
    constexpr teg::fixed_u16string<900> s0 = 
        u"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus nec " 
        u"finibus tortor, at egestas mi. Nam blandit lectus scelerisque neque posuere "
        u"porta. Sed dignissim erat id mollis porttitor. Nam finibus velit at neque "
        u"mattis viverra at vitae leo. Vestibulum ante ipsum primis in faucibus orci "
        u"luctus et ultrices posuere cubilia curae; Morbi non erat feugiat, posuere justo "
        u"et, pulvinar eros. Proin eget ipsum euismod, dapibus tellus vitae, ultricies "
        u"ligula. Suspendisse potenti. Cras eleifend lacus sit amet tempor egestas. Nulla "
        u"ac efficitur dui, a bibendum lectus. Cras risus nulla, sodales eu lorem at, "
        u"sollicitudin posuere lectus. Fusce gravida rhoncus nisl, eget malesuada dui tincidunt "
        u"quis. Phasellus dictum molestie cursus. Mauris maximus, magna a tempor vehicula, neque "
        u"turpis interdum tellus, nec semper libero sem a metus. Nam tristique felis non felis "
        u"rutrum, non consequat arcu porta.";
    teg::serialize(b, s0).or_throw();

    teg::fixed_u16string<900> s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}

TEST_CASE("De/serialize fixed u32string") {
    teg::byte_buffer b;
    constexpr teg::fixed_u32string<900> s0 = 
        U"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus nec " 
        U"finibus tortor, at egestas mi. Nam blandit lectus scelerisque neque posuere "
        U"porta. Sed dignissim erat id mollis porttitor. Nam finibus velit at neque "
        U"mattis viverra at vitae leo. Vestibulum ante ipsum primis in faucibus orci "
        U"luctus et ultrices posuere cubilia curae; Morbi non erat feugiat, posuere justo "
        U"et, pulvinar eros. Proin eget ipsum euismod, dapibus tellus vitae, ultricies "
        U"ligula. Suspendisse potenti. Cras eleifend lacus sit amet tempor egestas. Nulla "
        U"ac efficitur dui, a bibendum lectus. Cras risus nulla, sodales eu lorem at, "
        U"sollicitudin posuere lectus. Fusce gravida rhoncus nisl, eget malesuada dui tincidunt "
        U"quis. Phasellus dictum molestie cursus. Mauris maximus, magna a tempor vehicula, neque "
        U"turpis interdum tellus, nec semper libero sem a metus. Nam tristique felis non felis "
        U"rutrum, non consequat arcu porta.";
    teg::serialize(b, s0).or_throw();

    teg::fixed_u32string<900> s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}

TEST_CASE("De/serialize fixed wstring") {
    teg::byte_buffer b;
    constexpr teg::fixed_wstring<900> s0 = 
        L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus nec " 
        L"finibus tortor, at egestas mi. Nam blandit lectus scelerisque neque posuere "
        L"porta. Sed dignissim erat id mollis porttitor. Nam finibus velit at neque "
        L"mattis viverra at vitae leo. Vestibulum ante ipsum primis in faucibus orci "
        L"luctus et ultrices posuere cubilia curae; Morbi non erat feugiat, posuere justo "
        L"et, pulvinar eros. Proin eget ipsum euismod, dapibus tellus vitae, ultricies "
        L"ligula. Suspendisse potenti. Cras eleifend lacus sit amet tempor egestas. Nulla "
        L"ac efficitur dui, a bibendum lectus. Cras risus nulla, sodales eu lorem at, "
        L"sollicitudin posuere lectus. Fusce gravida rhoncus nisl, eget malesuada dui tincidunt "
        L"quis. Phasellus dictum molestie cursus. Mauris maximus, magna a tempor vehicula, neque "
        L"turpis interdum tellus, nec semper libero sem a metus. Nam tristique felis non felis "
        L"rutrum, non consequat arcu porta.";
    teg::serialize(b, s0).or_throw();

    teg::fixed_wstring<900> s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}