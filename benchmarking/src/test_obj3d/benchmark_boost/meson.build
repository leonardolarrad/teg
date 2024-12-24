obj3d_boost = executable(
  'obj3d_boost', 
  'obj3d_boost.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep, boost_dep]
)

benchmark(
  'obj3d_boost', 
  obj3d_boost,
  timeout: 0
)