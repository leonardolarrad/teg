#include <sstream>

#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

// Boost.Serialization
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/serialization/vector.hpp"

#define PRINT_BUFFER_SIZE 0

namespace bm = benchmarking;

namespace boost {
namespace serialization {

template <class Archive>
void serialize(Archive& ar, bm::test_obj3d::ivec3& v, const unsigned int) {
    ar & v.x & v.y & v.z;
}

template <class Archive>
void serialize(Archive& ar, bm::test_obj3d::fvec3& v, const unsigned int) {
    ar & v.x & v.y & v.z;
}

template <class Archive>
void serialize(Archive& ar, bm::test_obj3d::face& f, const unsigned int) {
    ar & f.vertex_index & f.normal_index;
}

template <class Archive>
void serialize(Archive& ar, bm::test_obj3d::obj_3d& o, const unsigned int) {
    ar & o.vertices & o.normals & o.faces;
}

} // namespace serialization
} // namespace boost

static inline void bm_serialization(benchmark::State& state) {
    auto data_out = bm::test_obj3d::generate_benchmark_data();

    std::ostringstream buffer_out;
    boost::archive::binary_oarchive archive_out(buffer_out);

    for (auto _ : state) {
    }
    /*
    auto data_out = bm::test_obj3d::generate_benchmark_data();
    std::ostringstream buffer_out;
    
    
    for (auto _ : state) {
        state.PauseTiming();
        boost::archive::binary_oarchive archive_out(buffer_out);
        buffer_out.seekp(0);
        state.ResumeTiming();

        archive_out << data_out;
    }
    */
    #if PRINT_BUFFER_SIZE
    std::cout << buffer_out.str().size() << std::endl;
    #endif
}

static inline void bm_deserialization(benchmark::State& state) {
    /*
    auto data_out = bm::test_obj3d::generate_benchmark_data();
    std::stringstream buffer_out;
    boost::archive::binary_oarchive archive_out(buffer_out);
    archive_out << data_out;

    bm::test_obj3d::obj_3d data_in;
    
    for (auto _ : state) {
        state.PauseTiming();
        std::istringstream buffer_in(buffer_out.str());
        boost::archive::binary_iarchive archive_in(buffer_in);
        state.ResumeTiming();

        archive_in >> data_in;
    }
    */
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();