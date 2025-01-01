ecommerce_teg_compact = executable(
  'ecommerce_teg_compact', 
  'ecommerce_teg_compact.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep]
)

benchmark(
  'ecommerce_teg_compact', 
  ecommerce_teg_compact,
  timeout: 0
)