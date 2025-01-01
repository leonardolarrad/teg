// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ECOMMERCE_FLATBUF_ECOMMERCE_H_
#define FLATBUFFERS_GENERATED_ECOMMERCE_FLATBUF_ECOMMERCE_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 6,
             "Non-compatible flatbuffers version included");

namespace flatbuf {
namespace ecommerce {

struct User;
struct UserBuilder;

struct Product;
struct ProductBuilder;

struct Page;
struct PageBuilder;

struct PageList;
struct PageListBuilder;

enum ProductCategory : int8_t {
  ProductCategory_Electronics = 0,
  ProductCategory_Books = 1,
  ProductCategory_Clothing = 2,
  ProductCategory_Home = 3,
  ProductCategory_Garden = 4,
  ProductCategory_Toys = 5,
  ProductCategory_Food = 6,
  ProductCategory_Baby = 7,
  ProductCategory_Pets = 8,
  ProductCategory_Health = 9,
  ProductCategory_Beauty = 10,
  ProductCategory_MIN = ProductCategory_Electronics,
  ProductCategory_MAX = ProductCategory_Beauty
};

inline const ProductCategory (&EnumValuesProductCategory())[11] {
  static const ProductCategory values[] = {
    ProductCategory_Electronics,
    ProductCategory_Books,
    ProductCategory_Clothing,
    ProductCategory_Home,
    ProductCategory_Garden,
    ProductCategory_Toys,
    ProductCategory_Food,
    ProductCategory_Baby,
    ProductCategory_Pets,
    ProductCategory_Health,
    ProductCategory_Beauty
  };
  return values;
}

inline const char * const *EnumNamesProductCategory() {
  static const char * const names[12] = {
    "Electronics",
    "Books",
    "Clothing",
    "Home",
    "Garden",
    "Toys",
    "Food",
    "Baby",
    "Pets",
    "Health",
    "Beauty",
    nullptr
  };
  return names;
}

inline const char *EnumNameProductCategory(ProductCategory e) {
  if (::flatbuffers::IsOutRange(e, ProductCategory_Electronics, ProductCategory_Beauty)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesProductCategory()[index];
}

struct User FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef UserBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UUID = 4,
    VT_NAME = 6,
    VT_EMAIL = 8,
    VT_RECENT_SEARCHES = 10
  };
  uint64_t uuid() const {
    return GetField<uint64_t>(VT_UUID, 0);
  }
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  const ::flatbuffers::String *email() const {
    return GetPointer<const ::flatbuffers::String *>(VT_EMAIL);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *recent_searches() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *>(VT_RECENT_SEARCHES);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_UUID, 8) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_EMAIL) &&
           verifier.VerifyString(email()) &&
           VerifyOffset(verifier, VT_RECENT_SEARCHES) &&
           verifier.VerifyVector(recent_searches()) &&
           verifier.VerifyVectorOfStrings(recent_searches()) &&
           verifier.EndTable();
  }
};

struct UserBuilder {
  typedef User Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_uuid(uint64_t uuid) {
    fbb_.AddElement<uint64_t>(User::VT_UUID, uuid, 0);
  }
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(User::VT_NAME, name);
  }
  void add_email(::flatbuffers::Offset<::flatbuffers::String> email) {
    fbb_.AddOffset(User::VT_EMAIL, email);
  }
  void add_recent_searches(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> recent_searches) {
    fbb_.AddOffset(User::VT_RECENT_SEARCHES, recent_searches);
  }
  explicit UserBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<User> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<User>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<User> CreateUser(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t uuid = 0,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    ::flatbuffers::Offset<::flatbuffers::String> email = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> recent_searches = 0) {
  UserBuilder builder_(_fbb);
  builder_.add_uuid(uuid);
  builder_.add_recent_searches(recent_searches);
  builder_.add_email(email);
  builder_.add_name(name);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<User> CreateUserDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t uuid = 0,
    const char *name = nullptr,
    const char *email = nullptr,
    const std::vector<::flatbuffers::Offset<::flatbuffers::String>> *recent_searches = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto email__ = email ? _fbb.CreateString(email) : 0;
  auto recent_searches__ = recent_searches ? _fbb.CreateVector<::flatbuffers::Offset<::flatbuffers::String>>(*recent_searches) : 0;
  return flatbuf::ecommerce::CreateUser(
      _fbb,
      uuid,
      name__,
      email__,
      recent_searches__);
}

