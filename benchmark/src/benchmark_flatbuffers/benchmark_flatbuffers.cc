#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

namespace bm = benchmarking;    

static std::vector<bm::ecommerce_page> data_out_1mib = bm::generate_benchmark_data(10, 2048);

static bool test_lib() {
    return true;
} 

static void benchmark() {

}

int main() {
    if (!test_lib()) {
        return 1;
    }
    benchmark();
    return 0;    
}
