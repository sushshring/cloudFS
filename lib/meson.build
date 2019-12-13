cmake = import('cmake')

subdir('src-config')
subdir('dep-config')
add_project_link_arguments(['-lstdc++fs', 'fPIC'], language: 'cpp')

# Build library
lib = static_library(
  'cloudfs',
  libSrc,
  # include_directories: [gcs_include],
  dependencies: deps,
  cpp_args : ['-D_FILE_OFFSET_BITS=64', '-DFUSE_USE_VERSION=26'],
  install: true)