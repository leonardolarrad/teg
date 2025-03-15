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

#ifndef TEG_ENCODER_H
#define TEG_ENCODER_H

#include "teg/def.h"
#include "teg/alignment.h"
#include "teg/buffer.h"
#include "teg/container_concepts.h"
#include "teg/core_concepts.h"
#include "teg/serialization_concepts.h"
#include "teg/endian.h"
#include "teg/error.h"
#include "teg/members_visitor.h"
#include "teg/options.h"

namespace teg {

///  \brief Policy for whether or not to validate the buffer space before reading/writing.
///
enum class buffer_safety_policy {
    safe,   // Check if there is enough space in the buffer before reading/writing.
    unsafe  // Skip validation process.
};

template <buffer_safety_policy policy = buffer_safety_policy::safe, class Buf = byte_array> 
requires (concepts::byte_buffer<Buf>)
class buffer_writer {
public:
    ///  \brief The byte type used by the buffer.
    using byte_type = std::remove_const_t<typename std::remove_cvref_t<Buf>::value_type>;
    
    // Disable default constructor.
    TEG_INLINE constexpr buffer_writer() = delete;

    ///  \brief Constructs a new buffer writer.
    ///  \param  buffer  The buffer to write into.
    ///
    TEG_INLINE constexpr buffer_writer(Buf& buffer, u64 position = 0) noexcept : m_buffer(buffer), m_position(position) {}
    TEG_INLINE constexpr buffer_writer(Buf&& buffer, u64 position = 0) noexcept : m_buffer(buffer), m_position(position) {}

    ///  \brief Writes a contiguous array of bytes into the buffer.
    ///  
    ///  \param  data  A pointer to the data to be written from.
    ///  \param  size  The number of bytes to be written.
    ///
    ///  \details Handles endian-swapping if needed.
    ///
    template <bool SwapEndian = false>
    TEG_NODISCARD TEG_INLINE constexpr auto write_bytes(byte_type const* data, std::size_t size) -> error {
        if constexpr (policy == buffer_safety_policy::safe) {
            if (m_position + size > m_buffer.size()) {
                return error { std::errc::no_buffer_space };
            }
        }

        if constexpr (!SwapEndian) {
            if (std::is_constant_evaluated()) {
                std::copy(data, data + size, m_buffer.data() + m_position);
                m_position += size;
                return {};
            }
            else {
                std::memcpy(m_buffer.data() + m_position, data, size);
                m_position += size;
                return {};
            }
        }
        else {
            std::reverse_copy(data, data + size, m_buffer.data() + m_position);
            m_position += size;
            return {};
        }
    }

    TEG_NODISCARD TEG_INLINE constexpr auto position() -> u64 {
        return m_position;
    }

private:
    Buf& m_buffer;
    u64 m_position;
};

class stream_writer {
public:
    using byte_type = char;

    stream_writer() = delete;

    constexpr stream_writer(std::ostream& stream) noexcept : m_stream(stream) {}
    constexpr stream_writer(std::ostream&& stream) noexcept : m_stream(stream) {}
        
    template <bool SwapEndian = false>
    TEG_NODISCARD TEG_INLINE auto write_bytes(byte_type const* data, std::size_t size) -> error {

        if constexpr (SwapEndian) {
            m_stream.write(data, size);

            if (m_stream.fail()) TEG_UNLIKELY {
                return error { std::errc::io_error };
            }

            return {};
        }
        else {
            for (std::size_t i = 0; i < size; ++i) {
                m_stream.put(data[size - i - 1]);

                if (m_stream.fail()) TEG_UNLIKELY {
                    return error { std::errc::io_error };
                }
            }
            return {};
        }

    }

private:
    std::ostream& m_stream;
};

template <options Opt, class Writer>
class encoder {
public:
    ///  \brief The options used by the encoder.
    static constexpr auto options = Opt;

    ///  \brief The writer used by the encoder.
    using byte_type = typename Writer::byte_type;
        
    ///  \brief The type used to represent the size of the container being serialized.
    using container_size_type = get_container_size_type<Opt>;

    ///  \brief The type used to represent the index of the variant being serialized.
    using variant_index_type = get_variant_index_type<Opt>;

    ///  \brief The maximum size of elements a container can have.
    static constexpr u64 max_container_size = std::numeric_limits<container_size_type>::max();

    ///  \brief The maximum number of alternatives a variant can have.
    static constexpr u64 max_variant_index = std::numeric_limits<variant_index_type>::max();

    TEG_INLINE constexpr encoder(Writer& writer) : m_writer(writer) {}
    TEG_INLINE constexpr encoder(Writer&& writer) : m_writer(writer) {}

