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

///  This file is a rewrite of the original `boost::mp_with_index` source code,
///  originally authored by Peter Dimov. Distributed under the following license:
///
///  Boost Software License - Version 1.0 - August 17th, 2003
///  
///  Permission is hereby granted, free of charge, to any person or organization
///  obtaining a copy of the software and accompanying documentation covered by
///  this license (the "Software") to use, reproduce, display, distribute,
///  execute, and transmit the Software, and to prepare derivative works of the
///  Software, and to permit third-parties to whom the Software is furnished to
///  do so, all subject to the following:
///  
///  The copyright notices in the Software and this entire statement, including
///  the above license grant, this restriction and the following disclaimer,
///  must be included in all copies of the Software, in whole or in part, and
///  all derivative works of the Software, unless such copies or derivative
///  works are solely in the form of machine-executable object code generated by
///  a source language processor.
///  
///  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
///  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
///  FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
///  SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
///  FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
///  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
///  DEALINGS IN THE SOFTWARE.

#pragma once
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>
#include "unreachable.h"

namespace teg::internal {

template<std::size_t N> struct index_table
{
    template<std::size_t K, class F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        if (i < N/2) {
            return index_table<N/2>::template lookup<K>(i, std::forward<F>(f));
        }
        else {
            return index_table<N-N/2>::template lookup<K+N/2>(i - N/2, std::forward<F>(f));
        }
    }
};

template<> 
struct index_table<0> {};

template<> 
struct index_table<1>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
    }
};

template<> 
struct index_table<2>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<3>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2: return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<4>
{
    template<std::size_t K, class F> static constexpr auto lookup(std::size_t i, F && f )
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2: return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3: return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<5>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2: return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3: return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4: return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<6>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2: return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3: return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4: return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5: return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<7>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2: return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3: return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4: return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5: return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6: return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<8>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2: return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3: return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4: return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5: return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6: return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7: return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<9>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2: return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3: return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4: return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5: return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6: return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7: return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            case 8: return std::forward<F>(f)(std::integral_constant<std::size_t, K+8>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<10>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0: return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1: return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2: return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3: return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4: return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5: return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6: return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7: return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            case 8: return std::forward<F>(f)(std::integral_constant<std::size_t, K+8>());
            case 9: return std::forward<F>(f)(std::integral_constant<std::size_t, K+9>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<11>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            case 8:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+8>());
            case 9:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+9>());
            case 10: return std::forward<F>(f)(std::integral_constant<std::size_t, K+10>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<12>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            case 8:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+8>());
            case 9:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+9>());
            case 10: return std::forward<F>(f)(std::integral_constant<std::size_t, K+10>());
            case 11: return std::forward<F>(f)(std::integral_constant<std::size_t, K+11>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<13>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            case 8:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+8>());
            case 9:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+9>());
            case 10: return std::forward<F>(f)(std::integral_constant<std::size_t, K+10>());
            case 11: return std::forward<F>(f)(std::integral_constant<std::size_t, K+11>());
            case 12: return std::forward<F>(f)(std::integral_constant<std::size_t, K+12>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<14>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            case 8:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+8>());
            case 9:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+9>());
            case 10: return std::forward<F>(f)(std::integral_constant<std::size_t, K+10>());
            case 11: return std::forward<F>(f)(std::integral_constant<std::size_t, K+11>());
            case 12: return std::forward<F>(f)(std::integral_constant<std::size_t, K+12>());
            case 13: return std::forward<F>(f)(std::integral_constant<std::size_t, K+13>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<15>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            case 8:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+8>());
            case 9:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+9>());
            case 10: return std::forward<F>(f)(std::integral_constant<std::size_t, K+10>());
            case 11: return std::forward<F>(f)(std::integral_constant<std::size_t, K+11>());
            case 12: return std::forward<F>(f)(std::integral_constant<std::size_t, K+12>());
            case 13: return std::forward<F>(f)(std::integral_constant<std::size_t, K+13>());
            case 14: return std::forward<F>(f)(std::integral_constant<std::size_t, K+14>());
            default: unreachable();
        }
    }
};

template<> 
struct index_table<16>
{
    template<std::size_t K, typename F> 
    static constexpr auto lookup(std::size_t i, F&& f)
        -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
    {
        switch (i) {
            case 0:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+0>());
            case 1:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+1>());
            case 2:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+2>());
            case 3:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+3>());
            case 4:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+4>());
            case 5:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+5>());
            case 6:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+6>());
            case 7:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+7>());
            case 8:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+8>());
            case 9:  return std::forward<F>(f)(std::integral_constant<std::size_t, K+9>());
            case 10: return std::forward<F>(f)(std::integral_constant<std::size_t, K+10>());
            case 11: return std::forward<F>(f)(std::integral_constant<std::size_t, K+11>());
            case 12: return std::forward<F>(f)(std::integral_constant<std::size_t, K+12>());
            case 13: return std::forward<F>(f)(std::integral_constant<std::size_t, K+13>());
            case 14: return std::forward<F>(f)(std::integral_constant<std::size_t, K+14>());
            case 15: return std::forward<F>(f)(std::integral_constant<std::size_t, K+15>());
            default: unreachable();
        }
    }
};

} // namespace teg::internal

namespace teg {

template<std::size_t N, typename F> 
inline constexpr auto index_table_lookup(std::size_t i, F&& f)
    -> decltype(std::declval<F>()(std::declval<std::integral_constant<std::size_t, 0>>()))
{
    assert(i < N);
    return internal::index_table<N>::template lookup<0>(i, std::forward<F>(f));
}

} // namespace teg
