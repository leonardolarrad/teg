#include "teg/teg.h"
#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h" // Google Benchmark

#include "zpp_bits.h"

#define PRINT_BUFFER_SIZE  0

static std::vector<benchmarking::test_ecommerce::ecommerce_page> data_out =
    benchmarking::test_ecommerce::generate_benchmark_data(512, 2048); // 2 MiB

#if PRINT_BUFFER_SIZE
#include <iostream>

class static_print_buffer {
public:
    static_print_buffer() {
        teg::byte_array buffer;
        teg::serialize(buffer, data_out).or_throw();
        std::cout << "teg:buffer size: " << buffer.size() << std::endl;
    }
} static_print_buffer;

#endif

static void bm_serialization(benchmark::State& state) {
    for (auto _ : state) {
        auto [buffer_out, out] = zpp::bits::data_out();
        out(data_out).or_throw();
    }
}

static void bm_deserialization(benchmark::State& state) {    
    auto buffer_in = []() -> std::vector<std::byte> {
        auto [buffer, out] = zpp::bits::data_out();
        out(data_out).or_throw();
        return buffer;
    }();

    for (auto _ : state) {
        zpp::bits::in in(buffer_in);
        std::vector<benchmarking::test_ecommerce::ecommerce_page> data_in;
        in(data_in).or_throw();
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();