ecommerce_zppbits = executable(
  'ecommerce_zppbits', 
  'ecommerce_zppbits.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep]
)

benchmark(
  'ecommerce_zppbits', 
  ecommerce_zppbits,
  timeout: 0
)