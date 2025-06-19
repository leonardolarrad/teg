project(
  'teg',
  ['c', 'cpp'],
  version: '0.1.0',
  license: 'zlib',
  subproject_dir: 'benchmarking/deps',
  default_options: [
      'cpp_std=c++20',
      'default_library=static'  
    ],
  meson_version: '>=1.1.0',
)

if get_option('generate_asm')
  add_project_arguments(
    ['-save-temps', '-fverbose-asm'],
    language: 'cpp',
  )
endif

# Generate header files: members_visit.h and members_tie.h
run_command(
  'python', './scripts/generate_headers.py', 
  '--n', get_option('max_visit_members').to_string(), 
  '--output-dir', meson.project_source_root() + '/include/teg/',
  check: true)

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
  subdir('benchmarking')
endif

# Install headers
install_subdir('include/teg', install_dir: get_option('includedir'))