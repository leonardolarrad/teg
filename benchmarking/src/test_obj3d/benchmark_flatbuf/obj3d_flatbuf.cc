#include <span>
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "flatbuffers/flatbuffers.h"
#include "obj3d_generated.h"

#define PRINT_BUFFER_SIZE 1
#define TEST_LIB          0

static auto const data_out = benchmarking::test_obj3d::generate_benchmark_data();

static inline auto serialize_obj3d(flatbuffers::FlatBufferBuilder& fbb) -> std::span<uint8_t> {
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

#if PRINT_BUFFER_SIZE
#include <iostream>
class static_print_size {
public:
    static_print_size() {
        flatbuffers::FlatBufferBuilder fbb;
        std::cout << serialize_obj3d(fbb).size() << std::endl;
    }
} static_print_size;
#endif

static void bm_serialization(benchmark::State& state) {
    for (auto _ : state) {
        flatbuffers::FlatBufferBuilder fbb;
        serialize_obj3d(fbb);
    }
}

static void bm_deserialization(benchmark::State& state) {    
    flatbuffers::FlatBufferBuilder fbb;
    auto buffer_in = serialize_obj3d(fbb);

    for (auto _ : state) {
        benchmarking::test_obj3d::obj_3d obj3d;
        auto const fb_obj3d = flatbuf::obj3d::GetObj3D(buffer_in.data());

        obj3d.vertices.reserve(fb_obj3d->vertices()->size());
        obj3d.normals.reserve(fb_obj3d->normals()->size());
        obj3d.faces.reserve(fb_obj3d->faces()->size());

        for (const auto& v : *fb_obj3d->vertices()) {
            obj3d.vertices.emplace_back(benchmarking::test_obj3d::vertex(v->x(), v->y(), v->z()));
        }

        for (const auto& n : *fb_obj3d->normals()) {
            obj3d.normals.emplace_back(benchmarking::test_obj3d::normal(n->x(), n->y(), n->z()));
        }

        for (const auto& f : *fb_obj3d->faces()) {
            obj3d.faces.emplace_back(benchmarking::test_obj3d::face(
                benchmarking::test_obj3d::ivec3(f->vertex_index().x(), f->vertex_index().y(), f->vertex_index().z()),
                benchmarking::test_obj3d::ivec3(f->normal_index().x(), f->normal_index().y(), f->normal_index().z())));
        }
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();