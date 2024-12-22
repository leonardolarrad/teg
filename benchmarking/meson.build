teg_benchmarking_inc = include_directories('include')
teg_benchmarking_dep = declare_dependency(
  link_args : [],
  compile_args : [],
  include_directories : teg_benchmarking_inc
)

google_benchmark_proj = subproject('google-benchmark')
google_benchmark_dep = google_benchmark_proj.get_variable('google_benchmark_dep')

subproject('abseil-cpp')
protobuf_proj = subproject('protobuf')
protobuf_dep = protobuf_proj.get_variable('protobuf_dep')

flatbuffers_proj = subproject('flatbuffers')
flatbuffers_dep = flatbuffers_proj.get_variable('flatbuffers_dep')

subdir('src/test_obj3d/benchmark_teg')
subdir('src/test_obj3d/benchmark_zppbits')

subdir('src/benchmark_protobuf')
subdir('src/benchmark_zppbits')
subdir('src/benchmark_flatbuffers')
subdir('src/benchmark_cista')
#subdir('src/gbm_teg')
