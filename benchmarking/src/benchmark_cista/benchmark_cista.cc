#include "benchmark/benchmark.h"
#include "benchmark/data.h"

#include "cista.h"

namespace bm = benchmarking;

static std::vector<bm::ecommerce_page> data_out_1mib = bm::generate_benchmark_data(512, 1024*2);

static bool test_lib() {
    constexpr auto const MODE = cista::mode::WITH_VERSION;
    int i0 = 111;
    auto serialized = cista::serialize<MODE>(i0);
    int i1;
    i1 = *cista::deserialize<int, MODE>(serialized);
    return i0 == i1;
}

static void run_benchmark() {
    constexpr auto MODE = cista::mode::WITH_INTEGRITY;    

    cista::offset::string cista_data_out_1mib = 
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
        "AAAAAAAAAAAAAAAAAAAasdasdasd12312312312asssssssssssssssssssss"
    ;

    bm::benchmark()
        .warmup(10)
        .iterations(1000)
        .repetitions(10)
        .run("cista:serialization:checksum", [&](){
            auto serialized = cista::serialize<MODE>(cista_data_out_1mib);
        })
        ;//.run("teg:deserialization:i64", [&](){
        //    teg::deserialize(buffer, d1).or_throw();
        //});
}

int main() {
    if (!test_lib()) {
        return 1;
    }
    run_benchmark();
    return 0;
}
