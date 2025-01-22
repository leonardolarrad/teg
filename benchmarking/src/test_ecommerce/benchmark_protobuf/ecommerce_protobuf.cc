#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h"

#define BM_PROFILE_MEMORY 0

#include "ecommerce.pb.h" // Google Protobuf
#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

namespace test = benchmarking::test_ecommerce;
namespace proto = protobuf::ecommerce;

void convert_to_pb(test::ecommerce_user const& src, proto::EcommerceUser* dest) {
    dest->set_uuid(src.uuid);
    dest->set_name(src.name);
    dest->set_email(src.email);
    for (auto const& s : src.recent_searches) {
        dest->add_recent_searches(s);
    }
}

void convert_from_pb(proto::EcommerceUser const& src, test::ecommerce_user& dest) {
    dest.uuid = src.uuid();
    dest.name = src.name();
    dest.email = src.email();
    for (auto const& s : src.recent_searches()) {
        dest.recent_searches.push_back(s);
    }
}

void convert_to_pb(test::ecommerce_product const& src, proto::EcommerceProduct* dest) {
    dest->set_uuid(src.uuid);
    dest->set_name(src.name);
    dest->set_description(src.description);
    for (auto const& s : src.tags) {
        dest->add_tags(s);
    }
    switch(src.category) {
        case test::electronics: dest->set_category(proto::EcommerceProductCategory::ELECTRONICS); break;
        case test::books:       dest->set_category(proto::EcommerceProductCategory::BOOKS); break;
        case test::clothing:    dest->set_category(proto::EcommerceProductCategory::CLOTHING); break;
        case test::home:        dest->set_category(proto::EcommerceProductCategory::HOME); break;
        case test::garden:      dest->set_category(proto::EcommerceProductCategory::GARDEN); break;
        case test::toys:        dest->set_category(proto::EcommerceProductCategory::TOYS); break;
        case test::food:        dest->set_category(proto::EcommerceProductCategory::FOOD); break;
        case test::baby:        dest->set_category(proto::EcommerceProductCategory::BABY); break;
        case test::pets:        dest->set_category(proto::EcommerceProductCategory::PETS); break;
        case test::health:      dest->set_category(proto::EcommerceProductCategory::HEALTH); break;
        case test::beauty:      dest->set_category(proto::EcommerceProductCategory::BEAUTY); break;
        default: break;
    }
    dest->set_image_lo_res_url(src.image_lo_res_url);
    dest->set_image_hi_res_url(src.image_hi_res_url);
    dest->set_price(src.price);
    dest->set_discount(src.discount);
    dest->set_stock(src.stock);
    dest->set_rating(src.rating);
    dest->set_reviews(src.reviews);
}

void convert_from_pb(proto::EcommerceProduct const& src, test::ecommerce_product& dest) {
    dest.uuid = src.uuid();
    dest.name = src.name();
    dest.description = src.description();
    for (auto const& s : src.tags()) {
        dest.tags.push_back(s);
    }
    switch(src.category()) {
        case proto::EcommerceProductCategory::ELECTRONICS: dest.category = test::electronics; break;
        case proto::EcommerceProductCategory::BOOKS: dest.category = test::books; break;
        case proto::EcommerceProductCategory::CLOTHING: dest.category = test::clothing; break;
        case proto::EcommerceProductCategory::HOME: dest.category = test::home; break;
        case proto::EcommerceProductCategory::GARDEN: dest.category = test::garden; break;
        case proto::EcommerceProductCategory::TOYS: dest.category = test::toys; break;
        case proto::EcommerceProductCategory::FOOD: dest.category = test::food; break;
        case proto::EcommerceProductCategory::BABY: dest.category = test::baby; break;
        case proto::EcommerceProductCategory::PETS: dest.category = test::pets; break;
        case proto::EcommerceProductCategory::HEALTH: dest.category = test::health; break;
        case proto::EcommerceProductCategory::BEAUTY: dest.category = test::beauty; break;
        default: break;
    }
    dest.image_lo_res_url = src.image_lo_res_url();
    dest.image_hi_res_url = src.image_hi_res_url();
    dest.price = src.price();
    dest.discount = src.discount();
    dest.stock = src.stock();
    dest.rating = src.rating();
    dest.reviews = src.reviews();
}

void convert_to_pb(test::ecommerce_page const& src, proto::EcommercePage* dest) {
    convert_to_pb(src.user, dest->mutable_user());
    dest->set_permanent_url(src.permanent_url);
    dest->set_query(src.query);
    dest->set_page(src.page);
    dest->set_total_pages(src.total_pages);
    dest->set_results_per_page(src.results_per_page);
    dest->set_total_results(src.total_results);
    for (auto const& p : src.products) {
        convert_to_pb(p, dest->add_products());
    }
}

void convert_from_pb(proto::EcommercePage const& src, test::ecommerce_page& dest) {
    convert_from_pb(src.user(), dest.user);
    dest.permanent_url = src.permanent_url();
    dest.query = src.query();
    dest.page = src.page();
    dest.total_pages = src.total_pages();
    dest.results_per_page = src.results_per_page();
    dest.total_results = src.total_results();
    for (auto const& p : src.products()) {
        convert_from_pb(p, dest.products.emplace_back());
    }
}

void convert_to_pb(std::vector<test::ecommerce_page> const& src, proto::EcommercePages* dest) {
    for (auto const& p : src) {
        convert_to_pb(p, dest->add_pages());
    }
}

void convert_from_pb(proto::EcommercePages const& src, std::vector<test::ecommerce_page>& dest) {
    for (auto const& p : src.pages()) {
        convert_from_pb(p, dest.emplace_back());
    }
}

static void bm_serialization(benchmark::State& state) {
    auto const test_obj = test::generate_benchmark_data(1024, state.range(0) * 1024);

    proto::EcommercePages test_pb;
    convert_to_pb(test_obj, &test_pb);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    std::string buffer_out;
    for (auto _ : state) {
        test_pb.SerializeToString(&buffer_out);

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
    const auto buffer_in = [&]() -> std::string {
        auto const test_obj = test::generate_benchmark_data(1024, state.range(0) * 1024);
        proto::EcommercePages pb_data_out;
        convert_to_pb(test_obj, &pb_data_out);
        
        std::string buffer_out;
        pb_data_out.SerializeToString(&buffer_out);
        return buffer_out;
    }();

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    proto::EcommercePages pb_data_in;
    for (auto _ : state) {
        pb_data_in.ParseFromString(buffer_in);

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

BENCHMARK(bm_serialization)
    ->RangeMultiplier(2)->Range(1, 1024)
    ->MinWarmUpTime(1)
    ->Repetitions(30)
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK(bm_deserialization)
    ->RangeMultiplier(2)->Range(1, 1024)
    ->MinWarmUpTime(1)
    ->Repetitions(30)
    ->Unit(benchmark::kMillisecond)
    ->UseRealTime();

BENCHMARK_MAIN();