import os
from enum import Enum

BUILD_DIR = "../../build/msvc/release/"
OUT_DIR = "../../build/msvc/release/benchmarking/src/test_obj3d/"
PROCESS = [
    'teg', 
    'teg_compact',
    'protobuf',
    'flatbuf',
    'msgpack',
    'boost'
]

def measure_process(process: str):    
    os.system(f'cd {OUT_DIR}benchmark_{process}/ && obj3d_{process}.exe --benchmark_counters_tabular=true --benchmark_out_format=csv --benchmark_out=__final_{process}.csv')

def measure_memory_usage():
    for process in PROCESS:
        measure_process(process)

def main():
    measure_memory_usage()

if __name__ == '__main__':
    main()