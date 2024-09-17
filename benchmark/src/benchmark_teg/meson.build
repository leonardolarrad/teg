benchmark_teg = executable(
  'benchmark_teg', 
  'benchmark_teg.cc',
  dependencies: [teg_dep, benchmark_dep]
)

benchmark(
  'teg', 
  benchmark_teg,
  timeout: 0
)