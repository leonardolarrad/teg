#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"
#include <iostream>
#include <fstream>

namespace bm = benchmarking;

static std::vector<bm::ecommerce_page> data_out_1mib = bm::generate_benchmark_data(512, 1024*2);

static bool test_lib() {
    teg::byte_array b;
    bm::ecommerce_page t0 = data_out_1mib[0];
    auto r0 = teg::serialize(b, t0);
    if (teg::failure(r0)) {
        return false;
    }

    bm::ecommerce_page t1;
    teg::error r1 = teg::deserialize(b, t1);
    if (teg::failure(r1)) {
        return false;
    }    
    return t0 == t1;
}

static void benchmark_00() {   
    std::vector<bm::ecommerce_page> data_in_1mib;
    teg::byte_array buffer{};
    std::string schema_in{};

    constexpr auto default_mode = teg::default_mode;
    constexpr auto force_varint = teg::default_mode | teg::options::force_varint;

    constexpr auto schema = teg::schema<1, bm::ecommerce_page>();
    std::cout << "Schema size: " << schema.size() << std::endl;
    std::cout << "Schema: " << schema.c_str() << std::endl;
    bm::benchmark()
        .warmup(10)
        .iterations(10)
        .repetitions(10)
        .run("teg:serialization:1mib", [&](){
            teg::serialize<force_varint>(buffer, std::string{schema.c_str(), schema.size()}, data_out_1mib).or_throw();
        })
        .run("teg:deserialization:1mib", [&](){
            teg::deserialize<force_varint>(buffer, schema_in, data_in_1mib).or_throw();
        });

    std::cout << "\nBuffer size: " << buffer.size() << std::endl;
}

static void benchmark_stream() {   
    std::vector<bm::ecommerce_page> data_in_1mib;
    std::ofstream out("bmsteam.bin", std::ios::binary | std::ios::trunc);

    constexpr auto schema = teg::schema<1, bm::ecommerce_page>();
    std::cout << "Schema size: " << schema.size() << std::endl;
    std::cout << "Schema: " << schema.c_str() << std::endl;
    bm::benchmark()
        .warmup(10)
        .iterations(10)
        .repetitions(10)
        .run("teg:serialization:stream:1mib", [&](){
            teg::serialize(out, std::string{schema.c_str(), schema.size()}, data_out_1mib).or_throw();
        })
        ;
}

static void benchmark_01() {
    int64_t d0 = 99999999999999ull;
    int64_t d1;
    teg::byte_array buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(1024 * 1024 * 10)
        .repetitions(10)
        .run("teg:serialization:i64", [&](){
            teg::serialize(buffer, d0).or_throw();
        })
        .run("teg:deserialization:i64", [&](){
            teg::deserialize(buffer, d1).or_throw();
        });
}

static void benchmark_02() {
    std::string d0 = "Hello World!";
    std::string d1;
    teg::byte_array buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(1024 * 1024)
        .repetitions(10)
        .run("teg:serialization:simple_string", [&](){
            buffer.clear();
            teg::serialize(buffer, d0).or_throw();
        })
        .run("teg:deserialization:simple_string", [&](){
            teg::deserialize(buffer, d1).or_throw();
        });
}

static void benchmark_03() {
    teg::byte_array buffer{};
    teg::byte_array buffer2{};
    std::string data = 
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
    ;

    bm::benchmark()
        .warmup(10)
        .iterations(1000)
        .repetitions(10)
        .run("teg:serialization:checksum", [&](){
            //buffer2.clear();
            teg::serialize(buffer, data).or_throw();
            //teg::serialize(buffer2, teg::md5_hash_u64(std::span<const std::byte>{buffer})).or_throw();
        });
}

static void benchmark_04() {
    teg::byte_array buffer{};
    teg::byte_array buffer2{};
    std::string data = 
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAxAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
    ;

    bm::benchmark()
        .warmup(10)
        .iterations(1000)
        .repetitions(10)
        .run("teg:serialization:xxhash", [&](){
            buffer.clear();
            buffer2.clear();
            teg::serialize(buffer, data).or_throw();
            teg::serialize(buffer2, teg::xxhash::hash64(std::span<const std::byte>{buffer})).or_throw();
        });
    for (auto b : buffer2) {
        std::cout << std::hex << static_cast<int>(b) << " ";
    }
    std::cout << std::endl;
}

int main() {
    if (!test_lib()) {
        return 1;
    }
    benchmark_00();
    return 0;
}
