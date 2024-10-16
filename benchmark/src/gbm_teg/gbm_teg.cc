#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

namespace bm = benchmarking;

#include "C:\Users\Warr\Development\teg\benchmark\deps\benchmark-1.8.4\include\benchmark\benchmark.h"

static std::vector<bm::ecommerce_page> data_in_10_2048b = bm::generate_benchmark_data(10, 2048);

static void bm_serialization(benchmark::State& state) {
    teg::byte_buffer buffer;

    for (auto _ : state) {
        buffer.clear();
        teg::serialize(buffer, data_in_10_2048b).or_throw();
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK_MAIN();