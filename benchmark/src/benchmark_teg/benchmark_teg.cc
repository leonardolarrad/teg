#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

namespace bm = benchmarking;

static std::vector<bm::ecommerce_page> data_in_10_2048b = bm::generate_benchmark_data(10, 2048);

static bool test_lib() {
    teg::byte_buffer b;
    bm::ecommerce_page t0 = data_in_10_2048b[0];
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

static void benchmark() {
    teg::byte_buffer buffer;
    std::vector<bm::ecommerce_page> data_out_10_2048bb;

    bm::benchmark()
        .warmup(0)
        .iterations(746667)
        .repetitions(10)
        .run("teg:serialization:10_2048b", [&](){
            buffer.clear();
            teg::serialize(buffer, data_in_10_2048b).or_throw();
        })
        //.run("teg:deserialization:10_1024b", [&](){
        //    teg::deserialize(buffer, data_out_10_1024b).or_throw();
        //})
        ;
}

int main() {
    if (!test_lib()) {
        return 1;
    }
    benchmark();
}