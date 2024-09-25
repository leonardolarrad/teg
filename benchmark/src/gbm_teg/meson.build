gbm_teg = executable(
  'gbm_teg', 
  'gbm_teg.cc',
  dependencies: [teg_dep, benchmark_dep, google_benchmark_dep]
)

benchmark(
  'gbm_teg', 
  gbm_teg,
  timeout: 0
)