obj3d_teg_native = executable(
  'obj3d_teg_native', 
  'obj3d_teg_native.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep]
)

benchmark(
  'obj3d_teg_native', 
  obj3d_teg_native,
  timeout: 0
)