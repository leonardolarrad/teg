#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark
#include "obj3d.pb.h"

#define BM_PROFILE_MEMORY 0
#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

namespace test = benchmarking::test_obj3d;
namespace proto = protobuf::obj3d;

void inline convert_to_pb(test::ivec3 const& src, proto::IVec3* dest) {
    dest->set_x(src.x);
    dest->set_y(src.y);
    dest->set_z(src.z);
}

void inline convert_to_pb(test::fvec3 const& src, proto::FVec3* dest) {
    dest->set_x(src.x);
    dest->set_y(src.y);
    dest->set_z(src.z);
}

void inline convert_to_pb(test::face const& src, proto::Face* dest) {
    convert_to_pb(src.vertex_index, dest->mutable_vertice_index());
    convert_to_pb(src.normal_index, dest->mutable_normal_index());
}

void inline convert_to_pb(test::obj_3d const& src, proto::Obj3D* dest) {
    for (auto const& v : src.vertices) {
        convert_to_pb(v, dest->add_vertices());
    }
    for (auto const& n : src.normals) {
        convert_to_pb(n, dest->add_normals());
    }
    for (auto const& f : src.faces) {
        convert_to_pb(f, dest->add_faces());
    }
}

void inline convert_from_pb(proto::IVec3 const& src, test::ivec3& dest) {
    dest.x = src.x();
    dest.y = src.y();
    dest.z = src.z();
}

void inline convert_from_pb(proto::FVec3 const& src, test::fvec3& dest) {
    dest.x = src.x();
    dest.y = src.y();
    dest.z = src.z();
}

void inline convert_from_pb(proto::Face const& src, test::face& dest) {
    convert_from_pb(src.vertice_index(), dest.vertex_index);
    convert_from_pb(src.normal_index(), dest.normal_index);
}

void inline convert_from_pb(proto::Obj3D const& src, test::obj_3d& dest) {
    for (auto const& v : src.vertices()) {        
        convert_from_pb(v, dest.vertices.emplace_back());
    }
    for (auto const& n : src.normals()) {        
        convert_from_pb(n, dest.normals.emplace_back());
    }
    for (auto const& f : src.faces()) {        
        convert_from_pb(f, dest.faces.emplace_back());
    }        
}

static void bm_serialization(benchmark::State& state) {
    auto const test_obj = test::generate_benchmark_data(state.range(0) * 1024 * 1024);

    proto::Obj3D test_pb;
    convert_to_pb(test_obj, &test_pb);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    std::string buffer_out;
    for (auto _ : state) {
        test_pb.SerializeToString(&buffer_out);

        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    state.counters["Buffer size (B)"] = benchmark::Counter(
        buffer_out.size(), 
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
    const auto buffer_in = [&]() -> std::string {
        auto const test_obj = test::generate_benchmark_data(state.range(0) * 1024 * 1024);

        proto::Obj3D pb_data_out;
        convert_to_pb(test_obj, &pb_data_out);
        
        std::string buffer_out;
        pb_data_out.SerializeToString(&buffer_out);
        return buffer_out;
    }();

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    proto::Obj3D pb_data_in;
    for (auto _ : state) {
        pb_data_in.ParseFromString(buffer_in);

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