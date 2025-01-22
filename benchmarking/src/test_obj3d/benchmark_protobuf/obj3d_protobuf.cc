#define PRINT_BUFFER_SIZE 0

#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "obj3d.pb.h"

static auto const data_out = benchmarking::test_obj3d::generate_benchmark_data();

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

static const auto test_obj = test::generate_benchmark_data();

#if PRINT_BUFFER_SIZE
class static_print_size {
public:
    static_print_size() {
        proto::Obj3D test_pb;
        convert_to_pb(test_obj, &test_pb);

        std::string buffer_out;
        test_pb.SerializeToString(&buffer_out);

        std::cout << buffer_out.size() << std::endl;
    }
} static_print_size;
#endif

static void bm_serialization(benchmark::State& state) {

    for (auto _ : state) {
        proto::Obj3D test_pb;
        convert_to_pb(test_obj, &test_pb);

        std::string buffer_out;
        test_pb.SerializeToString(&buffer_out);
    }
}

static void bm_deserialization(benchmark::State& state) {    
    const auto buffer_in = []() -> std::string {
        proto::Obj3D pb_data_out;
        convert_to_pb(test_obj, &pb_data_out);
        
        std::string buffer_out;
        pb_data_out.SerializeToString(&buffer_out);
        return buffer_out;
    }();

    for (auto _ : state) {
        proto::Obj3D pb_data_in;
        pb_data_in.ParseFromString(buffer_in);

        test::obj_3d data_in;
        convert_from_pb(pb_data_in, data_in);
    }
}

BENCHMARK(bm_serialization)->Repetitions(50);
BENCHMARK(bm_deserialization)->Repetitions(50);
BENCHMARK_MAIN();