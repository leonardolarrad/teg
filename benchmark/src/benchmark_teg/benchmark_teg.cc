#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

namespace bm = benchmarking;

static std::vector<bm::ecommerce_page> data_out_1mib = bm::generate_benchmark_data(512, 2048);

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

static void benchmark_lib() {   
    std::vector<bm::ecommerce_page> data_in_1mib;
    teg::byte_buffer buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(512)
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

int main() {
    if (!test_lib()) {
        return 1;
    }
    benchmark_lib();
    return 0;
}