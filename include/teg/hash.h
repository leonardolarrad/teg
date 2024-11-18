#include <span>

#include "teg/def.h"
#include "teg/buffer.h"

#define XXH_INLINE_ALL
#include "xxhash/xxhash.h"

namespace teg {

class xxhash {
public:

    template<concepts::byte T>
    teg_nodiscard teg_inline constexpr static auto hash64(std::span<const T> input) -> u64 {
        return XXH3_64bits(input.data(), input.size());
    }
};

} // namespace teg
