obj3d_teg = executable(
  'obj3d_teg', 
  'obj3d_teg.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep]
)

benchmark(
  'obj3d_teg', 
  obj3d_teg,
  timeout: 0
)