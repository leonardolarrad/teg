benchmark_zppbits = executable(
  'benchmark_zppbits', 
  'benchmark_zppbits.cc',
  dependencies: [teg_dep, teg_benchmarking_dep]
)

benchmark(
  'zppbits', 
  benchmark_zppbits,
  timeout: 0
)