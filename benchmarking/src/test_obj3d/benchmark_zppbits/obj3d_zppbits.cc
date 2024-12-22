#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "zpp_bits.h" // ZppBits

static auto data_out = benchmarking::test_obj3d::generate_benchmark_data();
static auto buffer_in = []() -> std::vector<std::byte> {
        auto [buffer, out] = zpp::bits::data_out();
        out(data_out).or_throw();
        return buffer;
    }();

static void bm_serialization(benchmark::State& state) {
    
    
    for (auto _ : state) {
        state.PauseTiming();        
        auto [buffer_out, out] = zpp::bits::data_out();
        state.ResumeTiming();
        out(buffer_out, data_out).or_throw();
    }
}

static void bm_deserialization(benchmark::State& state) {
    
    benchmarking::test_obj3d::obj_3d data_in;

    for (auto _ : state) {
        auto in = zpp::bits::in(buffer_in);
        in(buffer_in, data_in).or_throw();
    }
}

BENCHMARK(bm_serialization)->MinWarmUpTime(10.0)->Repetitions(10);
//BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();