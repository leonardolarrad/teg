#include <sstream>

#include "teg/teg.h"
#include "benchmarking/test_obj3d.h"
#include "benchmark/benchmark.h" // Google benchmark

// Boost.Serialization
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/serialization/vector.hpp"

#define PRINT_BUFFER_SIZE 1

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

#if PRINT_BUFFER_SIZE
#include <iostream>

class static_print_buffer {
public:
    static_print_buffer() {
        const auto data_out = bm::test_obj3d::generate_benchmark_data();

        std::ostringstream buffer_out;
        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;

        std::cout << "boost:buffer size: " << buffer_out.str().size() << std::endl;
    }
} static_print;
#endif // PRINT_BUFFER_SIZE

static void bm_serialization(benchmark::State& state) {
    const auto data_out = bm::test_obj3d::generate_benchmark_data();
        
    for (auto _ : state) {
        std::ostringstream buffer_out;
        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;
    }
}

static void bm_deserialization(benchmark::State& state) {
    const auto buffer_out = []() -> std::ostringstream {
        const auto data_out = bm::test_obj3d::generate_benchmark_data();

        std::ostringstream buffer_out;
        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;

        return buffer_out;
        }();

    for (auto _ : state) {
        bm::test_obj3d::obj_3d data_in;
        std::istringstream buffer_in(buffer_out.str());
        boost::archive::binary_iarchive archive_in(buffer_in);

        archive_in >> data_in;
    }
}

//BENCHMARK(bm_serialization)->MinWarmUpTime(2)->Repetitions(20);
BENCHMARK(bm_deserialization)->MinWarmUpTime(2)->Repetitions(20);
BENCHMARK_MAIN();
