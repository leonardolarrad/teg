project(
    'teg',
    'cpp',
    version: '0.1.0',
    license: 'zlib',
    subproject_dir: 'benchmark/deps',
    default_options: ['cpp_std=c++20'],
)

add_project_arguments(
  ['-save-temps', '-fverbose-asm'],
  language: 'cpp',
)

# Build project
teg_dir = include_directories('include')
teg_inc = [teg_dir]

teg_dep = declare_dependency(
  include_directories : teg_inc,
  link_with : [],
  link_args : [],
  compile_args : [],
)

# Build tests
if get_option('enable_tests')
  subdir('tests')
endif

# Build benchmarks
if get_option('enable_benchmarking')
  subdir('benchmark')
endif