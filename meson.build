project(
    'teg',
    'cpp',
    version: '0.1.0',
    license: 'zlib',
    subproject_dir: 'external',
    default_options: ['cpp_std=c++20'],
)

# Build project
teg_dir = include_directories('include')
teg_inc = [teg_dir]
teg_dep = declare_dependency(
  link_args : [],
  compile_args : [],
  include_directories : teg_inc
)

# Build tests
if get_option('enable_tests')
  subdir('tests')
endif

# Build benchmarks
if get_option('enable_benchmarking')
  subdir('benchmark')
endif