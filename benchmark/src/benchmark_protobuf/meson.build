benchmark_protobuf = executable(
  'benchmark_protobuf', 
  'benchmark_protobuf.cc',
  dependencies: [teg_dep, benchmark_dep]
)

benchmark(
  'protobuf', 
  benchmark_protobuf,
  timeout: 0
)