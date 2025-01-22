#include <sstream>
#include "teg/teg.h"
#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h" // Google benchmark

#define BM_PROFILE_MEMORY 0

#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

// Boost.Serialization
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/serialization/vector.hpp"

namespace test = benchmarking::test_ecommerce;

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, test::ecommerce_user& v, const unsigned int) {
    ar & v.uuid;
    ar & v.name;
    ar & v.email;
    ar & v.recent_searches;
}

template <class Archive>
void serialize(Archive& ar, test::ecommerce_product& v, const unsigned int) {
    ar & v.uuid;
    ar & v.name;
    ar & v.description;
    ar & v.category;
    ar & v.tags;
    ar & v.image_lo_res_url;
    ar & v.image_hi_res_url;
    ar & v.price;
    ar & v.discount;
    ar & v.stock;
    ar & v.rating;
    ar & v.reviews;
}

template <class Archive>
void serialize(Archive& ar, test::ecommerce_page& v, const unsigned int) {
    ar & v.user;
    ar & v.permanent_url;
    ar & v.query;
    ar & v.page;
    ar & v.total_pages;
    ar & v.results_per_page;
    ar & v.total_results;
    ar & v.products;
}

} // namespace serialization
} // namespace boost

static void bm_serialization(benchmark::State& state) {      
    auto const data_out = test::generate_benchmark_data(1024, state.range(0) * 1024);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    std::ostringstream buffer_out;
    for (auto _ : state) {
        state.PauseTiming();
        buffer_out.str("");
        buffer_out.clear();
        state.ResumeTiming();

        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;

        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    state.counters["Buffer size (B)"] = benchmark::Counter(
        buffer_out.str().size(),
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
    const auto buffer_out = [&]() -> std::ostringstream {
        auto const data_out = test::generate_benchmark_data(1024, state.range(0) * 1024);

        std::ostringstream buffer_out;
        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;

        return buffer_out;
        }();

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    std::vector<test::ecommerce_page> data_in;
    for (auto _ : state) {
        std::istringstream buffer_in(buffer_out.str());
        boost::archive::binary_iarchive archive_in(buffer_in);
        archive_in >> data_in;

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