    ///  \brief Calculates the number of bytes needed to encode the given objects.
    ///  
    ///  \tparam ...T The types of the objects to serialize.
    ///  \param ...objs The objects to serialize.
    ///  \return The number of bytes needed to serialize the given objects.
    ///  
    ///  \example
    ///  \code
    ///      constexpr auto serialized_size = 
    ///         teg::binary_serializer::serialized_size("A string!", 55, 9.99f);
    ///      
    ///      static_assert(serialized_size == 22);
    ///  \endcode
    ///  
    template <class... T> requires (concepts::serializable<T> && ...)
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size(T const&... objs) -> u64 {
        return encoded_size_many(objs...);
    }

    TEG_NODISCARD TEG_INLINE constexpr auto writer() -> Writer const& {
        return m_writer;
    }

    ///  \brief Binary serialization of the given objects.
    ///  
    ///  Performs a binary serialization of the given objects using the provided buffer 
    ///  and returns an error code indicating whether the serialization was successful.
    ///  
    ///  \tparam ...T The types of the objects to serialize.    
    ///  \param ...objs The objects to serialize.
    ///  \return An error code indicating whether the serialization was successful.
    ///  
    ///  \example
    ///  \code
    ///      auto buffer = teg::byte_buffer{};
    ///      auto serializer = teg::binary_serializer{buffer};
    ///      auto const result = serializer.serialize("A string!", 55, 9.99f);
    ///      
    ///      if (success(result)) {
    ///          for (auto const& byte : buffer) {
    ///              std::cout << static_cast<int>(byte);
    ///          }
    ///      } else {
    ///          std::cout << result.message();
    ///      }
    ///  \endcode
    ///  
    ///  \details The binary serialization algorithm is implemented as a series of calls 
    ///  to `encode_many` and `encode_one` using overload resolution through concepts.
    ///  
    template <class... T> requires (concepts::serializable<T> && ...)
    TEG_NODISCARD TEG_INLINE constexpr auto encode(T const&... objs) -> error {        

        if constexpr (sizeof...(objs) == 0) {
            return {};
        }

        return encode_many(objs...);
    }

private:
    ///  \brief Calculates the encoding size of the given objects.
    ///  
    template <class T0, class... TN> 
    TEG_NODISCARD TEG_INLINE 
    static constexpr auto encoded_size_many(T0 const& first_obj, TN const&... remaining_objs) -> u64 {

        return encoded_size_one(first_obj) + encoded_size_many(remaining_objs...);
    }

    ///  \brief Case where theres no objects to encode.
    ///  
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_many() -> u64 {
        return 0;
    }

    ///  \brief Calculates the encoding size of the given trivially serializable type.
    ///  
    template <class T> 
    requires concepts::builtin<T>
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& trivial) -> u64 {        
        if constexpr (concepts::varint_cast_required<T, Opt>) {
            return encoded_size_one(select_integer_type<T, Opt>{ trivial });
        }
        else {
            return sizeof(trivial);
        }
    }

