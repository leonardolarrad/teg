benchmark_cista = executable(
  'benchmark_cista', 
  'benchmark_cista.cc',
  dependencies: [teg_benchmarking_dep]
)

benchmark(
  'cista', 
  benchmark_cista,
  timeout: 0
)
