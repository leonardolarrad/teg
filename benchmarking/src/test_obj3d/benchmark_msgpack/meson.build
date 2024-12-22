obj3d_msgpack = executable(
  'obj3d_msgpack', 
  'obj3d_msgpack.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, msgpack_dep, google_benchmark_dep]
)

benchmark(
  'obj3d_msgpack', 
  obj3d_msgpack,
  timeout: 0
)