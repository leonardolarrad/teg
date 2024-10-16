#include <string>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("Trivial de/serialization") {    
    teg::byte_buffer b;
    std::string s0 = 
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus nec " 
      "finibus tortor, at egestas mi. Nam blandit lectus scelerisque neque posuere "
      "porta. Sed dignissim erat id mollis porttitor. Nam finibus velit at neque "
      "mattis viverra at vitae leo. Vestibulum ante ipsum primis in faucibus orci "
      "luctus et ultrices posuere cubilia curae; Morbi non erat feugiat, posuere justo "
      "et, pulvinar eros. Proin eget ipsum euismod, dapibus tellus vitae, ultricies "
      "ligula. Suspendisse potenti. Cras eleifend lacus sit amet tempor egestas. Nulla "
      "ac efficitur dui, a bibendum lectus. Cras risus nulla, sodales eu lorem at, "
      "sollicitudin posuere lectus. Fusce gravida rhoncus nisl, eget malesuada dui tincidunt "
      "quis. Phasellus dictum molestie cursus. Mauris maximus, magna a tempor vehicula, neque "
      "turpis interdum tellus, nec semper libero sem a metus. Nam tristique felis non felis "
      "rutrum, non consequat arcu porta.";
    teg::serialize(b, s0).or_throw();

    std::string s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT_EQ(s0, s1);
}

TEST_CASE("De/serialize const string") {
    teg::byte_buffer b;
    std::string const s0 = "Hello World!";
    teg::serialize(b, s0).or_throw();

    std::string s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}

TEST_CASE("De/serialize u8string") {
    teg::byte_buffer b;
    std::u8string s0 = 
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

    std::u8string s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}

TEST_CASE("De/serialize u16string") {
    teg::byte_buffer b;
    std::u16string s0 =
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

    std::u16string s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}

TEST_CASE("De/serialize u32string") {
    teg::byte_buffer b;
    std::u32string s0 = 
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

    std::u32string s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}

TEST_CASE("De/serialize wstring") {
    teg::byte_buffer b;
    std::wstring s0 =
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

    std::wstring s1;
    teg::deserialize(b, s1).or_throw();
    ASSERT(s0 == s1);
}

