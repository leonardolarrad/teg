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

#pragma once
#include <algorithm>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstring>
#include <format>
#include <iosfwd>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

namespace benchmarking {

using measured_time = double;

struct measured_result {
    measured_time real_time;
    measured_time cpu_time;
    std::size_t iterations;
};

struct stats {
    measured_time mean;
    measured_time median;
    measured_time stddev;
    double cv;

    stats() = default;
};

struct config {
    std::string benchmark_name;
    std::size_t warmup = 10;
    std::size_t iterations = 1;
    std::size_t repetitions = 1;
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

    benchmark& repetitions(std::size_t repetitions) {
        m_config.repetitions = repetitions;
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
        std::size_t repetitions = std::max(m_config.repetitions, std::size_t(1));
        
        std::vector<measured_result> repetitions_results;
        repetitions_results.reserve(repetitions);
        std::vector<measured_time> real_time_results;
        real_time_results.reserve(repetitions);
        std::vector<measured_time> cpu_time_results;
        cpu_time_results.reserve(repetitions);

        for (std::size_t i = 0; i < repetitions; ++i) {
            std::size_t iterations = std::max(m_config.iterations, std::size_t(1));

            auto const start = std::chrono::high_resolution_clock::now();
            for (std::size_t i = 0; i < iterations; ++i) {
                benchmark_func();
            }
            auto const end = std::chrono::high_resolution_clock::now();
            
            auto const duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
            measured_time const real_time = duration.count() / double(iterations);
            measured_time const cpu_time = duration.count() / double(iterations);

            repetitions_results.emplace_back(real_time, cpu_time, iterations);
            real_time_results.emplace_back(real_time);
            cpu_time_results.emplace_back(cpu_time);
        }

        // Stats    
        stats const real_time_stats = compute_stats(real_time_results);
        //stats const cpu_time_stats = compute_stats(cpu_time_results);

        // Output.
        report_header();
        report_repetitions(repetitions_results);

        if (repetitions > 1) {
            report_stats(real_time_stats);
        }

        return *this;       
    }

    stats compute_stats(std::vector<measured_time> times) {
        stats stats;        
        // Compute mean
        stats.mean = std::accumulate(times.begin(), times.end(), 0.0) / times.size();

        // Compute median
        std::sort(times.begin(), times.end());
        stats.median = times[times.size() / 2];        
        
        // Compute stddev
        double sum = 0.0;
        for (auto time : times) {
            sum += (time - stats.mean) * (time - stats.mean);
        }
        stats.stddev = std::sqrt(sum / times.size());

        // Compute coefficient of variation
        stats.cv = stats.stddev / stats.mean;
        
        return stats;
    }

    void report_header() {
        std::cout << "\n";
        std::cout << "| benchmark                              ";
        std::cout << "|       real time (ns/op)";
        std::cout << "|        cpu time (ns/op)";
        std::cout << "|              iterations";
        std::cout << "|\n";

        std::cout << "|----------------------------------------";
        std::cout << "|------------------------";
        std::cout << "|------------------------";
        std::cout << "|------------------------";
        std::cout << "|\n";
    }

    void report_repetitions(std::vector<measured_result> const& repetitions) {
        for (std::size_t i = 0; i < repetitions.size(); ++i) {
            if (repetitions.size() == 1) {
                std::cout << "| " << std::format("{:39}", m_config.benchmark_name);
            }
            else {
                std::cout << std::format("| {:39}", std::format("{}:{}", m_config.benchmark_name, i));
            }

            std::cout << std::format("| {:23.3f}", repetitions[i].real_time);
            std::cout << std::format("| {:23.3f}", repetitions[i].cpu_time);
            std::cout << std::format("| {:23}", repetitions[i].iterations);
            std::cout << "|\n";
        }
    }

    void report_stats(stats const& stats) {
        std::cout << "\n";
        std::cout << "| benchmark                              ";
        std::cout << "|    mean (ns/op)";
        std::cout << "|  median (ns/op)";
        std::cout << "|  stddev (ns/op)";
        std::cout << "|  cv (%)";
        std::cout << "|\n";

        std::cout << "|----------------------------------------";
        std::cout << "|----------------";
        std::cout << "|----------------";
        std::cout << "|----------------";
        std::cout << "|--------";
        std::cout << "|\n";

        std::cout << "| " << std::format("{:39}", m_config.benchmark_name);
        std::cout << std::format("| {:15.3f}", stats.mean);
        std::cout << std::format("| {:15.3f}", stats.median);
        std::cout << std::format("| {:15.3f}", stats.stddev);
        std::cout << std::format("| {:7.2f}", stats.cv*100);
        std::cout << "|\n";
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