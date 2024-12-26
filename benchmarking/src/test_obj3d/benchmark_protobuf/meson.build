obj3d_protobuf = executable(
  'obj3d_protobuf', 
  'obj3d_protobuf.cc', 'obj3d.pb.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep, protobuf_dep]
)

benchmark(
  'obj3d_protobuf', 
  obj3d_protobuf,
  timeout: 0
)