struct Product FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef ProductBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_UUID = 4,
    VT_NAME = 6,
    VT_DESCRIPTION = 8,
    VT_CATEGORY = 10,
    VT_TAGS = 12,
    VT_IMAGE_LO_RES_URL = 14,
    VT_IMAGE_HI_RES_URL = 16,
    VT_PRICE = 18,
    VT_DISCOUNT = 20,
    VT_STOCK = 22,
    VT_RATING = 24,
    VT_REVIEWS = 26
  };
  uint64_t uuid() const {
    return GetField<uint64_t>(VT_UUID, 0);
  }
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  const ::flatbuffers::String *description() const {
    return GetPointer<const ::flatbuffers::String *>(VT_DESCRIPTION);
  }
  flatbuf::ecommerce::ProductCategory category() const {
    return static_cast<flatbuf::ecommerce::ProductCategory>(GetField<int8_t>(VT_CATEGORY, 0));
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *tags() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>> *>(VT_TAGS);
  }
  const ::flatbuffers::String *image_lo_res_url() const {
    return GetPointer<const ::flatbuffers::String *>(VT_IMAGE_LO_RES_URL);
  }
  const ::flatbuffers::String *image_hi_res_url() const {
    return GetPointer<const ::flatbuffers::String *>(VT_IMAGE_HI_RES_URL);
  }
  double price() const {
    return GetField<double>(VT_PRICE, 0.0);
  }
  double discount() const {
    return GetField<double>(VT_DISCOUNT, 0.0);
  }
  uint32_t stock() const {
    return GetField<uint32_t>(VT_STOCK, 0);
  }
  int8_t rating() const {
    return GetField<int8_t>(VT_RATING, 0);
  }
  uint32_t reviews() const {
    return GetField<uint32_t>(VT_REVIEWS, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_UUID, 8) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_DESCRIPTION) &&
           verifier.VerifyString(description()) &&
           VerifyField<int8_t>(verifier, VT_CATEGORY, 1) &&
           VerifyOffset(verifier, VT_TAGS) &&
           verifier.VerifyVector(tags()) &&
           verifier.VerifyVectorOfStrings(tags()) &&
           VerifyOffset(verifier, VT_IMAGE_LO_RES_URL) &&
           verifier.VerifyString(image_lo_res_url()) &&
           VerifyOffset(verifier, VT_IMAGE_HI_RES_URL) &&
           verifier.VerifyString(image_hi_res_url()) &&
           VerifyField<double>(verifier, VT_PRICE, 8) &&
           VerifyField<double>(verifier, VT_DISCOUNT, 8) &&
           VerifyField<uint32_t>(verifier, VT_STOCK, 4) &&
           VerifyField<int8_t>(verifier, VT_RATING, 1) &&
           VerifyField<uint32_t>(verifier, VT_REVIEWS, 4) &&
           verifier.EndTable();
  }
};

struct ProductBuilder {
  typedef Product Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_uuid(uint64_t uuid) {
    fbb_.AddElement<uint64_t>(Product::VT_UUID, uuid, 0);
  }
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(Product::VT_NAME, name);
  }
  void add_description(::flatbuffers::Offset<::flatbuffers::String> description) {
    fbb_.AddOffset(Product::VT_DESCRIPTION, description);
  }
  void add_category(flatbuf::ecommerce::ProductCategory category) {
    fbb_.AddElement<int8_t>(Product::VT_CATEGORY, static_cast<int8_t>(category), 0);
  }
  void add_tags(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> tags) {
    fbb_.AddOffset(Product::VT_TAGS, tags);
  }
  void add_image_lo_res_url(::flatbuffers::Offset<::flatbuffers::String> image_lo_res_url) {
    fbb_.AddOffset(Product::VT_IMAGE_LO_RES_URL, image_lo_res_url);
  }
  void add_image_hi_res_url(::flatbuffers::Offset<::flatbuffers::String> image_hi_res_url) {
    fbb_.AddOffset(Product::VT_IMAGE_HI_RES_URL, image_hi_res_url);
  }
  void add_price(double price) {
    fbb_.AddElement<double>(Product::VT_PRICE, price, 0.0);
  }
  void add_discount(double discount) {
    fbb_.AddElement<double>(Product::VT_DISCOUNT, discount, 0.0);
  }
  void add_stock(uint32_t stock) {
    fbb_.AddElement<uint32_t>(Product::VT_STOCK, stock, 0);
  }
  void add_rating(int8_t rating) {
    fbb_.AddElement<int8_t>(Product::VT_RATING, rating, 0);
  }
  void add_reviews(uint32_t reviews) {
    fbb_.AddElement<uint32_t>(Product::VT_REVIEWS, reviews, 0);
  }
  explicit ProductBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Product> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Product>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Product> CreateProduct(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t uuid = 0,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    ::flatbuffers::Offset<::flatbuffers::String> description = 0,
    flatbuf::ecommerce::ProductCategory category = flatbuf::ecommerce::ProductCategory_Electronics,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<::flatbuffers::String>>> tags = 0,
    ::flatbuffers::Offset<::flatbuffers::String> image_lo_res_url = 0,
    ::flatbuffers::Offset<::flatbuffers::String> image_hi_res_url = 0,
    double price = 0.0,
    double discount = 0.0,
    uint32_t stock = 0,
    int8_t rating = 0,
    uint32_t reviews = 0) {
  ProductBuilder builder_(_fbb);
  builder_.add_discount(discount);
  builder_.add_price(price);
  builder_.add_uuid(uuid);
  builder_.add_reviews(reviews);
  builder_.add_stock(stock);
  builder_.add_image_hi_res_url(image_hi_res_url);
  builder_.add_image_lo_res_url(image_lo_res_url);
  builder_.add_tags(tags);
  builder_.add_description(description);
  builder_.add_name(name);
  builder_.add_rating(rating);
  builder_.add_category(category);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Product> CreateProductDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t uuid = 0,
    const char *name = nullptr,
    const char *description = nullptr,
    flatbuf::ecommerce::ProductCategory category = flatbuf::ecommerce::ProductCategory_Electronics,
    const std::vector<::flatbuffers::Offset<::flatbuffers::String>> *tags = nullptr,
    const char *image_lo_res_url = nullptr,
    const char *image_hi_res_url = nullptr,
    double price = 0.0,
    double discount = 0.0,
    uint32_t stock = 0,
    int8_t rating = 0,
    uint32_t reviews = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto description__ = description ? _fbb.CreateString(description) : 0;
  auto tags__ = tags ? _fbb.CreateVector<::flatbuffers::Offset<::flatbuffers::String>>(*tags) : 0;
  auto image_lo_res_url__ = image_lo_res_url ? _fbb.CreateString(image_lo_res_url) : 0;
  auto image_hi_res_url__ = image_hi_res_url ? _fbb.CreateString(image_hi_res_url) : 0;
  return flatbuf::ecommerce::CreateProduct(
      _fbb,
      uuid,
      name__,
      description__,
      category,
      tags__,
      image_lo_res_url__,
      image_hi_res_url__,
      price,
      discount,
      stock,
      rating,
      reviews);
}

