#include "teg/teg.h"
#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h" // Google Benchmark

#define PRINT_BUFFER_SIZE  0
#define TEST_LIB           0

static std::vector<benchmarking::test_ecommerce::ecommerce_page> data_out =
    benchmarking::test_ecommerce::generate_benchmark_data(512, 2048); // 1 MiB

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

#if TEST_LIB
#include <exception>

class static_test_lib {
public:
    static_test_lib() {
        teg::byte_array buffer;
        teg::serialize(buffer, data_out).or_throw();
        std::vector<benchmarking::test_ecommerce::ecommerce_page> data_in;
        teg::deserialize(buffer, data_in).or_throw();

        if (data_out != data_in) {
            throw std::runtime_error("data_out != data_in");
        }
    }
} static_test_lib;
#endif

static void bm_serialization(benchmark::State& state) {
    for (auto _ : state) {
        teg::byte_array buffer_out;
        teg::serialize(buffer_out, data_out).or_throw();
    }
}

static void bm_deserialization(benchmark::State& state) {    
    auto buffer_in = []() -> teg::byte_array {
        teg::byte_array buffer;
        teg::serialize(buffer, data_out).or_throw();
        return buffer;
    }();

    for (auto _ : state) {
        std::vector<benchmarking::test_ecommerce::ecommerce_page> data_in;
        teg::deserialize(buffer_in, data_in).or_throw();
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();