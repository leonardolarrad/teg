#define PRINT_BUFFER_SIZE 1

#include <sstream>
#include "teg/teg.h"
#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h" // Google benchmark

// Boost.Serialization
#include "boost/archive/binary_iarchive.hpp"
#include "boost/archive/binary_oarchive.hpp"
#include "boost/serialization/vector.hpp"

namespace test = benchmarking::test_ecommerce;

static std::vector<test::ecommerce_page> data_out =
    test::generate_benchmark_data(512, 2048); // 1 MiB

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

#if PRINT_BUFFER_SIZE
#include <iostream>

class static_print_buffer {
public:
    static_print_buffer() {
        std::ostringstream buffer_out;
        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;

        std::cout << "boost:buffer size: " << buffer_out.str().size() << std::endl;
    }
} static_print;
#endif // PRINT_BUFFER_SIZE

static void bm_serialization(benchmark::State& state) {        
    for (auto _ : state) {
        std::ostringstream buffer_out;
        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;
    }
}

static void bm_deserialization(benchmark::State& state) {
    const auto buffer_out = []() -> std::ostringstream {
        std::ostringstream buffer_out;
        boost::archive::binary_oarchive archive_out(buffer_out);
        archive_out << data_out;

        return buffer_out;
        }();

    for (auto _ : state) {
        std::vector<test::ecommerce_page> data_in;
        std::istringstream buffer_in(buffer_out.str());
        boost::archive::binary_iarchive archive_in(buffer_in);

        archive_in >> data_in;
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();
