#include "teg/teg.h"
#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h"

namespace test = benchmarking::test_ecommerce;

static std::vector<test::ecommerce_page> data_out =
#if defined(BM_DATASIZE_1KIB)
    test::generate_benchmark_data(1, 1024);           // 1 KiB
#elif defined(BM_DATASIZE_1MIB)
    test::generate_benchmark_data(512, 2048);         // 1 MiB
#elif defined(BM_DATASIZE_1GIB)
    test::generate_benchmark_data(1024, 1024 * 1024); // 1 GiB
#elif defined(BM_DATASIZE_CUSTOM)
    test::generate_benchmark_data(BM_DATASIZE_CUSTOM/2, BM_DATASIZE_CUSTOM * 2); // Custom
#else
    test::generate_benchmark_data(1, 1024);           // 1 KiB (default)
#endif

#if defined(BM_LOG_BUFFER_SIZE)
#include <iostream>

class static_print_buffer {
public:
    static_print_buffer() {
        teg::byte_array buffer;
        teg::serialize<teg::native_mode>(buffer, data_out).or_throw();
        std::cout << "teg_native:buffer size: " << buffer.size() << std::endl;
    }
} static_print_buffer;

#endif

#if BM_TEST_LIB
#include <exception>

class static_test_lib {
public:
    static_test_lib() {
        teg::byte_array buffer;
        teg::serialize<teg::native_mode>(buffer, data_out).or_throw();
        std::vector<benchmarking::test_ecommerce::ecommerce_page> data_in;
        teg::deserialize<teg::native_mode>(buffer, data_in).or_throw();

        if (data_out != data_in) {
            throw std::runtime_error("data_out != data_in");
        }
    }
} static_test_lib;
#endif

static void bm_serialization(benchmark::State& state) {
    for (auto _ : state) {
        teg::byte_array buffer_out;
        teg::serialize<teg::native_mode>(buffer_out, data_out).or_throw();
    }
}

static void bm_deserialization(benchmark::State& state) {    
    auto buffer_in = []() -> teg::byte_array {        
        teg::byte_array buffer_out;
        teg::serialize<teg::native_mode>(buffer_out, data_out).or_throw();
        return buffer_out;
    }();

    for (auto _ : state) {
        std::vector<benchmarking::test_ecommerce::ecommerce_page> data_in;
        teg::deserialize<teg::native_mode>(buffer_in, data_in).or_throw();
    }
}

#if defined(BM_ONLY_SERIALIZATION)
BENCHMARK(bm_serialization)->Repetitions(15);
#elif defined(BM_ONLY_DESERIALIZATION)
BENCHMARK(bm_deserialization)->Repetitions(15);
#else
BENCHMARK(bm_serialization)->Repetitions(50);
BENCHMARK(bm_deserialization)->Repetitions(50);
#endif
BENCHMARK_MAIN();