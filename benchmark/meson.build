benchmark_inc = include_directories('include')
benchmark_dep = declare_dependency(
  link_args : [],
  compile_args : [],
  include_directories : benchmark_inc
)

subproject('abseil-cpp')
protobuf_proj = subproject('protobuf')
protobuf_dep = protobuf_proj.get_variable('protobuf_dep')

google_benchmark_proj = subproject('google-benchmark')
google_benchmark_dep = google_benchmark_proj.get_variable('google_benchmark_dep')

flatbuffers_proj = subproject('flatbuffers')
flatbuffers_dep = flatbuffers_proj.get_variable('flatbuffers_dep')

subdir('src/benchmark_teg')
subdir('src/benchmark_protobuf')
subdir('src/benchmark_zppbits')
subdir('src/benchmark_flatbuffers')
subdir('src/benchmark_cista')
#subdir('src/gbm_teg')
