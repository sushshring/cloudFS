project('cloudFS', 'cpp',
  version : '0.1',
  license: 'MIT',
  default_options : ['warning_level=3', 'cpp_std=c++17'])
inc = include_directories('include')

# Subdirectories
subdir('include')
subdir('lib')
subdir('test')
# deps += test_deps

src = [
  'main.cpp'
]
# Executable
exe = executable(
  'Cloudfs',
  src,
  include_directories: inc,
  cpp_args : ['-D_FILE_OFFSET_BITS=64', '-DFUSE_USE_VERSION=26'],
  dependencies: deps,
  link_with: lib,
  install: true)
#test('cloudfs tests', exe)
