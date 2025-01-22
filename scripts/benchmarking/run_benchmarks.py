import os
from enum import Enum

BUILD_DIR = "../../build/msvc/release/"
OUT_DIR = "../../build/msvc/release/benchmarking/src/test_ecommerce/"
BENCHMARKS = [
    'teg', 
    'teg_compact',
    'protobuf',
    'flatbuf',
    'msgpack',
    'boost'
]

class BenchmarkDataSize(Enum):
    KiB = 1
    MiB = 2
    GiB = 3

BENCHMARKS_DATA_SIZES = [
    BenchmarkDataSize.MiB,
    BenchmarkDataSize.GiB
]

def to_cpp_def(data_size: BenchmarkDataSize) -> str:
    if data_size == BenchmarkDataSize.KiB:
        return "BM_DATASIZE_1KIB"
    elif data_size == BenchmarkDataSize.MiB:
        return "BM_DATASIZE_1MIB"
    elif data_size == BenchmarkDataSize.GiB:
        return "BM_DATASIZE_1GIB"
    else:
        raise ValueError("Invalid BenchmarkDataSize")

def to_string(data_size: BenchmarkDataSize) -> str:
    if data_size == BenchmarkDataSize.KiB:
        return "1KiB"
    elif data_size == BenchmarkDataSize.MiB:
        return "1MiB"
    elif data_size == BenchmarkDataSize.GiB:
        return "1GiB"
    else:
        raise ValueError("Invalid BenchmarkDataSize")

def setup_buildsystem(data_size: BenchmarkDataSize):
    os.system(f'cd {BUILD_DIR} && meson configure -Dcpp_args="-DBM_LOG_BUFFER_SIZE -D{to_cpp_def(data_size)}"')
    os.system(f'cd {BUILD_DIR} && msbuild teg.sln')

def run_benchmark(benchmark: str, data_size: BenchmarkDataSize):
    #os.system(f'cd {BUILD_DIR} && meson test --benchmark ecommerce_{benchmark} -v --test-args="--benchmark_out_format=csv --benchmark_out={benchmark}_{to_string(data_size)}.csv"')
    os.system(f'cd {OUT_DIR}benchmark_{benchmark}/ && ecommerce_{benchmark}.exe --benchmark_out_format=csv --benchmark_out={benchmark}_{to_string(data_size)}.csv')

def measure_benchmarks():
    for data_size in BENCHMARKS_DATA_SIZES:
        setup_buildsystem(data_size)
        for benchmark in BENCHMARKS:
            run_benchmark(benchmark, data_size)

def main():
    measure_benchmarks()

if __name__ == '__main__':
    main()