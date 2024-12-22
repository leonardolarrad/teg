#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

static void bm_serialization(benchmark::State& state) {
    auto data_out = benchmarking::test_obj3d::generate_benchmark_data();
    teg::byte_array buffer_out;

    for (auto _ : state) {
        teg::serialize<teg::compact_mode>(buffer_out, data_out).or_throw();
    }

    std::cout << buffer_out.size() << std::endl;
}

static void bm_deserialization(benchmark::State& state) {    
    auto buffer_in = []() -> teg::byte_array {
        teg::byte_array buffer;
        teg::serialize<teg::compact_mode>(buffer, benchmarking::test_obj3d::generate_benchmark_data()).or_throw();
        return buffer;
    }();
    benchmarking::test_obj3d::obj_3d data_in;

    for (auto _ : state) {
        teg::deserialize<teg::compact_mode>(buffer_in, data_in).or_throw();
    }
}

BENCHMARK(bm_serialization)->MinWarmUpTime(10)->Repetitions(10);
BENCHMARK(bm_deserialization)->MinWarmUpTime(10)->Repetitions(10);
BENCHMARK_MAIN();