struct Page FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef PageBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_USER = 4,
    VT_PERMANENT_URL = 6,
    VT_QUERY = 8,
    VT_PAGE = 10,
    VT_TOTAL_PAGES = 12,
    VT_RESULTS_PER_PAGE = 14,
    VT_TOTAL_RESULTS = 16,
    VT_PRODUCTS = 18
  };
  const flatbuf::ecommerce::User *user() const {
    return GetPointer<const flatbuf::ecommerce::User *>(VT_USER);
  }
  const ::flatbuffers::String *permanent_url() const {
    return GetPointer<const ::flatbuffers::String *>(VT_PERMANENT_URL);
  }
  const ::flatbuffers::String *query() const {
    return GetPointer<const ::flatbuffers::String *>(VT_QUERY);
  }
  uint32_t page() const {
    return GetField<uint32_t>(VT_PAGE, 0);
  }
  uint32_t total_pages() const {
    return GetField<uint32_t>(VT_TOTAL_PAGES, 0);
  }
  uint32_t results_per_page() const {
    return GetField<uint32_t>(VT_RESULTS_PER_PAGE, 0);
  }
  uint32_t total_results() const {
    return GetField<uint32_t>(VT_TOTAL_RESULTS, 0);
  }
  const ::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::ecommerce::Product>> *products() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::ecommerce::Product>> *>(VT_PRODUCTS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_USER) &&
           verifier.VerifyTable(user()) &&
           VerifyOffset(verifier, VT_PERMANENT_URL) &&
           verifier.VerifyString(permanent_url()) &&
           VerifyOffset(verifier, VT_QUERY) &&
           verifier.VerifyString(query()) &&
           VerifyField<uint32_t>(verifier, VT_PAGE, 4) &&
           VerifyField<uint32_t>(verifier, VT_TOTAL_PAGES, 4) &&
           VerifyField<uint32_t>(verifier, VT_RESULTS_PER_PAGE, 4) &&
           VerifyField<uint32_t>(verifier, VT_TOTAL_RESULTS, 4) &&
           VerifyOffset(verifier, VT_PRODUCTS) &&
           verifier.VerifyVector(products()) &&
           verifier.VerifyVectorOfTables(products()) &&
           verifier.EndTable();
  }
};

