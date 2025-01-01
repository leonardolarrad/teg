ecommerce_teg = executable(
  'ecommerce_teg', 
  'ecommerce_teg.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep]
)

benchmark(
  'ecommerce_teg', 
  ecommerce_teg,
  timeout: 0
)