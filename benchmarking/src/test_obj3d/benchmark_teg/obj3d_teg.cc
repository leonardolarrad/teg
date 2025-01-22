#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

#define PRINT_BUFFER_SIZE  0
#define TEST_LIB           0

#if PRINT_BUFFER_SIZE
#include <iostream>

class static_print_buffer {
public:
    static_print_buffer() {
        const auto data_out = benchmarking::test_obj3d::generate_benchmark_data();
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
        const auto data_out = benchmarking::test_obj3d::generate_benchmark_data();
        teg::byte_array buffer;
        teg::serialize(buffer, data_out).or_throw();
        benchmarking::test_obj3d::obj_3d data_in;
        teg::deserialize(buffer, data_in).or_throw();

        if (data_out != data_in) {
            throw std::runtime_error("data_out != data_in");
        }
    }
} static_test_lib;
#endif

static void bm_serialization(benchmark::State& state) {
    const auto data_out = benchmarking::test_obj3d::generate_benchmark_data();

    for (auto _ : state) {
        teg::byte_array buffer_out;
        teg::serialize(buffer_out, data_out).or_throw();
    }
}

static void bm_deserialization(benchmark::State& state) {    
    auto buffer_in = []() -> teg::byte_array {
        const auto data_out = benchmarking::test_obj3d::generate_benchmark_data();

        teg::byte_array buffer;
        teg::serialize(buffer, data_out).or_throw();
        return buffer;
    }();

    for (auto _ : state) {
        benchmarking::test_obj3d::obj_3d data_in;
        teg::deserialize(buffer_in, data_in).or_throw();
    }
}

BENCHMARK(bm_serialization)->Repetitions(50);
BENCHMARK(bm_deserialization)->Repetitions(50);
BENCHMARK_MAIN();