#include <sstream>

#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

// Boost.Serialization
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/serialization/vector.hpp"

#define BM_PROFILE_MEMORY 0

#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

namespace test = benchmarking::test_obj3d;
namespace bm = benchmarking;

namespace boost {
namespace serialization {

template <class Archive>
BOOST_FORCEINLINE void serialize(Archive& ar, bm::test_obj3d::ivec3& v, const unsigned int) {
    ar & v.x & v.y & v.z;
}

template <class Archive>
BOOST_FORCEINLINE void serialize(Archive& ar, bm::test_obj3d::fvec3& v, const unsigned int) {
    ar & v.x & v.y & v.z;
}

template <class Archive>
BOOST_FORCEINLINE void serialize(Archive& ar, bm::test_obj3d::face& f, const unsigned int) {
    ar & f.vertex_index & f.normal_index;
}

template <class Archive>
BOOST_FORCEINLINE void serialize(Archive& ar, bm::test_obj3d::obj_3d& o, const unsigned int) {
    ar & o.vertices & o.normals & o.faces;
}

} // namespace serialization
} // namespace boost

static void bm_serialization(benchmark::State& state) {      
    auto const data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);

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
        auto const data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);

        std::ostringstream buffer_out;
        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;

        return buffer_out;
        }();

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    test::obj_3d data_in;
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