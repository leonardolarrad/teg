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

#ifndef TEG_DECODER_H
#define TEG_DECODER_H

#include "teg/encoder.h"

namespace teg {

template <buffer_safety_policy policy = buffer_safety_policy::safe, class Buf = byte_array> 
    requires (concepts::byte_buffer<Buf>)
class buffer_reader {
public:
    ///  \brief The byte type used by the buffer.
    ///
    using byte_type = std::remove_const_t<typename std::remove_cvref_t<Buf>::value_type>;
    
    ///  Disable default constructor.
    TEG_INLINE constexpr buffer_reader() = delete;

    ///  \brief Constructs a new buffer writer.
    ///  \param  buffer  The buffer to write into.
    ///
    TEG_INLINE constexpr buffer_reader(Buf& buffer) noexcept : m_buffer(buffer), m_position(0) {}
    TEG_INLINE constexpr buffer_reader(Buf&& buffer) noexcept : m_buffer(buffer), m_position(0) {}

    ///  \brief Writes a contiguous array of bytes into the buffer.
    ///  
    ///  \param  data  A pointer to the data to be written from.
    ///  \param  size  The number of bytes to be written.
    ///
    ///  \details Handles endian-swapping if needed.
    ///
    template <bool swap_endian = false>
    TEG_NODISCARD TEG_INLINE constexpr auto read_bytes(byte_type* data, std::size_t size) -> error {

        if constexpr (policy == buffer_safety_policy::safe) {
            if (m_position + size > m_buffer.size()) {
                return error { std::errc::no_buffer_space };
            }
        }

        if constexpr (!swap_endian) {
            if (std::is_constant_evaluated()) {
                std::copy(m_buffer.data() + m_position, m_buffer.data() + m_position + size, data);
                m_position += size;
                return {};
            }
            else {
                std::memcpy(data, m_buffer.data() + m_position, size);
                m_position += size;
                return {};
            }
        }
        else {
            std::reverse_copy(m_buffer.data() + m_position, m_buffer.data() + m_position + size, data);
            m_position += size;
            return {};
        }

    }

private:
    Buf& m_buffer;
    u64 m_position;
};

///  \brief A binary deserializer.
///  
///  Decodes data from a given buffer using a binary format.
///  
///  \tparam  Buf  The type of the buffer to deserialize from.
///  \tparam  Opt  The options for deserialization.
///  
template <options Opt, class Reader>
class decoder {
public:
    ///  \brief The options used by the decoder.
    static constexpr auto options = Opt;

    ///  \brief The type used to read from source.
    using reader_type = std::remove_cvref_t<Reader>;

    ///  \brief The type used to represent a byte.
    ///  \see `teg::concepts::byte`
    using byte_type = typename reader_type::byte_type;
    
    ///  \brief The type used to represent the size of the container being serialized.
    using container_size_type = get_container_size_type<Opt>;

    ///  \brief The type used to represent the index of the variant being serialized.
    using variant_index_type = get_variant_index_type<Opt>;

    ///  \brief The buffer's maximum in-memory size.
    static constexpr u64 allocation_limit = get_allocation_limit<Opt>();

    ///  \brief The maximum size of elements a container can have.
    static constexpr u64 max_container_size = std::numeric_limits<container_size_type>::max();

    ///  \brief The maximum number of alternatives a variant can have.
    static constexpr u64 max_variant_index = std::numeric_limits<variant_index_type>::max();

    decoder() = delete; // Non-default-initializable and non-copyable.
    decoder(decoder const&) = delete;
    decoder& operator=(decoder const&) = delete;

    ///  \brief Construct a new binary deserializer.
    ///  \param buffer The buffer to deserialize from.
    ///  
    TEG_INLINE constexpr explicit decoder(reader_type & reader)  : m_reader(reader) {}
    TEG_INLINE constexpr explicit decoder(reader_type && reader) : m_reader(reader) {}

