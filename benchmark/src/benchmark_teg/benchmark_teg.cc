#include "teg/md5.h"
#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

namespace bm = benchmarking;

static std::vector<bm::ecommerce_page> data_out_1mib = bm::generate_benchmark_data(512, 1024*2);

static bool test_lib() {
    teg::byte_buffer b;
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

static void run_benchmark_00() {   
    std::vector<bm::ecommerce_page> data_in_1mib;
    teg::byte_buffer buffer{};
    teg::byte_buffer buffer2{};
    std::string schema_in{};

    constexpr auto schema = teg::schema<decltype(teg::schema<bm::ecommerce_page>())>();
    std::cout << "Schema size: " << schema.size() << std::endl;
    std::cout << "Schema: " << schema.c_str() << std::endl;
    bm::benchmark()
        .warmup(10)
        .iterations(10)
        .repetitions(10)
        .run("teg:serialization:1mib", [&](){
            buffer.clear();
            buffer2.clear();
            teg::serialize(buffer, std::string{schema.c_str(), schema.size()}, data_out_1mib).or_throw();
            teg::serialize(buffer2, teg::xxhash::hash64(std::span<const std::byte>{buffer})).or_throw();
        })
        ;//.run("teg:deserialization:1mib", [&](){
        //    teg::deserialize(buffer, schema_in, data_in_1mib).or_throw();
        //});

    std::cout << "\nBuffer size: " << buffer.size() << std::endl;
    std::cout << "\nBuffer2 size: " << buffer2.size() << std::endl;
}

static void run_benchmark_01() {
    int64_t d0 = 99999999999999ull;
    int64_t d1;
    teg::byte_buffer buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(1024 * 1024 * 1024)
        .repetitions(10)
        .run("teg:serialization:i64", [&](){
            buffer.clear();
            teg::serialize(buffer, d0).or_throw();
        })
        .run("teg:deserialization:i64", [&](){
            teg::deserialize(buffer, d1).or_throw();
        });
}

static void run_benchmark_02() {
    std::string d0 = "Hello World!";
    std::string d1;
    teg::byte_buffer buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(1024 * 1024 * 100)
        .repetitions(10)
        .run("teg:serialization:simple_string", [&](){
            buffer.clear();
            teg::serialize(buffer, d0).or_throw();
        })
        .run("teg:deserialization:simple_string", [&](){
            teg::deserialize(buffer, d1).or_throw();
        });
}

static void run_benchmark_03() {
    teg::byte_buffer buffer{};
    teg::byte_buffer buffer2{};
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
            buffer.clear();
            //buffer2.clear();
            teg::serialize(buffer, data).or_throw();
            //teg::serialize(buffer2, teg::md5_hash_u64(std::span<const std::byte>{buffer})).or_throw();
        });
}

static void run_benchmark_04() {
    teg::byte_buffer buffer{};
    teg::byte_buffer buffer2{};
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
    run_benchmark_00();
    return 0;
}
