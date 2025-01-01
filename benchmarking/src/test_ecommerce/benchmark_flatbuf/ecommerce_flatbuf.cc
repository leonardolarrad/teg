#define PRINT_BUFFER_SIZE 1

#include <span>
#include "benchmarking/test_ecommerce.h"
#include "benchmark/benchmark.h" // Google benchmark

#include "flatbuffers/flatbuffers.h"    
#include "ecommerce_generated.h"

namespace test = benchmarking::test_ecommerce;

static std::vector<test::ecommerce_page> data_out =
    test::generate_benchmark_data(512, 2048); // 1 MiB

static auto serialize_ecommerce_pages(flatbuffers::FlatBufferBuilder& fbb) -> std::span<uint8_t> {
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

#if PRINT_BUFFER_SIZE
#include <iostream>

class static_print_size {
public:
    static_print_size() {
        flatbuffers::FlatBufferBuilder fbb;
        std::cout << serialize_ecommerce_pages(fbb).size() << std::endl;
    }
} static_print_size;
#endif

static void bm_serialization(benchmark::State& state) {
    for (auto _ : state) {
        flatbuffers::FlatBufferBuilder fbb;
        serialize_ecommerce_pages(fbb);
    }
}

static void bm_deserialization(benchmark::State& state) {
    flatbuffers::FlatBufferBuilder fbb;
    auto buffer_in = serialize_ecommerce_pages(fbb);

    for (auto _ : state) {
        std::vector<test::ecommerce_page> data_in;        
        auto const fb_page_list = flatbuf::ecommerce::GetPageList(buffer_in.data());
        data_in.reserve(fb_page_list->pages()->size());

        for (const auto& fb_page : *fb_page_list->pages()) {
            auto const fb_user = fb_page->user();

            std::vector<std::string> recent_searches;
            recent_searches.reserve(fb_user->recent_searches()->size());
            for (const auto& fb_search : *fb_user->recent_searches()) {
                recent_searches.emplace_back(fb_search->str());
            }
            
            test::ecommerce_user user {
                fb_user->uuid(),
                fb_user->name()->str(),
                fb_user->email()->str(),
                recent_searches
            };
            
            std::vector<test::ecommerce_product> products;
            products.reserve(fb_page->products()->size());

            for (const auto& fb_product : *fb_page->products()) {
                std::vector<std::string> tags;
                tags.reserve(fb_product->tags()->size());
                for (const auto& fb_tag : *fb_product->tags()) {
                    tags.emplace_back(fb_tag->str());
                }
                
                test::ecommerce_product product {
                    fb_product->uuid(),
                    fb_product->name()->str(),
                    fb_product->description()->str(),
                    static_cast<test::ecommerce_product_category>(fb_product->category()),
                    tags,
                    fb_product->image_lo_res_url()->str()
                };
                products.emplace_back(product);
            }

            test::ecommerce_page page {
                user,
                fb_page->permanent_url()->str(),
                fb_page->query()->str(),
                fb_page->page(),
                fb_page->total_pages(),
                fb_page->results_per_page(),
                fb_page->total_results(),
                products
            };

            data_in.emplace_back(page);
        }
    }
}

BENCHMARK(bm_serialization)->Repetitions(10);
BENCHMARK(bm_deserialization)->Repetitions(10);
BENCHMARK_MAIN();