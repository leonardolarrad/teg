test_inc = include_directories('include')
test_dep = declare_dependency(
  link_args : [],
  compile_args : [],
  include_directories : test_inc
)

subdir('src/test_reflection')
subdir('src/test_md5')
subdir('src/test_overload_resolution')
subdir('src/test_buffer')
subdir('src/test_fundamental')
subdir('src/test_c_array')
subdir('src/test_container')
subdir('src/test_vector')
subdir('src/test_string')
subdir('src/test_fixed_string')
subdir('src/test_set')
subdir('src/test_map')
subdir('src/test_owning_ptr')
subdir('src/test_optional')
subdir('src/test_tuple')
subdir('src/test_variant')
subdir('src/test_varint')
subdir('src/test_aggregates')
subdir('src/test_comptime_serialization')
subdir('src/test_user_serialization')
subdir('src/test_alignment')
subdir('src/test_endianness')
subdir('src/test_stream')
subdir('src/test_schema')
subdir('src/test_compatible')

#subdir('src/ex_comptime_serialization')

if get_option('with_modules')
  subdir('src/test_modules')
endif