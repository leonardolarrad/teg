///  Copyright (c) 2024 Adrian & Leonardo Larrad.
///  
///  This software is provided 'as-is', without any express or implied warranty. 
///  In no event will the authors be held liable for any damages arising from
///  the use of this software.
///  
///  Permission is granted to anyone to use this software for any purpose,including
///  commercial applications, and to alter it and redistribute it freely, subject
///  to the following restrictions:
///  
///  1. The origin of this software must not be misrepresented; you must not claim
///     that you wrote the original software. If you use this software in a product, 
///     an acknowledgment in the product documentation would be appreciated but is 
///     not required.
///  2. Altered source versions must be plainly marked as such, and must not be
///     misrepresented as being the original software.
///  3. This notice may not be removed or altered from any source distribution.

#ifndef TEG_DESERIALIZATION_H
#define TEG_DESERIALIZATION_H

#include "teg/serialization.h"
#include "teg/decoder.h"

namespace teg {

template <options Opt = default_mode, class Buf, class... T>
    requires (concepts::byte_buffer<Buf>) && (concepts::deserializable<T> && ...)
TEG_NODISCARD TEG_INLINE constexpr auto deserialize(Buf& input_buffer, T&... objs) -> error {

    constexpr auto host_magic_word = teg::magic_word();
    constexpr auto host_data_opts = Opt;
    constexpr auto host_header_opts = options::little_endian | options::container_size_1b;
    constexpr auto host_schema_table = teg::schema_hash_table<T...>();

    using buffer_t = Buf;
    using buffer_reader_t = buffer_reader<buffer_safety_policy::safe, buffer_t>;
    using header_decoder_t = decoder<host_header_opts, buffer_reader_t>;
    using data_decoder_t = decoder<host_data_opts, buffer_reader_t>;

    auto header_decoder = header_decoder_t{ buffer_reader_t { input_buffer } };

    // Decode the header.
    u32 client_magic_word;
    options client_opts;
    u8 client_schema_table_size;

    auto result = header_decoder.decode(
        client_magic_word, 
        client_opts,
        client_schema_table_size         
    );

    if (failure(result)) {
        return result;
    }

    // Verify magic word and options.
    if ((client_magic_word != host_magic_word) || (client_opts != host_data_opts)) {
        return error { std::errc::protocol_error };            
    }
    
    // Verify schema.
    for (u8 i = 0; i < client_schema_table_size; ++i) {
        u32 client_schema_hash;
        
        if (result = header_decoder.decode(client_schema_hash); failure(result)) TEG_UNLIKELY {
            return result;
        }

        if ((i < host_schema_table.size()) && (client_schema_hash != host_schema_table[i])) TEG_UNLIKELY {
            return error { std::errc::protocol_error };
        }
    }

    // Decode the data.
    auto data_decoder = data_decoder_t{ buffer_reader_t { input_buffer, header_decoder.reader().position() }}; 
    return data_decoder.decode(objs...);    
}

template <options Opt = default_mode, class... T> requires (concepts::serializable<T> && ...)
TEG_NODISCARD TEG_INLINE constexpr auto deserialize(std::istream& input_stream, T&... objs) -> error {

    try {
        constexpr auto options = Opt;
        using decoder_t = decoder<options, stream_reader>;
        return decoder_t{ stream_reader{ input_stream } }.decode(objs...);
    }
    catch (...) {
        return error { std::errc::interrupted };
    }

}

} // namespace teg

#endif // TEG_DESERIALIZATION_H 
