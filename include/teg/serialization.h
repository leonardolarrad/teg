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

#ifndef TEG_SERIALIZATION_H
#define TEG_SERIALIZATION_H

#include "teg/schema.h"
#include "teg/encoder.h"
#include "teg/version.h"

namespace teg {

///  \brief Serializes the provided objects into the specified buffer using a binary format.
///  
///  \tparam  Opt   The options for serialization.
///  \tparam  ...T  The types of the objects to serialize.
///  \tparam  Buf   The type of the buffer to serialize into.
///  
///  \param  output_buffer  The buffer to serialize the data into.
///  \param  objs...        The objects to be serialized.
///  
///  \return An error code indicating the success or failure of the serialization process.
///  
///  \example 
///  \code
///      teg::byte_buffer buffer{};
///      auto const result = teg::serialize(buffer, "A string!", 55, 9.99f);
///      
///      if (success(result)) { 
///          // Use the serialized bytes
///          for (auto const& byte : buffer) {
///              std::cout << static_cast<int>(byte); 
///          }
///      }
///      else { 
///          // Handle the error.
///          std::cout << result.message(); 
///      }
///  \endcode
///  
template <options Opt = default_mode, class Buf, class... T>
requires (concepts::byte_buffer<Buf>) 
      && (concepts::serializable<T> && ...)
TEG_NODISCARD TEG_INLINE constexpr auto serialize(Buf& output_buffer, T const&... data) -> error {
    try {
        constexpr auto magic_word = teg::magic_word();
        constexpr auto data_format_options = Opt;
        constexpr auto header_format_options = options::little_endian | options::container_size_1b;
        constexpr auto schema_hash_table = teg::schema_hash_table<std::remove_cvref_t<T>...>();
        
        using buffer_t = Buf;
        using writer_t = buffer_writer<buffer_safety_policy::unsafe, buffer_t>;
        using header_encoder_t = encoder<header_format_options, writer_t>;
        using data_encoder_t = encoder<data_format_options, writer_t>;
        
        auto constexpr header_size = header_encoder_t::encoded_size(
            magic_word, data_format_options, u8{}, schema_hash_table
        );             
        auto data_size = data_encoder_t::encoded_size(data...);
        auto archive_size = header_size + data_size;
        
        if constexpr (concepts::resizable_container<Buf>) {                
            // Resize the buffer if needed.
            output_buffer.resize(archive_size);
        }
        else {
            // Otherwise, check buffer space.
            if (output_buffer.size() < archive_size) TEG_UNLIKELY {
                return error { std::errc::no_buffer_space };
            }
        }
        
        // Encode the header.
        auto header_encoder = header_encoder_t{ writer_t{ output_buffer } };
        auto result = header_encoder.encode(
            magic_word,
            data_format_options,
            static_cast<u8>(schema_hash_table.size()),
            schema_hash_table
        );

        if (failure(result)) TEG_UNLIKELY {
            if constexpr (concepts::resizable_container<Buf>) {
                output_buffer.clear();
            }
            return result;
        }

        // Encode the data.
        auto data_encoder = data_encoder_t { writer_t { output_buffer, header_size } };
        result = data_encoder.encode(data...);

        if (failure(result)) TEG_UNLIKELY {
            if constexpr (concepts::resizable_container<Buf>) {
                output_buffer.clear();
            }
        }

        return result;
    }
    catch (...) {
        if constexpr (concepts::resizable_container<Buf>) {
            output_buffer.clear();
        }

        return error { std::errc::interrupted };
    }
}

template <options Opt = default_mode, class... T> requires (concepts::serializable<T> && ...)
TEG_NODISCARD TEG_INLINE constexpr auto serialize(std::ostream& output_stream, T const&... objs) -> error {

    try {
        constexpr auto options = Opt;
        using encoder_t = encoder<options, stream_writer>;
        return encoder_t{ stream_writer{ output_stream } }.encode(objs...);
    }
    catch (...) {
        return error { std::errc::interrupted };
    }

}

} // namespace teg

#endif // TEG_SERIALIZATION_H
