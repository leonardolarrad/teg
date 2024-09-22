benchmark_protobuf = executable(
  'benchmark_protobuf', 
  ['benchmark_protobuf.cc', 'test_data.pb.cc'],
  dependencies: [teg_dep, benchmark_dep, protobuf_dep]
)

benchmark(
  'protobuf', 
  benchmark_protobuf,
  timeout: 0
)