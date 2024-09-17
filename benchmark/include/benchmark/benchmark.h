// Copyright (c) 2024 Adrian & Leonardo Larrad.
// 
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
// 
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and mu    st not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
#pragma once
#include <concepts>
#include <chrono>
#include <cstring>
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <format>

namespace benchmarking {

struct stats {
    std::chrono::nanoseconds real_time;
    std::chrono::nanoseconds cpu_time;
    // cpu usage
    // ram usage
};

struct config {
    std::string benchmark_name;
    std::size_t warmup = 10;
    std::size_t iterations = 1;

    bool is_relative = false;
    bool auto_determine_iterations = true;
    std::size_t min_epoch_iterations = 1;
    std::chrono::nanoseconds max_epoch_time = std::chrono::milliseconds(100);
    std::chrono::nanoseconds min_epoch_time = std::chrono::milliseconds(1);
};

class benchmark {
public:
    benchmark() = default;
    benchmark(config const& config) = delete;
    benchmark(config && config) = delete;
    benchmark& operator=(config const& config) = delete;
    benchmark& operator=(config && config) = delete;

    benchmark& name(std::string const& name) {
        m_config.benchmark_name = name;
        return *this;
    }

    benchmark& warmup(std::size_t warmup) {
        m_config.warmup = warmup;
        return *this;
    }

    benchmark& iterations(std::size_t iterations) {
        m_config.iterations = iterations;
        return *this;
    }

    template <typename Func>
    benchmark& run(std::string benchmark_name, Func&& benchmark_func) {
        name(benchmark_name);
        return run(std::forward<Func>(benchmark_func));
    }

    template <typename Func>
    benchmark& run(Func&& benchmark_func) {
        // Warmup.
        for (int i = m_config.warmup; i > 0; --i) {
            benchmark_func();
        }

        // Benchmark.
        //std::vector<stats> results;
        std::size_t iterations = std::max(m_config.iterations, std::size_t(1));

        auto const start = std::chrono::steady_clock::now();
        for (std::size_t i = 0; i < iterations; ++i) {
            benchmark_func();
        }
        auto const end = std::chrono::steady_clock::now();
        
        auto const duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        double const real_time = duration.count() / double(iterations);
        double const cpu_time = duration.count() / double(iterations);

        // Output.
        std::cout << "Benchmark: " << m_config.benchmark_name << std::endl;
        std::cout << std::format("Real time: {:.3f} ns", real_time) << std::endl;
        std::cout << std::format("CPU time: {:.3f} ns", cpu_time) << std::endl;


        /*
        // Statistics.
        double total_execution_time = 0.0;
        double mean = 0.0;
        double stddev = 0.0;
        for (auto const& r : results) {
            total_execution_time += r.real_time.count();
            mean += r.real_time.count();
        }
        mean /= results.size();
        for (auto const& r : results) {
            stddev += (r.real_time.count() - mean) * (r.real_time.count() - mean);
        }
        stddev = std::sqrt(stddev / results.size());

        // Output.
        std::cout << "Benchmark: " << m_config.benchmark_name << std::endl;
        std::cout << std::format("Total execution time: {:.3f} ns", total_execution_time) << std::endl;
        std::cout << std::format("Mean execution time: {:.3f} ns", mean) << std::endl;
        std::cout << std::format("Standard deviation: {:.3f} ns", stddev) << std::endl;
        */
        return *this;       
    }

private:
    config m_config;
};

namespace {

#if defined(_MSC_VER)

# pragma optimize("", off)
void _msvc_dont_optimize_away(void const*) {
}
# pragma optimize("", on)

template <typename T>
void _dont_optimize_away(T const& t) {
    _msvc_dont_optimize_away(&t);
}

#else 

template <typename T>
void _dont_optimize_away(T const& val) {
    // NOLINTNEXTLINE(hicpp-no-assembler)
    asm volatile("" : : "r,m"(val) : "memory");
}

template <typename T>
void _dont_optimize_away(T& val) {
#if defined(__clang__)
    // NOLINTNEXTLINE(hicpp-no-assembler)
    asm volatile("" : "+r,m"(val) : : "memory");
#else
    // NOLINTNEXTLINE(hicpp-no-assembler)
    asm volatile("" : "+m,r"(val) : : "memory");
#endif
}

#endif


} // namespace

template <typename T>
void dont_optimize_away(T&& t) {
    _dont_optimize_away(std::forward<T>(t));
}

} // namespace benchmarking