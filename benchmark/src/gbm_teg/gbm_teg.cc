#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

namespace bm = benchmarking;

#include "C:\Users\Warr\Development\teg\benchmark\deps\benchmark-1.8.4\include\benchmark\benchmark.h"

static std::vector<bm::ecommerce_page> data_1mib = bm::generate_benchmark_data(512, 2048);

static void bm_serialization(benchmark::State& state) {
    teg::byte_buffer buffer;

    for (auto _ : state) {
        buffer.clear();
        teg::serialize(buffer, data_1mib).or_throw();
    }
}

static void bm_deserialization(benchmark::State& state) {
    teg::byte_buffer buffer;
    teg::serialize(buffer, data_1mib).or_throw();
    std::vector<bm::ecommerce_page> data_out_1mib;

    for (auto _ : state) {
        teg::deserialize(buffer, data_out_1mib).or_throw();
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();