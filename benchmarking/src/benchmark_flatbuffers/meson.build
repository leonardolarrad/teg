benchmark_flatbuffers = executable(
  'benchmark_flatbuffers', 
  ['benchmark_flatbuffers.cc'],
  dependencies: [teg_dep, teg_benchmarking_dep, flatbuffers_dep]
)

benchmark(
  'flatbuffers', 
  benchmark_flatbuffers,
  timeout: 0
)
