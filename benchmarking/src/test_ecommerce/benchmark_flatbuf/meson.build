ecommerce_flatbuf = executable(
  'ecommerce_flatbuf', 
  'ecommerce_flatbuf.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep, flatbuffers_dep]
)

benchmark(
  'ecommerce_flatbuf', 
  ecommerce_flatbuf,
  timeout: 0
)