#include <span>
#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "flatbuffers/flatbuffers.h" // FlatBuffers    
#include "ecommerce_generated.h"

#define BM_PROFILE_MEMORY 0

namespace test = benchmarking::test_ecommerce;

#if BM_PROFILE_MEMORY
#include "benchmarking/memory_profiling.h"
#endif

static auto serialize_ecommerce_pages(flatbuffers::FlatBufferBuilder& fbb, std::vector<test::ecommerce_page> const& data_out) -> std::span<uint8_t> {
    std::vector<flatbuffers::Offset<flatbuf::ecommerce::Page>> pages {};

    for (auto const& page : data_out) {
        auto const user = flatbuf::ecommerce::CreateUser(
            fbb,
            page.user.uuid,
            fbb.CreateString(page.user.name),
            fbb.CreateString(page.user.email),
            fbb.CreateVectorOfStrings(page.user.recent_searches)
        );

        std::vector<flatbuffers::Offset<flatbuf::ecommerce::Product>> products {};

        for (auto const& product : page.products) {
            products.emplace_back(flatbuf::ecommerce::CreateProduct(
                fbb,
                product.uuid,
                fbb.CreateString(product.name),
                fbb.CreateString(product.description),
                flatbuf::ecommerce::ProductCategory(product.category),
                fbb.CreateVectorOfStrings(product.tags),
                fbb.CreateString(product.image_lo_res_url)
            ));
        }

        pages.emplace_back(flatbuf::ecommerce::CreatePage(
            fbb,
            user,
            fbb.CreateString(page.permanent_url),
            fbb.CreateString(page.query),
            page.page,
            page.total_pages,
            page.results_per_page,
            page.total_results,
            fbb.CreateVector(products)
        ));
    }

    auto const page_list = flatbuf::ecommerce::CreatePageList(fbb, fbb.CreateVector(pages));
    flatbuf::ecommerce::FinishPageListBuffer(fbb, page_list);
    return std::span(fbb.GetBufferPointer(), fbb.GetSize());
}

static void bm_serialization(benchmark::State& state) {
    std::vector<test::ecommerce_page> data_out = test::generate_benchmark_data(1024, state.range(0) * 1024);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    for (auto _ : state) {
        flatbuffers::FlatBufferBuilder fbb;
        serialize_ecommerce_pages(fbb, data_out);

        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    flatbuffers::FlatBufferBuilder fbb;
    state.counters["Buffer size"] = benchmark::Counter(
        serialize_ecommerce_pages(fbb, data_out).size(),
        benchmark::Counter::kDefaults, 
        benchmark::Counter::kIs1024);

    state.counters["Bytes/s"] = benchmark::Counter(
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
    std::vector<test::ecommerce_page> data_out = test::generate_benchmark_data(1024, state.range(0) * 1024);
    flatbuffers::FlatBufferBuilder fbb;
    auto buffer_in = serialize_ecommerce_pages(fbb, data_out);

    #if BM_PROFILE_MEMORY
    uint64_t memory_usage = 0;
    #endif

    for (auto _ : state) {     
        auto const fb_page_list = flatbuf::ecommerce::GetPageList(buffer_in.data());

        // Traverse the data
        for (const auto& fb_page : *fb_page_list->pages()) {
            auto const fb_user = fb_page->user();

            for (const auto& fb_search : *fb_user->recent_searches()) {
                benchmark::DoNotOptimize(fb_search);
            }
            
            benchmark::DoNotOptimize(fb_user->uuid());
            benchmark::DoNotOptimize(fb_user->name()->str());
            benchmark::DoNotOptimize(fb_user->email()->str());

            for (const auto& fb_product : *fb_page->products()) {
                for (const auto& fb_tag : *fb_product->tags()) {
                    benchmark::DoNotOptimize(fb_tag->str());
                }
                
                benchmark::DoNotOptimize(fb_product->uuid());
                benchmark::DoNotOptimize(fb_product->name()->str());
                benchmark::DoNotOptimize(fb_product->description()->str());
                benchmark::DoNotOptimize(fb_product->category());
                benchmark::DoNotOptimize(fb_product->image_lo_res_url()->str());
            }
        }
        
        #if BM_PROFILE_MEMORY
        state.PauseTiming();
        memory_usage = std::max<uint64_t>(memory_usage, get_memory_usage());
        state.ResumeTiming();
        #endif
    }

    state.counters["Bytes/s"] = benchmark::Counter(
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