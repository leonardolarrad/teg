#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h"

#define BM_PROFILE_MEMORY 0

#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

namespace test = benchmarking::test_obj3d;

#if BM_TEST_LIB
#include <exception>

class static_test_lib {
public:
    static_test_lib() {
        teg::byte_array buffer;
        const auto data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);
        teg::serialize<teg::native_mode>(buffer, data_out).or_throw();
        test::obj_3d data_in;
        teg::deserialize<teg::native_mode>(buffer, data_in).or_throw();

        if (data_out != data_in) {
            throw std::runtime_error("data_out != data_in");
        }
    }
} static_test_lib;
#endif

static void bm_serialization(benchmark::State& state) {
    const auto data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);
    teg::byte_array buffer_out;

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    for (auto _ : state) {
        teg::serialize<teg::native_mode>(buffer_out, data_out).or_throw();

        #if BM_PROFILE_MEMORY
        state.PauseTiming();    
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    state.counters["Buffer size (B)"] = benchmark::Counter(
        buffer_out.size(), 
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);
    state.counters["B/s"] = benchmark::Counter(
        int64_t(state.iterations()) * int64_t(state.range(0) * 1024 * 1024),
        benchmark::Counter::kIsRate, 
        benchmark::Counter::kIs1024);

    #if BM_PROFILE_MEMORY
    state.counters["Memory usage (B)"] = benchmark::Counter(
        memory_usage, 
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);
    #endif 
}

static void bm_deserialization(benchmark::State& state) {    
    auto buffer_in = [&]() -> teg::byte_array {
        teg::byte_array buffer;
        const auto data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);
        teg::serialize<teg::native_mode>(buffer, data_out).or_throw();
        return buffer;
    }();

    #if BM_PROFILE_MEMORY
        uint64_t memory_usage = 0;
    #endif

    test::obj_3d data_in;
    for (auto _ : state) {
        teg::deserialize<teg::native_mode>(buffer_in, data_in).or_throw();

        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    state.counters["B/s"] = benchmark::Counter(
        int64_t(state.iterations()) * int64_t(state.range(0) * 1024 * 1024),
        benchmark::Counter::kIsRate, 
        benchmark::Counter::kIs1024);

    #if BM_PROFILE_MEMORY
    state.counters["Memory usage (B)"] = benchmark::Counter(
        memory_usage, 
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);
    #endif 
}

BENCHMARK(bm_serialization)
    ->RangeMultiplier(2)->Range(1, 1024)
    ->MinWarmUpTime(1)
    ->Repetitions(30)
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK(bm_deserialization)
    ->RangeMultiplier(2)->Range(1, 1024)
    ->MinWarmUpTime(1)
    ->Repetitions(30)
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK_MAIN();