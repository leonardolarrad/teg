#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "msgpack.hpp" // MessagePack

#define BM_PROFILE_MEMORY 0
#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

namespace test = benchmarking::test_obj3d;

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {
    
template <>
struct pack<test::ivec3> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const test::ivec3& v) const {
        o.pack_array(3);
        o.pack(v.x);
        o.pack(v.y);
        o.pack(v.z);
        return o;
    }
};

template <>
struct convert<test::ivec3> {
    msgpack::object const& operator()(msgpack::object const& o, test::ivec3& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 3) { throw msgpack::type_error(); }
        v.x = o.via.array.ptr[0].as<int64_t>();
        v.y = o.via.array.ptr[1].as<int64_t>();
        v.z = o.via.array.ptr[2].as<int64_t>();
        return o;
    }
};

template <>
struct pack<test::fvec3> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const test::fvec3& v) const {
        o.pack_array(3);
        o.pack(v.x);
        o.pack(v.y);
        o.pack(v.z);
        return o;
    }
};

template <>
struct convert<test::fvec3> {
    msgpack::object const& operator()(msgpack::object const& o, test::fvec3& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 3) { throw msgpack::type_error(); }
        v.x = o.via.array.ptr[0].as<double>();
        v.y = o.via.array.ptr[1].as<double>();
        v.z = o.via.array.ptr[2].as<double>();
        return o;
    }
};

template <>
struct pack<test::face> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const test::face& v) const {
        o.pack_array(2);
        o.pack(v.vertex_index);
        o.pack(v.normal_index);
        return o;
    }
};

template <>
struct convert<test::face> {
    msgpack::object const& operator()(msgpack::object const& o, test::face& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 2) { throw msgpack::type_error(); }
        v.vertex_index = o.via.array.ptr[0].as<test::ivec3>();
        v.normal_index = o.via.array.ptr[1].as<test::ivec3>();
        return o;
    }
};

template <>
struct pack<test::obj_3d> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const test::obj_3d& v) const {
        o.pack_array(3);
        o.pack(v.vertices);
        o.pack(v.normals);
        o.pack(v.faces);
        return o;
    }
};

template <>
struct convert<test::obj_3d> {
    msgpack::object const& operator()(msgpack::object const& o, test::obj_3d& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 3) { throw msgpack::type_error(); }
        v.vertices = o.via.array.ptr[0].as<std::vector<test::vertex>>();
        v.normals = o.via.array.ptr[1].as<std::vector<test::normal>>();
        v.faces = o.via.array.ptr[2].as<std::vector<test::face>>();
        return o;
    }
};

} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE
} // namespace msgpack

static void bm_serialization(benchmark::State& state) {  
    auto const data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    msgpack::sbuffer buffer_out;
    for (auto _ : state) {
        buffer_out.clear();
        msgpack::pack(buffer_out, data_out);

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
    auto buffer_in = [&]() -> msgpack::sbuffer {
        auto const data_out = test::generate_benchmark_data(state.range(0) * 1024 * 1024);
        msgpack::sbuffer buffer;
        msgpack::pack(buffer, data_out);
        return buffer;
    }();
    
    
    #if BM_PROFILE_MEMORY
        uint64_t memory_usage = 0;
    #endif

    test::obj_3d data_in;
    for (auto _ : state) {
        msgpack::object_handle obj_handle = msgpack::unpack(buffer_in.data(), buffer_in.size());
        msgpack::object obj = obj_handle.get();
        //obj.convert(data_in);

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