ecommerce_protobuf = executable(
  'ecommerce_protobuf', 
  'ecommerce_protobuf.cc', 'ecommerce.pb.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep, protobuf_dep]
)

benchmark(
  'ecommerce_protobuf', 
  ecommerce_protobuf,
  timeout: 0
)