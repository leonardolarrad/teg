#include <span>
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "flatbuffers/flatbuffers.h"
#include "obj3d_generated.h"

#define BM_PROFILE_MEMORY 0
#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

namespace test = benchmarking::test_obj3d;

static inline auto serialize_obj3d(flatbuffers::FlatBufferBuilder& fbb, test::obj_3d const& data_out) -> std::span<uint8_t> {
    std::vector<flatbuf::obj3d::FVec3> vertices;
    std::vector<flatbuf::obj3d::FVec3> normals;
    std::vector<flatbuf::obj3d::Face> faces;

    vertices.reserve(data_out.vertices.size());
    normals.reserve(data_out.normals.size());
    faces.reserve(data_out.faces.size());

    for (const auto& v : data_out.vertices) {
        vertices.emplace_back(flatbuf::obj3d::FVec3(v.x, v.y, v.z));
    }

    for (const auto& n : data_out.normals) {
        normals.emplace_back(flatbuf::obj3d::FVec3(n.x, n.y, n.z));
    }

    for (const auto& f : data_out.faces) {
        faces.emplace_back(flatbuf::obj3d::Face(
            flatbuf::obj3d::IVec3(f.vertex_index.x, f.vertex_index.y, f.vertex_index.z), 
            flatbuf::obj3d::IVec3(f.normal_index.x, f.normal_index.y, f.normal_index.z)));
    }

    auto obj3d = flatbuf::obj3d::CreateObj3DDirect(fbb, &vertices, &normals, &faces);
    fbb.Finish(obj3d);

    return std::span{fbb.GetBufferPointer(), fbb.GetSize()};
}

static void bm_serialization(benchmark::State& state) {
    const auto data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    for (auto _ : state) {
        flatbuffers::FlatBufferBuilder fbb;
        serialize_obj3d(fbb, data_out);

        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    flatbuffers::FlatBufferBuilder fbb;
    state.counters["Bytes/s"] = benchmark::Counter(
        int64_t(state.iterations()) * int64_t(state.range(0) * 1024 * 1024),
        benchmark::Counter::kIsRate,
        benchmark::Counter::kIs1024);
    state.counters["Buffer size"] = benchmark::Counter(
        serialize_obj3d(fbb, data_out).size(),
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);
    #if BM_PROFILE_MEMORY
    state.counters["Memory usage (B)"] = benchmark::Counter(
        memory_usage, 
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);
    #endif
}

static void bm_deserialization(benchmark::State& state) {
    const auto data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);
    flatbuffers::FlatBufferBuilder fbb;
    const auto buffer_in = serialize_obj3d(fbb, data_out);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    for (auto _ : state) {
        auto const fb_obj3d = flatbuf::obj3d::GetObj3D(buffer_in.data());
        
        for (const auto& v : *fb_obj3d->vertices()) {
            benchmark::DoNotOptimize(v);
        }

        for (const auto& n : *fb_obj3d->normals()) {
            benchmark::DoNotOptimize(n);
        }

        for (const auto& f : *fb_obj3d->faces()) {
            benchmark::DoNotOptimize(f);
        }

        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    state.counters["Bytes/s"] = benchmark::Counter(
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