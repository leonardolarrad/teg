#include "teg/teg.h"
#include "benchmark/benchmark.h"
#include "benchmark/data.h"
#include "test_data.pb.h"
#include "test_data_conversions.h"

namespace bm = benchmarking;    

static std::vector<bm::ecommerce_page> data_in_10_2048b = bm::generate_benchmark_data(10, 2048);

static bool test_lib() {
    std::string buffer;
    pb::EcommercePage pb_t0;
    bm::ecommerce_page const bm_t0 = data_in_10_2048b[0];
    convert_to_pb(bm_t0, &pb_t0);
    pb_t0.SerializeToString(&buffer);

    pb::EcommercePage pb_t1;
    bm::ecommerce_page bm_t1;
    pb_t1.ParseFromString(buffer);
    convert_from_pb(pb_t1, bm_t1);

    return bm_t0 == bm_t1;
}

static void benchmark() {
    std::string buffer;

    bm::benchmark()
        .warmup(64)
        .iterations(102400)
        .repetitions(10)
        .run("protobuf-25.2:serialization:10_1024b", [&](){
            pb::EcommercePages pages;
            for (auto const& data : data_in_10_2048b) {
                convert_to_pb(data, pages.add_pages());
            }
            pages.SerializeToString(&buffer);
        })
        .run("protobuf-25.2:deserialization:10_1024b", [&](){
            pb::EcommercePages pages;
            pages.ParseFromString(buffer);
            std::vector<bm::ecommerce_page> data_out_10_1024b;
            data_out_10_1024b.reserve(pages.pages().size());
            for (auto const& page : pages.pages()) {
                convert_from_pb(page, data_out_10_1024b.emplace_back());
            }
        });
}

int main() {
    if (!test_lib()) {
        return 1;
    }
    benchmark();
    return 0;    
}