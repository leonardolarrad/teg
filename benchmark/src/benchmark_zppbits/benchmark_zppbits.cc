#include "benchmark/benchmark.h"
#include "benchmark/data.h"
#include "zpp_bits.h"

namespace bm = benchmarking;

static std::vector<bm::ecommerce_page> data_out_1mib = bm::generate_benchmark_data(512, 2048);

static bool test_lib() {
    try {
        auto [data, in, out] = zpp::bits::data_in_out();
        auto& t0 = data_out_1mib[0];
        out(t0).or_throw();

        bm::ecommerce_page t1;
        in(t1).or_throw();

        return t0 == t1;
    } catch (const std::exception& e) {
        return false;
    }
}


static void benchmark_lib() {
    std::vector<bm::ecommerce_page> data_in_1mib;
    std::vector<std::byte> buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(512)
        .repetitions(10)
        .run("zppbits:serialization:1mib", [&](){
            buffer.clear();
            zpp::bits::out{buffer}(data_out_1mib).or_throw();
        })
        .run("zppbits:deserialization:1mib", [&](){            
            zpp::bits::in{buffer}(data_in_1mib).or_throw();
        })
        ;

    std::cout << "\nBuffer size: " << buffer.size() << std::endl;
}

int main() {
    if (!test_lib()) {
        return 1;
    }
    benchmark_lib();
    return 0;    
}