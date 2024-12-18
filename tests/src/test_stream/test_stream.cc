#include <ostream>
#include <istream>
#include <fstream>

#include "teg/teg.h"
#include "test/test.h"

TEST_CASE("De/serialize to stream: Fundamentals") {            
    teg::i32 i01 = 11, i02 = 5, i03 = 99;    
    {
        std::ofstream out("fundamentals.bin", std::ios::binary | std::ios::trunc);
        teg::serialize(out, i01, i02, i03).or_throw();
    }

    teg::i32 i11, i12, i13;
    {
        std::ifstream in("fundamentals.bin", std::ios::binary);
        teg::deserialize(in, i11, i12, i13).or_throw();    
    }

    ASSERT(i01 == i11);
    ASSERT(i02 == i12);
    ASSERT(i03 == i13);
}

TEST_CASE("De/serialize to stream: Arrays") {            
    std::array<teg::u8, 5> b0 = { 1, 2, 3, 4, 5 };
    {
        std::ofstream out("arrays.bin", std::ios::binary | std::ios::trunc);
        teg::serialize(out, b0).or_throw();
    }

    std::array<teg::u8, 5> b1 = { 5, 4, 3, 2, 1 };
    {
        std::ifstream in("arrays.bin", std::ios::binary);
        teg::deserialize(in, b1).or_throw();    
    }

    ASSERT(std::memcmp(b0.data(), b1.data(), b0.size()) == 0);
}

TEST_CASE("De/serialize to stream: Strings") {            
    std::string h0 = "TEG";
    std::string s0 = "This is how liberty dies ... with thunderous applause.";
    {
        std::ofstream out("strings.bin", std::ios::binary | std::ios::trunc);
        teg::serialize(out, h0, s0).or_throw();
    }

    std::string h1;
    std::string s1;
    {
        std::ifstream in("strings.bin", std::ios::binary);
        teg::deserialize(in, h1, s1).or_throw();    
    }

    ASSERT(h0 == h1);
    ASSERT(s0 == s1);
}

TEST_CASE("Small test") {
    const char* s = "This is how liberty dies ... with thunderous applause.";

    std::ofstream out("small.bin", std::ios::binary | std::ios::trunc);
    /*out << s;*/
    out.write(s, strlen(s));
    

    teg::serialize(out, "!TEG^").or_throw();
}
