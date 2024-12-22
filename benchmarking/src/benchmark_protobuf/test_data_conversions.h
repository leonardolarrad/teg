#pragma once
#include "benchmark/benchmark.h"
#include "benchmark/data.h"
#include "test_data.pb.h"

namespace bm = benchmarking;

void convert_to_pb(bm::ecommerce_user const& src, pb::EcommerceUser* dest) {
    dest->set_uuid(src.uuid);
    dest->set_name(src.name);
    dest->set_email(src.email);
    for (auto const& s : src.recent_searches) {
        dest->add_recent_searches(s);
    }
}

void convert_from_pb(pb::EcommerceUser const& src, bm::ecommerce_user& dest) {
    dest.uuid = src.uuid();
    dest.name = src.name();
    dest.email = src.email();
    for (auto const& s : src.recent_searches()) {
        dest.recent_searches.push_back(s);
    }
}

void convert_to_pb(bm::ecommerce_product const& src, pb::EcommerceProduct* dest) {
    dest->set_uuid(src.uuid);
    dest->set_name(src.name);
    dest->set_description(src.description);
    for (auto const& s : src.tags) {
        dest->add_tags(s);
    }
    switch(src.category) {
        case bm::electronics: dest->set_category(pb::EcommerceProductCategory::ELECTRONICS); break;
        case bm::books:       dest->set_category(pb::EcommerceProductCategory::BOOKS); break;
        case bm::clothing:    dest->set_category(pb::EcommerceProductCategory::CLOTHING); break;
        case bm::home:        dest->set_category(pb::EcommerceProductCategory::HOME); break;
        case bm::garden:      dest->set_category(pb::EcommerceProductCategory::GARDEN); break;
        case bm::toys:        dest->set_category(pb::EcommerceProductCategory::TOYS); break;
        case bm::food:        dest->set_category(pb::EcommerceProductCategory::FOOD); break;
        case bm::baby:        dest->set_category(pb::EcommerceProductCategory::BABY); break;
        case bm::pets:        dest->set_category(pb::EcommerceProductCategory::PETS); break;
        case bm::health:      dest->set_category(pb::EcommerceProductCategory::HEALTH); break;
        case bm::beauty:      dest->set_category(pb::EcommerceProductCategory::BEAUTY); break;
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

void convert_from_pb(pb::EcommerceProduct const& src, bm::ecommerce_product& dest) {
    dest.uuid = src.uuid();
    dest.name = src.name();
    dest.description = src.description();
    for (auto const& s : src.tags()) {
        dest.tags.push_back(s);
    }
    switch(src.category()) {
        case pb::EcommerceProductCategory::ELECTRONICS: dest.category = bm::electronics; break;
        case pb::EcommerceProductCategory::BOOKS: dest.category = bm::books; break;
        case pb::EcommerceProductCategory::CLOTHING: dest.category = bm::clothing; break;
        case pb::EcommerceProductCategory::HOME: dest.category = bm::home; break;
        case pb::EcommerceProductCategory::GARDEN: dest.category = bm::garden; break;
        case pb::EcommerceProductCategory::TOYS: dest.category = bm::toys; break;
        case pb::EcommerceProductCategory::FOOD: dest.category = bm::food; break;
        case pb::EcommerceProductCategory::BABY: dest.category = bm::baby; break;
        case pb::EcommerceProductCategory::PETS: dest.category = bm::pets; break;
        case pb::EcommerceProductCategory::HEALTH: dest.category = bm::health; break;
        case pb::EcommerceProductCategory::BEAUTY: dest.category = bm::beauty; break;
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

void convert_to_pb(bm::ecommerce_page const& src, pb::EcommercePage* dest) {
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

void convert_from_pb(pb::EcommercePage const& src, bm::ecommerce_page& dest) {
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