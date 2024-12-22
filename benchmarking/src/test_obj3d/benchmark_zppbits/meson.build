obj3d_zppbits = executable(
  'obj3d_zppbits', 
  'obj3d_zppbits.cc',
  dependencies: [teg_dep, teg_benchmarking_dep, google_benchmark_dep]
)

benchmark(
  'obj3d_zppbits', 
  obj3d_zppbits,
  timeout: 0
)