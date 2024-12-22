obj3d_teg_compact = executable(
  'obj3d_teg_compact', 
  'obj3d_teg_compact.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep]
)

benchmark(
  'obj3d_teg_compact', 
  obj3d_teg_compact,
  timeout: 0
)