teg_benchmarking_inc = include_directories('include')
teg_benchmarking_dep = declare_dependency(
  link_args : [],
  compile_args : [],
  include_directories : teg_benchmarking_inc,
)

# Install dependencies
google_benchmark_proj = subproject('google-benchmark', default_options: ['default_library=static'])
google_benchmark_dep = google_benchmark_proj.get_variable('google_benchmark_dep')

subproject('abseil-cpp')
protobuf_proj = subproject('protobuf')
protobuf_dep = protobuf_proj.get_variable('protobuf_dep')

flatbuffers_proj = subproject('flatbuffers')
flatbuffers_dep = flatbuffers_proj.get_variable('flatbuffers_dep')

msgpack_proj = subproject('msgpackc-cxx')
msgpack_dep = msgpack_proj.get_variable('msgpack_cxx_dep')

boost_dep = dependency('boost', modules: ['serialization'], required: true, static: true)

# Test 1: E-commerce
subdir('src/test_ecommerce/benchmark_teg')
subdir('src/test_ecommerce/benchmark_teg_compact')
subdir('src/test_ecommerce/benchmark_teg_native')
#subdir('src/test_ecommerce/benchmark_boost')
subdir('src/test_ecommerce/benchmark_msgpack')
subdir('src/test_ecommerce/benchmark_flatbuf')
subdir('src/test_ecommerce/benchmark_protobuf')

# Test 2: Model 3d
subdir('src/test_obj3d/benchmark_teg')
subdir('src/test_obj3d/benchmark_teg_compact')
subdir('src/test_obj3d/benchmark_teg_native')
subdir('src/test_obj3d/benchmark_msgpack')
#subdir('src/test_obj3d/benchmark_boost')
subdir('src/test_obj3d/benchmark_flatbuf')
subdir('src/test_obj3d/benchmark_protobuf')