    template <class... T> requires (concepts::deserializable<T> && ...)
    TEG_NODISCARD TEG_INLINE constexpr auto decode(T&... objs) -> error {        

        if constexpr (sizeof...(objs) == 0) {
            return {};
        }        

        return decode_many(objs...);

    }

private:
    ///  \brief Deserializes the given objects.
    ///  
    template <class T0, class... TN>
    TEG_NODISCARD TEG_INLINE constexpr auto decode_many(T0& first_obj, TN&... remaining_objs) -> error {        
        if (auto const result = decode_one(first_obj); failure(result)) [[unlikely]] {
            return result;
        }

        return decode_many(remaining_objs...);
    }

    ///  \brief Case where there is no more objects to deserialize.
    ///  
    TEG_NODISCARD TEG_INLINE constexpr auto decode_many() -> error {
        return {};
    }
    
    ///  \brief Deserializes the given trivially-serializable object.
    ///  
    template <class T> requires (concepts::trivially_deserializable<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& trivial) -> error {
        
        constexpr bool swap_endian = concepts::endian_swapping_required<T, Opt>;

        if (std::is_constant_evaluated()) {
            // Compile-time decoding.
            using trivial_t = std::remove_cvref_t<T>;
            using src_bytes_t = std::array<std::remove_cv_t<byte_type>, sizeof(trivial_t)>;

            src_bytes_t src_bytes{};
            auto const result = m_reader.template read_bytes<swap_endian>(src_bytes.data(), src_bytes.size());

            if (failure(result)) [[unlikely]] {
                return result;
            }
            
            if constexpr (!concepts::c_array<trivial_t>) {
                // The object being deserialized is not a c-array and we can simply `bit-cast` its content.
                trivial = std::bit_cast<trivial_t>(src_bytes);
                return {};
            }
            else {
                // The object being deserialized is a c-array and can have multiple dimensions (rank can be > 1).
                using c_array_t = trivial_t;
                using element_t = std::remove_cvref_t<std::remove_all_extents_t<c_array_t>>;
                using tmp_array_t = std::array<element_t, sizeof(c_array_t) / sizeof(element_t)>;

                // In C++ functions can not return c-arrays, and therefore, std::bit_cast can not be used directly.
                // Instead, we have to create a temporary array and then copy its elements one by one.
                tmp_array_t tmp_array = std::bit_cast<tmp_array_t>(src_bytes);
                copy_md_c_array(trivial, tmp_array);                

                return {};
            }
        }
        else {
            // Run-time decoding.
            return m_reader.template read_bytes<swap_endian>(
                reinterpret_cast<byte_type*>(&trivial), sizeof(T));
        }

    }

    ///  \brief Deserializes the given trivially-serializable object.
    ///  
    template <class T> requires (concepts::trivially_serializable_container<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& trivial_container) -> error {

        using container_t = std::remove_cvref_t<T>;
        using element_t = typename container_t::value_type;

        if (std::is_constant_evaluated()) {
            // Compile-time decoding.
            using src_bytes_t = std::array<std::remove_cv_t<byte_type>, sizeof(container_t)>;

            src_bytes_t src_bytes{};
            auto const result = m_reader.read_bytes(src_bytes.data(), std::tuple_size_v<container_t> * sizeof(element_t));

            if (failure(result)) [[unlikely]] {
                return result;
            }
            
            trivial_container = std::bit_cast<container_t>(src_bytes);
            return {};
        }
        else {
            // Run-time decoding.
            return m_reader.read_bytes(
                reinterpret_cast<byte_type*>(&trivial_container), 
                std::tuple_size_v<container_t> * sizeof(element_t)
            );
        }
    }

    ///  \brief Deserializes the given aggregate.
    ///  
    template <class T>
        requires (concepts::serializable_aggregate<T>)
             && (!concepts::trivially_deserializable<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& aggregate) -> error {

        return visit_members(
            [&](auto&&... members) TEG_INLINE_LAMBDA {
                return decode_many(members...);
            },
            aggregate
        );
    }

