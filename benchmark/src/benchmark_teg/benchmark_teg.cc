#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

namespace bm = benchmarking;

static void benchmark_serialization() {    
    std::vector<bm::ecommerce_page> test_data_1024b = bm::generate_benchmark_data(8, 1024);
    
    teg::buffer b;
    bm::benchmark()
        .warmup(10)
        .iterations(100000)
        .repetitions(100)
        .run("teg serialization:buffer_raii", [&test_data_1024b](){
            teg::buffer b;
            teg::serialize(b, test_data_1024b).or_throw();
        })
        .run("teg serialization:buffer_clear", [&](){
            b.clear();
            teg::serialize(b, test_data_1024b).or_throw();
        });
}

int main() {
    benchmark_serialization();
    return 0;    
}