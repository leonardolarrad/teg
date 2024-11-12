#include "benchmark/benchmark.h"
#include "benchmark/data.h"
#include "zpp_bits.h"

namespace bm = benchmarking;

static std::vector<bm::ecommerce_page> data_out_1mib = bm::generate_benchmark_data(512, 1024*2);

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

static void run_benchmark_00() {
    std::vector<bm::ecommerce_page> data_in_1mib;
    std::vector<std::byte> buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(1024)
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

static void run_benchmark_01() {
    int64_t d0 = 99999999999999ull;
    int64_t d1;
    std::vector<std::byte> buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(1024 * 1024 * 1024)
        .repetitions(10)
        .run("zppbits:serialization:i64", [&](){
            buffer.clear();
            zpp::bits::out{buffer}(d0).or_throw();
        })
        .run("zppbits:deserialization:i64", [&](){            
            zpp::bits::in{buffer}(d1).or_throw();
        })
        ;
}

static void run_benchmark_02() {
    std::string d0 = "Hello World!";
    std::string d1;
    std::vector<std::byte> buffer{};

    bm::benchmark()
        .warmup(10)
        .iterations(1024 * 1024 * 100)
        .repetitions(10)
        .run("zppbits:serialization:simple_string", [&](){
            buffer.clear();
            zpp::bits::out{buffer}(d0).or_throw();
        })
        .run("zppbits:deserialization:simple_string", [&](){            
            zpp::bits::in{buffer}(d1).or_throw();
        })
        ;
}

int main() {
    if (!test_lib()) {
        return 1;
    }
    //benchmark_lib();
    run_benchmark_00();
    return 0;    
}