    ///  \brief Deserializes a bounded c-array or a fixed-size container.
    ///  
    template <class T>
        requires (concepts::serializable_c_array<T> 
              || (concepts::serializable_container<T> && concepts::fixed_size_container<T>))
              && (!concepts::trivially_deserializable<T, Opt>)
              && (!concepts::trivially_serializable_container<T, Opt>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& array) -> error {

        // Deserialize the array's elements.       
        for (auto& element : array) {
            if (auto const result = decode_one(element); failure(result)) [[unlikely]] {
                return result;
            }
        }

        return {};
    }

    ///  \brief Deserialize a container.
    ///  
    ///  \details This function handles the deserialization of most container types, 
    ///  including contiguous containers and associative containers with a generic approach.
    ///  
    template <class T> 
        requires (concepts::serializable_container<T>)
              && (!concepts::fixed_size_container<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& container) -> error {
        
        using container_type = std::remove_reference_t<T>;
        using element_type = typename container_type::value_type;

        // Deserialize the size.
        container_size_type size;
        if (auto const result = decode_one(size); failure(result)) [[unlikely]] {
            return result;
        }

        // Deserialize the elements.
        if constexpr (
               concepts::contiguous_container<container_type>
            && concepts::resizable_container<container_type> 
            && concepts::trivially_serializable<element_type, Opt>
        ) {
            // Optimized path: memory copy elements.
            container.resize(size);

            return m_reader.read_bytes(
                reinterpret_cast<byte_type*>(container.data()), 
                size * sizeof(element_type));
        }
        else if constexpr (
            concepts::resizable_container<container_type>
        ) {
            container.resize(size);
            for (auto& element : container) {
                if (auto const result = decode_one(element); failure(result)) [[unlikely]] {
                    return result;
                }
            }
            return {};
        }
        else {
            // Unoptimized path: deserialize elements one by one.
            // Select the most suitable method to emplace the elements based on the container type.

            if constexpr (concepts::clearable_container<container_type>) {
                // Pre-condition: the container should be empty.
                container.clear();
            }

            if constexpr (concepts::reservable_container<container_type>) {
                // Optimization: pre-allocate uninitialized memory whenever possible.
                container.reserve(size);
            }

            if constexpr (concepts::map_container<container_type>) {
                // By default, `std::map` uses `std::pair<const key_type, mapped_type>` as its element_type.
                // We cannot deserialize const-qualified types, so we need to remove the const qualifiers.
                using key_value_element = std::pair<typename container_type::key_type, typename container_type::mapped_type>;
                key_value_element element;

                for (container_size_type i = 0; i < size; ++i) {                    
                    if (auto const result = decode_one(element); failure(result)) [[unlikely]] {
                        return result;
                    }
                    container.emplace(std::move(element));
                }
                return {};
            }
            else if constexpr (concepts::inplace_constructing_container<container_type>) {
                // Construct the elements and then move them into the container.
                element_type element;
                for (container_size_type i = 0; i < size; ++i) {
                    if (auto const result = decode_one(element); failure(result)) [[unlikely]] {
                        return result;
                    }
                    container.emplace(std::move(element));
                }
                return {};
            }
            else if constexpr (concepts::back_inplace_constructing_container<container_type>) {
                // Emplace the elements at the back of the container.
                for (container_size_type i = 0; i < size; ++i) {                
                    if (auto const result = decode_one(container.emplace_back()); failure(result)) [[unlikely]] {
                        return result;
                    }
                }
                return {};
            }
            else if constexpr (concepts::front_inplace_constructing_container<container_type>) {
                // Worst-case scenario: the container constructs its elements at the front of its storage.
                // We may need to reverse the container after deserialization.
                for (container_size_type i = 0; i < size; ++i) {
                    if (auto const result = decode_one(container.emplace_front()); failure(result)) [[unlikely]] {
                        return result;
                    }
                }

                if constexpr (concepts::invertible_container<container_type>) {
                    container.reverse();
                }
                return {};
            }
            else {
                static_assert(!sizeof(T), "Unsupported container container_type.");
            }
        }
    }

