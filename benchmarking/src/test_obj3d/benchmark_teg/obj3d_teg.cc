#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

static void bm_serialization(benchmark::State& state) {
    auto data_out = benchmarking::test_obj3d::generate_benchmark_data();
    teg::byte_array buffer_out;

    for (auto _ : state) {
        teg::serialize(buffer_out, data_out).or_throw();
    }
}

static void bm_deserialization(benchmark::State& state) {    
    auto buffer_in = []() -> teg::byte_array {
        teg::byte_array buffer;
        teg::serialize(buffer, benchmarking::test_obj3d::generate_benchmark_data()).or_throw();
        return buffer;
    }();
    benchmarking::test_obj3d::obj_3d data_in;

    for (auto _ : state) {
        teg::deserialize(buffer_in, data_in).or_throw();
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();