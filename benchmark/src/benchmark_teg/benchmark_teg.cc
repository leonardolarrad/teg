#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"

#include <atomic>
#include <thread>



namespace bm = benchmarking;

static void xplusplus() {
    uint64_t x = 1;
    bm::benchmark()
        .warmup(100)
        .iterations(896000000)
        .repetitions(1)
        .run("sleep", [&]() {
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            //ankerl::nanobench::doNotOptimizeAway(x += 1);
            bm::dont_optimize_away(x += 1);
        });
}

int main() {
    std::seed_seq seed { 99, 5, 11 };
    std::mt19937 rng { seed };

    std::string str = bm::random_string(rng, 2048);
    auto user = bm::random_user_data(rng, 2048);
    std::cout<< teg::buffer_size(str) << std::endl;

    std::cout << str << std::endl;
    
    xplusplus();
    return 0;    
}