    ///  \brief Calculates the encoding size of the given aggregate.
    ///  
    template <class T> 
    requires concepts::match_aggregate<T>
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& aggregate) -> u64 {
        return visit_members(
            [&](auto&&... members) TEG_INLINE_LAMBDA {
                return encoded_size_many(members...);
            },
            aggregate
        );
    }

    ///  \brief Calculates the encoding size of the given c-array.
    ///  
    template <class T> 
    requires concepts::match_c_array<T> 
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& c_array) -> u64 {
        return std::size(c_array) * encoded_size_one(c_array[0]);
    }

    ///  \brief Calculates the encoding size of the given fixed-size container.
    ///  
    template <class T> 
    requires (concepts::match_container<T>)
          && (concepts::fixed_size_container<T>)
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& fixed_container) -> u64 {

        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;

        if constexpr (concepts::trivially_serializable<element_type, Opt>) {
            return sizeof(element_type) * fixed_container.size();
        } 
        else {
            u64 result = 0;
            for (auto const& element : fixed_container) {
                result += encoded_size_one(element);
            }
            return result;
        }
    }

    ///  \brief Calculates the encoding size of the given container.
    ///  
    template <class T> 
    requires (concepts::match_container<T>)
          && (!concepts::fixed_size_container<T>)
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& container) -> u64 {

        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;
        
        u64 const container_size = [&]() constexpr {
            if constexpr (concepts::sized_container<container_type>) {
                return static_cast<u64>(container.size());
            } else {
                return static_cast<u64>(std::distance(container.begin(), container.end()));
            }
        }();

        u64 result = encoded_size_one(static_cast<container_size_type>(container_size));

        if constexpr (concepts::trivially_serializable<element_type, Opt>) {
            result += sizeof(element_type) * container_size;
            return result;
        }
        else {
            for (auto const& element : container) {
                result += encoded_size_one(element);
            }
            return result;
        }
    }

    ///  \brief Calculates the encoding size of the given owning pointer.
    ///  
    template <class T> requires (concepts::match_owning_ptr<T>)
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& ptr) -> u64 {
        if (ptr == nullptr) {
            return 0;
        }    
        return encoded_size_one(*ptr);
    }

    ///  \brief Calculates the encoding size of the given optional.
    ///  
    template <class T> requires (concepts::match_optional<T>)
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& optional) -> u64 {    
        if (optional.has_value()) {
            return sizeof(byte_type) + encoded_size_one(optional.value());
        }
        else {
            return sizeof(byte_type);
        }
    }

    ///  \brief Calculates the encoding size of the given tuple-like object.
    ///  
    template <class T> requires (concepts::match_tuple<T>) 
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& tuple) -> u64 {    
        return std::apply(
            [](auto&&... elements) TEG_INLINE_LAMBDA {
                return encoded_size_many(elements...);
            },
            tuple
        );
    }

    ///  \brief Calculates the encoding size of the given variant.
    ///  
    template <class T> requires (concepts::match_variant<T>)
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& variant) -> u64 {
        const u64 index_size = sizeof(variant_index_type);
        const u64 element_size = std::visit(
            [](auto&& value) constexpr {
                return encoded_size_one(value);
            },
            variant
        );
        return index_size + element_size;
    }

    ///  \brief Calculates the encoding size of a user-defined serializable type.
    ///  
    template <class T> requires (concepts::user_defined_serialization<T>)
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& usr_obj) -> u64 {
        return usr_serialized_size(
            [&](auto&&... objs) constexpr {
                return encoded_size_many(objs...);
            }, usr_obj);
    }

    ///  \brief Calculates the encoding size of the given compatible type.
    ///
    template <class T> requires (concepts::match_compatible<T>)
    TEG_NODISCARD TEG_INLINE static constexpr auto encoded_size_one(T const& compatible) -> u64 {
        if (compatible.has_value()) {
            return sizeof(byte_type) + encoded_size_one(compatible.value());
        }
        else {
            return sizeof(byte_type);
        }
    }

    ///  \brief Serializes the given objects.
    ///  
    template <class T0, class... TN> 
    TEG_NODISCARD TEG_INLINE 
    constexpr auto encode_many(T0 const& first_obj, TN const&... remaining_objs) -> error {        
        if (auto const result = encode_one(first_obj); failure(result)) TEG_UNLIKELY {
            return result;
        }

        return encode_many(remaining_objs...);
    }

    ///  \brief Serialize zero objects.
    ///  
    TEG_NODISCARD TEG_INLINE constexpr auto encode_many() -> error {
        return {};
    }

    ///  \brief Serialize a trivially serializable object.
    ///  
    template <class T>
        requires (concepts::trivially_serializable<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& trivial) -> error {

        if constexpr (concepts::varint_cast_required<T, Opt>) {
            return encode_one(select_integer_type<T, Opt>{ trivial });
        }
        else {
            constexpr bool swap_endian = concepts::endian_swapping_required<T, Opt>;

            if (std::is_constant_evaluated()) {
                // Compile-time encoding.
                auto const src_bytes = std::bit_cast<std::array<byte_type, sizeof(T)>>(trivial);

                return m_writer.template write_bytes<swap_endian>(
                    src_bytes.data(), src_bytes.size()); 
            }
            else {
                // Run-time encoding.
                return m_writer.template write_bytes<swap_endian>(
                    reinterpret_cast<byte_type const*>(&trivial), sizeof(T));
            }
        }
    }
   
    ///  \brief Serialize a trivially serializable container.
    ///  
    template <class T> requires (concepts::trivially_serializable_container<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& container) -> error {
        
        auto const src_bytes = std::bit_cast<std::array<byte_type, sizeof(T)>>(container);

        return m_writer.write_bytes(
                src_bytes.data(), 
                std::tuple_size_v<std::remove_cvref_t<decltype(container)>> * sizeof(typename T::value_type)); 

    }

    ///  \brief Serialize the given aggregate.
    ///  
    template <class T>
        requires (concepts::match_aggregate<T>)
              && (!concepts::trivially_serializable<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& aggregate) -> error {

        return visit_members(
            [&](auto&&... members) {
                return encode_many(members...);
            },
            aggregate
        );

    }

    ///  \brief Serialize the given bounded c-array.
    ///  
    template <class T> 
        requires (concepts::match_c_array<T>) 
              && (!concepts::trivially_serializable<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& c_array) -> error {

        // Serialize elements.            
        for (auto const& element : c_array) {
            if (auto const result = encode_one(element); failure(result)) TEG_UNLIKELY {
                return result;
            }
        }
        
        return {};

    }

    ///  \brief Serialize a container.
    ///  
    ///  \details This function handles the serialization all container types, 
    ///  including fixed-size containers, contiguous containers, and associative containers, 
    ///  in a generic way.
    ///  
    template <class T> 
        requires (concepts::match_container<T>)
              && (!concepts::trivially_serializable_container<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& container) -> error {

        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;
        using native_size_type = typename container_type::size_type;
        
        // Serialize the container's size if needed.
        if constexpr (!concepts::fixed_size_container<container_type>) {

            // For fixed-size containers the size is known at compile-time; therefore, no serialization
            // is needed. Otherwise, serialize the container's size.
            
            if constexpr (concepts::sized_container<container_type>) {
                // Optimized path: we don't need to compute the size; 
                // it is already known at run-time.
                native_size_type const size = container.size();

                if (size > max_container_size) TEG_UNLIKELY {
                    return error { std::errc::value_too_large };
                }

                auto const result = encode_one(static_cast<container_size_type>(size));
                if (failure(result)) TEG_UNLIKELY {
                    return result;
                }
            }
            else {
                // Non-optimized path: some containers (like `std::forward_list`) 
                // don't have a `size()` observer.
                native_size_type const size = std::distance(container.begin(), container.end());
                
                if (size > max_container_size) TEG_UNLIKELY {
                    return error { std::errc::value_too_large };
                }

                auto const result = encode_one(static_cast<container_size_type>(size));
                if (failure(result)) TEG_UNLIKELY {
                    return result;
                }
            }
        }

        // Serialize the container's elements.
        if constexpr (
            concepts::contiguous_container<container_type> &&
            concepts::trivially_serializable<element_type, Opt>
        ) {
            // Optimized path: memory copy elements.            
            return m_writer.write_bytes(
                reinterpret_cast<byte_type const*>(container.data()),
                container.size() * sizeof(element_type)
            );
        }
        else {
            // Non-optimized path: serialize each element one by one.
            for (auto const& element : container) {
                if (auto const result = encode_one(element); failure(result)) TEG_UNLIKELY {
                    return result;
                }
            }

            return {};
        }

    }

    ///  \brief Serializes a given owning pointer.
    ///  
    template <class T> requires (concepts::match_owning_ptr<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& ptr) -> error  {

        if (ptr == nullptr) TEG_UNLIKELY {
            return std::errc::invalid_argument;
        }

        return encode_one(*ptr);

    }

    ///  \brief Serializes the given optional.
    ///  
    template <class T> requires (concepts::match_optional<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& optional) -> error {

        if (!optional.has_value()) TEG_UNLIKELY {
            return encode_one(byte_type(false));
        } else {
            return encode_many(byte_type(true), *optional);
        }

    }

    ///  \brief Serializes the given tuple-like object. 
    ///  
    template <class T> requires (concepts::match_tuple<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& tuple) -> error {    

        return std::apply(
            [&](auto&&... elements) TEG_INLINE_LAMBDA {
                return encode_many(elements...);
            },
            tuple
        );

    }

    ///  \brief Serializes the given variant.
    ///  
    template <class T> requires (concepts::match_variant<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& variant) -> error {

        // Check valueless by exception.
        if (variant.valueless_by_exception()) TEG_UNLIKELY {
            return error { std::errc::invalid_argument };
        }

        // Serialize the index.
        auto const result = encode_one(static_cast<variant_index_type>(variant.index()));
        if (failure(result)) TEG_UNLIKELY {
            return result;        
        }    

        // Serialize the value.
        return std::visit(
            [&](auto&& value) TEG_INLINE_LAMBDA {
                return encode_one(value);
            },
            variant
        );

    }

    ///  \brief Serializes the given user-defined serializable object.
    ///  
    template <class T> requires (concepts::user_defined_serialization<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& usr_obj) -> error {

        return usr_serialize(
            [&](auto&&... objs) constexpr {
                return encode_many(objs...);
            }, 
            usr_obj
        );

    }

    template <class T> requires (concepts::match_compatible<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto encode_one(T const& compatible) -> error {

        if (!compatible.has_value()) TEG_UNLIKELY {
            return encode_one(byte_type(false));
        }
        else {
            return encode_many(byte_type(true), *compatible);
        }

    }

    Writer m_writer;
};

} // namespace teg

#endif // TEG_ENCODER_H
