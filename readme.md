# TEG
This repository contains the source code for the undergraduate thesis submitted in partial fulfillment of the requirements for the degree of Software Engineer at Andres Bello Catholic University (UCAB). You can download a copy of the thesis document [here](docs/thesis.pdf).

```cpp
#include <teg/teg.h>

teg::byte_array archive;
teg::serialize(archive, u8"¡Hola mundo!");
```

## Abstract

Serialization is the process that converts structured data into a persistent format for storage or transmission, while deserialization restores it to its original form. In C++, the absence of a standard reflection mechanism has limited the capabilities of serialization libraries, forcing developers to implement ad-hoc solutions based on code generation, preprocessor macros, and repetitive, error-prone boilerplate code. This increases complexity, reduces maintainability, and compromises software portability.

However, features introduced in the ISO/IEC 14882:2020 standard (C++20) enable the implementation of a novel compile-time reflection approach, creating opportunities to improve software developed in this language. This research aimed to develop a binary serialization library based on this approach. To achieve this, an incremental software methodology was used, beginning with the design of a portable binary format specification, followed by three increments: the Static Reflection Module, the Object Serialization Module, and the Variable-Length Integer Module.

Finally, performance tests demonstrated the high efficiency of the Teg library—the product of this work—in terms of data throughput, memory usage, CPU load, and file compression. On a 3.4 GHz Intel processor, Teg reported a maximum throughput of 21 GiB/s, performing up to 20 to 100 times faster than the most widely used industry libraries, such as Protocol Buffers, MessagePack, and Boost.Serialization.

*Keywords*: binary serialization, binary deserialization, compile-time reflection, C++20 programming language, ISO/IEC 14882:2020.

## Install

You would need to have [Meson](https://mesonbuild.com/) and a C++20 compatible compiler installed on your system. Inside the cloned repository, run the following commands:

```bash
# Setup the build dir `out`
meson setup out/ --buildtype=release

# Change dir
cd out/

# Configure `max_visit_members` (Optional)
meson configure -Dmax_visit_members=255

# Install
meson install
```

## How to use it

### Scheme

A valid data schema is any type modeling the `teg::concepts::serializable` concept via compile-time reflection, eliminating the need for external files or macros. In essence, any fundamental data type, aggregate, or array of these types constitutes a valid data schema.

```cpp
class User {
public:
    uint64_t id;
    std::string username;
    std::string email;
    uint64_t created_at;
    uint64_t modified_at;
    std::optional<std::array<uint8_t, 6>> sms_code;
};

assert(teg::concepts::serializable<User>);
```

### Serialization

```cpp
/// \file: server.cpp
#include "teg/teg.h"

// Define the schema.
class InvoiceLine {
public:
    std::string description;
    teg::u16 quantity;
    teg::f64 discount;
    teg::f64 price;
    teg::f64 subtotal;
};

class Invoice {
public:
    Date date = {};
    std::string customer = {};
    std::vector<InvoiceLine> lines = {};
    teg::f64 total = {};
};

// Serialize.
Invoice invoice{};
invoice.date = Date(2021, 10, 1);
invoice.customer = "John Doe";
invoice.lines.push_back(InvoiceLine{"Product 1", 2, 0.0, 50.0, 100.0});
invoice.lines.push_back(InvoiceLine{"Product 2", 1, 0.0, 50.0, 50.0});
invoice.lines.push_back(InvoiceLine{"Product 3", 1, 0.0, 50.0, 50.0});
invoice.total = 200.0;

teg::byte_array archive{};
teg::serialize(archive, server_invoice).or_throw();

// Do something with the archive, send it over the network or store it on disk.
send_to_client(archive);
...
```

### Deserialization

```cpp
/// \file: client.cpp
#include "teg/teg.h"

// Define the same schema as the server-side.
class InvoiceLine {
public:
    std::string description;
    teg::u16 quantity;
    teg::f64 discount;
    teg::f64 price;
    teg::f64 subtotal;
};

class Invoice {
public:
    Date date = {};
    std::string customer = {};
    std::vector<InvoiceLine> lines = {};
    teg::f64 total = {};
};

// Receive the data from server.
teg::byte_array archive = receive_from_server();

// Deserialize.
Invoice invoice{};
teg::deserialize(archive, client_invoice).or_throw();

// Do something with the invoice.
print(invoice);
...
```