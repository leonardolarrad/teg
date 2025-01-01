ecommerce_msgpack = executable(
  'ecommerce_msgpack', 
  'ecommerce_msgpack.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, msgpack_dep, google_benchmark_dep]
)

benchmark(
  'ecommerce_msgpack', 
  ecommerce_msgpack,
  timeout: 0
)