#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "msgpack.hpp" // MessagePack

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {
    
template <>
struct pack<benchmarking::test_obj3d::ivec3> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const benchmarking::test_obj3d::ivec3& v) const {
        o.pack_array(3);
        o.pack(v.x);
        o.pack(v.y);
        o.pack(v.z);
        return o;
    }
};

template <>
struct convert<benchmarking::test_obj3d::ivec3> {
    msgpack::object const& operator()(msgpack::object const& o, benchmarking::test_obj3d::ivec3& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 3) { throw msgpack::type_error(); }
        v.x = o.via.array.ptr[0].as<int64_t>();
        v.y = o.via.array.ptr[1].as<int64_t>();
        v.z = o.via.array.ptr[2].as<int64_t>();
        return o;
    }
};

template <>
struct pack<benchmarking::test_obj3d::fvec3> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const benchmarking::test_obj3d::fvec3& v) const {
        o.pack_array(3);
        o.pack(v.x);
        o.pack(v.y);
        o.pack(v.z);
        return o;
    }
};

template <>
struct convert<benchmarking::test_obj3d::fvec3> {
    msgpack::object const& operator()(msgpack::object const& o, benchmarking::test_obj3d::fvec3& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 3) { throw msgpack::type_error(); }
        v.x = o.via.array.ptr[0].as<double>();
        v.y = o.via.array.ptr[1].as<double>();
        v.z = o.via.array.ptr[2].as<double>();
        return o;
    }
};

template <>
struct pack<benchmarking::test_obj3d::face> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const benchmarking::test_obj3d::face& v) const {
        o.pack_array(2);
        o.pack(v.vertex_index);
        o.pack(v.normal_index);
        return o;
    }
};

template <>
struct convert<benchmarking::test_obj3d::face> {
    msgpack::object const& operator()(msgpack::object const& o, benchmarking::test_obj3d::face& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 2) { throw msgpack::type_error(); }
        v.vertex_index = o.via.array.ptr[0].as<benchmarking::test_obj3d::ivec3>();
        v.normal_index = o.via.array.ptr[1].as<benchmarking::test_obj3d::ivec3>();
        return o;
    }
};

template <>
struct pack<benchmarking::test_obj3d::obj_3d> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const benchmarking::test_obj3d::obj_3d& v) const {
        o.pack_array(3);
        o.pack(v.vertices);
        o.pack(v.normals);
        o.pack(v.faces);
        return o;
    }
};

template <>
struct convert<benchmarking::test_obj3d::obj_3d> {
    msgpack::object const& operator()(msgpack::object const& o, benchmarking::test_obj3d::obj_3d& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 3) { throw msgpack::type_error(); }
        v.vertices = o.via.array.ptr[0].as<std::vector<benchmarking::test_obj3d::vertex>>();
        v.normals = o.via.array.ptr[1].as<std::vector<benchmarking::test_obj3d::normal>>();
        v.faces = o.via.array.ptr[2].as<std::vector<benchmarking::test_obj3d::face>>();
        return o;
    }
};

} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE
} // namespace msgpack

static void bm_serialization(benchmark::State& state) {
    auto data_out = benchmarking::test_obj3d::generate_benchmark_data();
    msgpack::sbuffer buffer_out;
    
    for (auto _ : state) {
        state.PauseTiming();
        buffer_out.clear();
        state.ResumeTiming();
        msgpack::pack(buffer_out, data_out);
    }
}

static void bm_deserialization(benchmark::State& state) {
    auto buffer_in = []() -> msgpack::sbuffer {
        msgpack::sbuffer buffer;
        msgpack::pack(buffer, benchmarking::test_obj3d::generate_benchmark_data());
        return buffer;
    }();
    benchmarking::test_obj3d::obj_3d data_in;
    
    for (auto _ : state) {
        msgpack::object_handle obj_handle = msgpack::unpack(buffer_in.data(), buffer_in.size());
        msgpack::object obj = obj_handle.get();
        obj.convert(data_in);
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();