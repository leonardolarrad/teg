#include "teg/teg.h"
#include "benchmark/benchmark.h"
#define ANKERL_NANOBENCH_IMPLEMENT
#include "benchmark/nanobench.h"
#include <atomic>
#include <thread>

namespace bm = benchmarking;

int main() {
    uint64_t x = 1;
    ankerl::nanobench::Bench()
    .minEpochIterations(5638667)
    .run("sleep", [&]() {
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ankerl::nanobench::doNotOptimizeAway(x += 1);
    });

    bm::benchmark()
        .warmup(100)
        .iterations(5638667)
        .run("sleep", [&]() {
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
            //ankerl::nanobench::doNotOptimizeAway(x += 1);
            bm::dont_optimize_away(x += 1);
        });
}