ecommerce_boost = executable(
  'ecommerce_boost', 
  'ecommerce_boost.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep, boost_dep],
)

benchmark(
  'ecommerce_boost', 
  ecommerce_boost,
  timeout: 0
)
