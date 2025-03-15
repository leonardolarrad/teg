#include "teg/teg.h"
#include <iostream>

#if 0

class Date {
public:
    Date() = default;
    Date(uint16_t year, uint16_t month, uint16_t day) 
        : m_year(year), m_month(month), m_day(day) {}

    auto get_year()  const -> uint16_t { return m_year; }
    auto get_month() const -> uint16_t { return m_month; }
    auto get_day()   const -> uint16_t { return m_day; }

    auto set_year(uint16_t year)   -> void { m_year = year; }
    auto set_month(uint16_t month) -> void { m_month = month; }
    auto set_day(uint16_t day)     -> void { m_day = day; }

    constexpr bool operator==(Date const&) const = default;

private:
    uint16_t  m_year;
    uint16_t  m_month;
    uint16_t  m_day;
};



template <class F>
constexpr auto usr_serialized_size(F&& serialized_size, Date const& date) -> teg::u64 {
    return serialized_size(teg::u16{}, teg::u16{}, teg::u16{});
}

template <class F>
constexpr auto usr_serialize(F&& encode, Date const& date) -> teg::error {
    auto const result = encode(
        (teg::u16) date.get_year(), 
        (teg::u16) date.get_month(), 
        (teg::u16) date.get_day());

    if (teg::failure(result)) {
        return result;        
    }

    return {}; // success
}

template <class F>
constexpr auto usr_deserialize(F&& decode, Date& date) -> teg::error {    
    struct { 
        teg::u16 year; 
        teg::u16 month; 
        teg::u16 day;
    } data;

    auto const result = decode(data.year, data.month, data.day);

    if (teg::failure(result)) {
        return result;        
    }

    date = Date(data.year, data.month, data.day);

    return {}; // success
}

constexpr auto usr_schema(Date const& date) -> decltype(auto) {
    return teg::make_fixed_string("exp::Date");
}


class InvoiceLine {
public:
    std::string description;
    teg::u16 quantity;
    teg::f64 discount;
    teg::f64 price;
    teg::f64 total;

    constexpr bool operator==(InvoiceLine const&) const = default;
};

struct Invoice {

    Date date = { 0, 0, 0 };
    std::string customer = {};
    std::vector<InvoiceLine> lines = {};
    teg::f64 total = {};

    constexpr bool operator==(Invoice const&) const = default;
    //Invoice() = default;
};


int main() {


constexpr teg::options custom_mode = 
    teg::options::big_endian | teg::options::force_varint;

teg::byte_array archive;
teg::serialize<custom_mode>(archive, std::string {"A very important text"}).or_throw();

std::string str;
teg::deserialize<custom_mode>(archive, str).or_throw();

return server_invoice == invoice2;
}

#endif


teg::byte_array serialize_number(teg::i32 number) {
    teg::byte_array buffer{};
    teg::serialize(buffer, number).or_throw();
    return buffer;
}

int main() {    

    auto buffer = serialize_number(99);

    return 0;
}