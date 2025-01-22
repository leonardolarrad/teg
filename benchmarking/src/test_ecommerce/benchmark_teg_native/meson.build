ecommerce_teg_native = executable(
  'ecommerce_teg_native', 
  'ecommerce_teg_native.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep]
)

benchmark(
  'ecommerce_teg_native', 
  ecommerce_teg_native,
  timeout: 0
)