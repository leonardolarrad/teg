#include "teg/teg.h"
#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "msgpack.hpp" // MessagePack

#define BM_PROFILE_MEMORY 1

#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

namespace test = benchmarking::test_ecommerce;

namespace msgpack {
MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
namespace adaptor {

template <>
struct pack<test::ecommerce_product_category> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const test::ecommerce_product_category& v) const {
        o.pack((std::uint8_t)v);
        return o;
    }
};

template <>
struct convert<test::ecommerce_product_category> {
    msgpack::object const& operator()(msgpack::object const& o, test::ecommerce_product_category& v) const {
        v = (test::ecommerce_product_category) o.as<std::uint8_t>();
        return o;        
    }
};

template <>
struct pack<test::ecommerce_user> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const test::ecommerce_user& v) const {
        o.pack_array(4);
        o.pack(v.uuid);
        o.pack(v.name);
        o.pack(v.email);
        o.pack(v.recent_searches);
        return o;
    }
};

template <>
struct convert<test::ecommerce_user> {
    msgpack::object const& operator()(msgpack::object const& o, test::ecommerce_user& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 4) { throw msgpack::type_error(); }
        v.uuid = o.via.array.ptr[0].as<std::uint64_t>();
        v.name = o.via.array.ptr[1].as<std::string>();
        v.email = o.via.array.ptr[2].as<std::string>();
        v.recent_searches = o.via.array.ptr[3].as<std::vector<std::string>>();
        return o;
    }
};

template <>
struct pack<test::ecommerce_product> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const test::ecommerce_product& v) const {
        o.pack_array(12);
        o.pack(v.uuid);
        o.pack(v.name);
        o.pack(v.description);
        o.pack(v.category);
        o.pack(v.tags);
        o.pack(v.image_lo_res_url);
        o.pack(v.image_hi_res_url);
        o.pack(v.price);
        o.pack(v.discount);
        o.pack(v.stock);
        o.pack(v.rating);
        o.pack(v.reviews);
        return o;
    }
};

template <>
struct convert<test::ecommerce_product> {
    msgpack::object const& operator()(msgpack::object const& o, test::ecommerce_product& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 12) { throw msgpack::type_error(); }
        v.uuid = o.via.array.ptr[0].as<std::uint64_t>();
        v.name = o.via.array.ptr[1].as<std::string>();
        v.description = o.via.array.ptr[2].as<std::string>();
        v.category = o.via.array.ptr[3].as<test::ecommerce_product_category>();
        v.tags = o.via.array.ptr[4].as<std::vector<std::string>>();
        v.image_lo_res_url = o.via.array.ptr[5].as<std::string>();
        v.image_hi_res_url = o.via.array.ptr[6].as<std::string>();
        v.price = o.via.array.ptr[7].as<double>();
        v.discount = o.via.array.ptr[8].as<double>();
        v.stock = o.via.array.ptr[9].as<std::uint32_t>();
        v.rating = o.via.array.ptr[10].as<std::uint8_t>();
        v.reviews = o.via.array.ptr[11].as<std::uint32_t>();
        return o;
    }
};

template <>
struct pack<test::ecommerce_page> {
    template <typename Stream>
    packer<Stream>& operator()(msgpack::packer<Stream>& o, const test::ecommerce_page& v) const {
        o.pack_array(8);
        o.pack(v.user);
        o.pack(v.permanent_url);
        o.pack(v.query);
        o.pack(v.page);
        o.pack(v.total_pages);
        o.pack(v.results_per_page);
        o.pack(v.total_results);
        o.pack(v.products);
        return o;
    }
};

template <>
struct convert<test::ecommerce_page> {
    msgpack::object const& operator()(msgpack::object const& o, test::ecommerce_page& v) const {
        if (o.type != msgpack::type::ARRAY) { throw msgpack::type_error(); }
        if (o.via.array.size != 8) { throw msgpack::type_error(); }
        v.user = o.via.array.ptr[0].as<test::ecommerce_user>();
        v.permanent_url = o.via.array.ptr[1].as<std::string>();
        v.query = o.via.array.ptr[2].as<std::string>();
        v.page = o.via.array.ptr[3].as<std::uint32_t>();
        v.total_pages = o.via.array.ptr[4].as<std::uint32_t>();
        v.results_per_page = o.via.array.ptr[5].as<std::uint32_t>();
        v.total_results = o.via.array.ptr[6].as<std::uint32_t>();
        v.products = o.via.array.ptr[7].as<std::vector<test::ecommerce_product>>();
        return o;
    }
};

} // namespace adaptor
} // MSGPACK_API_VERSION_NAMESPACE
} // namespace msgpack


static void bm_serialization(benchmark::State& state) {  
    auto const data_out = test::generate_benchmark_data(1024, state.range(0) * 1024);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    msgpack::sbuffer buffer_out;
    for (auto _ : state) {
        buffer_out.clear();
        msgpack::pack(buffer_out, data_out);

        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    state.counters["Buffer size (B)"] = benchmark::Counter(
        buffer_out.size(), 
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);
    state.counters["B/s"] = benchmark::Counter(
        int64_t(state.iterations()) * int64_t(state.range(0) * 1024 * 1024),
        benchmark::Counter::kIsRate, 
        benchmark::Counter::kIs1024);

    #if BM_PROFILE_MEMORY
    state.counters["Memory usage (B)"] = benchmark::Counter(
        memory_usage, 
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);
    #endif 
}

static void bm_deserialization(benchmark::State& state) {
    auto buffer_in = [&]() -> msgpack::sbuffer {
        auto const data_out = test::generate_benchmark_data(1024, state.range(0) * 1024);
        msgpack::sbuffer buffer;
        msgpack::pack(buffer, data_out);
        return buffer;
    }();
    std::vector<test::ecommerce_page> data_in;
    
    #if BM_PROFILE_MEMORY
        uint64_t memory_usage = 0;
    #endif

    for (auto _ : state) {
        msgpack::object_handle obj_handle = msgpack::unpack(buffer_in.data(), buffer_in.size());
        msgpack::object obj = obj_handle.get();
        obj.convert(data_in);

        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    state.counters["B/s"] = benchmark::Counter(
    int64_t(state.iterations()) * int64_t(state.range(0) * 1024 * 1024),
    benchmark::Counter::kIsRate, 
    benchmark::Counter::kIs1024);

    #if BM_PROFILE_MEMORY
    state.counters["Memory usage (B)"] = benchmark::Counter(
        memory_usage, 
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);
    #endif 
}

//BENCHMARK(bm_serialization)
//    ->RangeMultiplier(2)->Range(1, 1024)
//    ->MinWarmUpTime(1)
//    ->Repetitions(30)
//    ->Unit(benchmark::kMillisecond)
//    ->UseRealTime();

BENCHMARK(bm_deserialization)
    ->RangeMultiplier(2)->Range(1, 1024)
    ->MinWarmUpTime(1)
    ->Repetitions(1)
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK_MAIN();