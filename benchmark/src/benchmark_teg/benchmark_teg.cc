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

    bm::benchmark()
        .warmup(10)
        .iterations(1024)
        .repetitions(10)
        .run("teg:serialization:1mib", [&](){
            buffer.clear();
            teg::serialize(buffer, data_out_1mib).or_throw();
        })
        .run("teg:deserialization:1mib", [&](){
            teg::deserialize(buffer, data_in_1mib).or_throw();
        });

    std::cout << "\nBuffer size: " << buffer.size() << std::endl;
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

int main() {
    if (!test_lib()) {
        return 1;
    }
    //benchmark_lib();
    run_benchmark_00();
    return 0;
}