    ///  \brief Deserializes the given owning pointer.
    ///  
    template <class T> requires (concepts::serializable_owning_ptr<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& pointer) -> error {

        using type = std::remove_reference_t<T>;
        using element_type = typename type::element_type;

        // Deserialize the element.
        auto data = std::make_unique<element_type>();
        if (auto const result = decode_one(*data); failure(result)) [[unlikely]] {
            return result;
        }

        // Transfer ownership.
        pointer.reset(data.release());

        return {};
    }

    ///  \brief Deserializes the given optional.
    ///  
    template <class T> requires (concepts::serializable_optional<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& optional) -> error {

        using type = std::remove_reference_t<T>;
        using value_type = typename type::value_type;
        
        // Deserialize the `has_value` flag.
        byte_type has_value;    
        if (auto const result = decode_one(has_value); failure(result)) [[unlikely]] {
            return result;
        }

        // Check if there is a value to deserialize.
        if (!bool(has_value)) {
            optional = std::nullopt;
            return {};
        }

        // Deserialize the value.
        if (!optional.has_value()) {
            // The optional may have a default value. If not, we default-construct one.
            optional = value_type{};
        }

        return decode_one(*optional);
    }

    ///  \brief Deserializes the given tuple-like object. 
    ///
    template <class T> requires (concepts::serializable_tuple<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& tuple) -> error {

        return std::apply(
            [&](auto&&... elements) TEG_INLINE_LAMBDA {
                return decode_many(elements...);
            },
            tuple
        );
    }

    ///  \brief Deserializes the given variant.
    ///  
    template <class T> requires (concepts::serializable_variant<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& variant) -> error {

        using variant_type = std::remove_reference_t<T>;

        // Deserialize the index.
        variant_index_type runtime_index;
        if (auto const result = decode_one(runtime_index); failure(result)) [[unlikely]] {
            return result;
        }

        // Deserialize the alternative.
        constexpr std::size_t table_size = std::variant_size_v<variant_type>;

        if (runtime_index >= table_size) [[unlikely]] {
            return error { std::errc::invalid_argument };
        }
        
        return index_table_lookup<table_size>(runtime_index, [&](auto comptime_index) TEG_INLINE_LAMBDA {
            // Using an index table we can transform a runtime index into a compile-time index. 
            // With this technique we can then deserialize the variant alternative (based on the index)
            // at run-time.
            std::variant_alternative_t<comptime_index, variant_type> element;        
            if (auto const result = decode_one(element); failure(result)) [[unlikely]] {
                return result;
            }

            variant.template emplace<comptime_index>(std::move(element));
            return teg::error {};
        });
    }

    ///  \brief Deserializes the given user-defined serializable object.
    ///  
    template <class T> requires (concepts::user_defined_serialization<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& usr_obj) -> error {

        return usr_deserialize(
            [&](auto&&... objs) TEG_INLINE_LAMBDA {
                return decode_many(objs...);
            }, usr_obj);
    }

    template <class T> requires (concepts::serializable_compatible<T>)
    TEG_NODISCARD TEG_INLINE constexpr auto decode_one(T& compatible) -> error {
        
        using type = std::remove_reference_t<T>;
        using value_type = typename type::value_type;
        
        // Deserialize the `has_value` flag.
        byte_type has_value;    
        if (auto const result = decode_one(has_value); failure(result)) [[unlikely]] {
            // Cannot decode a single byte, in the case of compatible objects that means
            // that the value is not present in the stream.
            compatible = std::nullopt;
            return {};
        }

        // Check if there is a value to deserialize.
        if (!bool(has_value)) {
            return {};
        }

        // Deserialize the value.
        if (!compatible.has_value()) {
            // The optional may have a default value. If not, we default-construct one.
            compatible = value_type{};
        }
        
        // At this point, the stream should contain a valid value. If not,
        // the deserialization will fail.
        return decode_one(*compatible);
    }

private:
    reader_type m_reader;
};

} // namespace teg

#endif // TEG_DECODER_H
