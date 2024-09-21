#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

// lib
#include "zpp_bits.h"

namespace bm = benchmarking;
static std::vector<bm::ecommerce_page> data_in_10_1024b = bm::generate_benchmark_data(10, 1024);

static bool test_lib() {
    try {
        auto [data, in, out] = zpp::bits::data_in_out();
        auto& t0 = data_in_10_1024b[0];
        out(t0).or_throw();

        bm::ecommerce_page t1;
        in(t1).or_throw();

        return t0 == t1;
    } catch (const std::exception& e) {
        return false;
    }
}

static void benchmark() {
    auto [data, in, out] = zpp::bits::data_in_out();

    bm::benchmark()
        .warmup(128)
        .iterations(302400)
        .repetitions(10)
        .run("zppbits:serialization:10_1024b", [&](){
            out.reset();
            out(data_in_10_1024b).or_throw();
        });        
}

int main() {
    if (!test_lib()) {
        return 1;
    }
    benchmark();
    return 0;    
}