struct PageBuilder {
  typedef Page Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_user(::flatbuffers::Offset<flatbuf::ecommerce::User> user) {
    fbb_.AddOffset(Page::VT_USER, user);
  }
  void add_permanent_url(::flatbuffers::Offset<::flatbuffers::String> permanent_url) {
    fbb_.AddOffset(Page::VT_PERMANENT_URL, permanent_url);
  }
  void add_query(::flatbuffers::Offset<::flatbuffers::String> query) {
    fbb_.AddOffset(Page::VT_QUERY, query);
  }
  void add_page(uint32_t page) {
    fbb_.AddElement<uint32_t>(Page::VT_PAGE, page, 0);
  }
  void add_total_pages(uint32_t total_pages) {
    fbb_.AddElement<uint32_t>(Page::VT_TOTAL_PAGES, total_pages, 0);
  }
  void add_results_per_page(uint32_t results_per_page) {
    fbb_.AddElement<uint32_t>(Page::VT_RESULTS_PER_PAGE, results_per_page, 0);
  }
  void add_total_results(uint32_t total_results) {
    fbb_.AddElement<uint32_t>(Page::VT_TOTAL_RESULTS, total_results, 0);
  }
  void add_products(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::ecommerce::Product>>> products) {
    fbb_.AddOffset(Page::VT_PRODUCTS, products);
  }
  explicit PageBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Page> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Page>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Page> CreatePage(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<flatbuf::ecommerce::User> user = 0,
    ::flatbuffers::Offset<::flatbuffers::String> permanent_url = 0,
    ::flatbuffers::Offset<::flatbuffers::String> query = 0,
    uint32_t page = 0,
    uint32_t total_pages = 0,
    uint32_t results_per_page = 0,
    uint32_t total_results = 0,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::ecommerce::Product>>> products = 0) {
  PageBuilder builder_(_fbb);
  builder_.add_products(products);
  builder_.add_total_results(total_results);
  builder_.add_results_per_page(results_per_page);
  builder_.add_total_pages(total_pages);
  builder_.add_page(page);
  builder_.add_query(query);
  builder_.add_permanent_url(permanent_url);
  builder_.add_user(user);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Page> CreatePageDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<flatbuf::ecommerce::User> user = 0,
    const char *permanent_url = nullptr,
    const char *query = nullptr,
    uint32_t page = 0,
    uint32_t total_pages = 0,
    uint32_t results_per_page = 0,
    uint32_t total_results = 0,
    const std::vector<::flatbuffers::Offset<flatbuf::ecommerce::Product>> *products = nullptr) {
  auto permanent_url__ = permanent_url ? _fbb.CreateString(permanent_url) : 0;
  auto query__ = query ? _fbb.CreateString(query) : 0;
  auto products__ = products ? _fbb.CreateVector<::flatbuffers::Offset<flatbuf::ecommerce::Product>>(*products) : 0;
  return flatbuf::ecommerce::CreatePage(
      _fbb,
      user,
      permanent_url__,
      query__,
      page,
      total_pages,
      results_per_page,
      total_results,
      products__);
}

struct PageList FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef PageListBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PAGES = 4
  };
  const ::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::ecommerce::Page>> *pages() const {
    return GetPointer<const ::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::ecommerce::Page>> *>(VT_PAGES);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_PAGES) &&
           verifier.VerifyVector(pages()) &&
           verifier.VerifyVectorOfTables(pages()) &&
           verifier.EndTable();
  }
};

struct PageListBuilder {
  typedef PageList Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_pages(::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::ecommerce::Page>>> pages) {
    fbb_.AddOffset(PageList::VT_PAGES, pages);
  }
  explicit PageListBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<PageList> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<PageList>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<PageList> CreatePageList(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<::flatbuffers::Offset<flatbuf::ecommerce::Page>>> pages = 0) {
  PageListBuilder builder_(_fbb);
  builder_.add_pages(pages);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<PageList> CreatePageListDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<::flatbuffers::Offset<flatbuf::ecommerce::Page>> *pages = nullptr) {
  auto pages__ = pages ? _fbb.CreateVector<::flatbuffers::Offset<flatbuf::ecommerce::Page>>(*pages) : 0;
  return flatbuf::ecommerce::CreatePageList(
      _fbb,
      pages__);
}

inline const flatbuf::ecommerce::PageList *GetPageList(const void *buf) {
  return ::flatbuffers::GetRoot<flatbuf::ecommerce::PageList>(buf);
}

inline const flatbuf::ecommerce::PageList *GetSizePrefixedPageList(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<flatbuf::ecommerce::PageList>(buf);
}

inline bool VerifyPageListBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<flatbuf::ecommerce::PageList>(nullptr);
}

inline bool VerifySizePrefixedPageListBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<flatbuf::ecommerce::PageList>(nullptr);
}

inline void FinishPageListBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<flatbuf::ecommerce::PageList> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedPageListBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<flatbuf::ecommerce::PageList> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace ecommerce
}  // namespace flatbuf

#endif  // FLATBUFFERS_GENERATED_ECOMMERCE_FLATBUF_ECOMMERCE_H_
