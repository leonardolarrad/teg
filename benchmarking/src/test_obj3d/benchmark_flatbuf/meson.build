obj3d_flatbuf = executable(
  'obj3d_flatbuf', 
  'obj3d_flatbuf.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep, flatbuffers_dep]
)

benchmark(
  'obj3d_flatbuf', 
  obj3d_flatbuf,
  timeout: 0
)