# Add fuse dependency
platform = host_machine.system()
if platform == 'darwin'
  fuse = subproject('osxfuse')
elif platform == 'cygwin' or platform == 'windows'
  fuse = subproject('winfuse')
else
  fuse = subproject('libfuse')
endif
# subproject('gcs-cpp', required: false)

# Add GCS dependency
# env = environment()
# env.set('OPENSSL_ROOT_DIR', '/usr/local/opt/openssl')
# gcs_source_dir = meson.source_root() + '/subprojects/gcs-cpp'
# gcs_build_dir = meson.source_root() + '/build/subprojects/gcs-cpp/__CMake_build'
# run_command('mkdir', '-p', gcs_build_dir)
# run_command('cmake','-G', 'Ninja', '-S', gcs_source_dir, '-B', gcs_build_dir, check: true, env: env)
# run_command('ninja', '-C', gcs_build_dir, check: true)
# cplr = meson.get_compiler('cpp')
# gcs_cpp = cplr.find_library('google_cloud_cpp_common', dirs: gcs_build_dir + '/google/cloud/', static: true)
# gcs_include = ['../../subprojects/gcs-cpp']
# glob_result = run_command('bash', '../../scripts/globber.sh', '../../build/subprojects/gcs-cpp', 'include')
# gcs_include += glob_result.stdout().strip().split('\n')

# gcs_include = include_directories(gcs_include, is_system: true)
# gcs_cpp_dep = declare_dependency(dependencies: [gcs_cpp], include_directories: gcs_include)

# Configure all dependencies
deps = [
  dependency('threads'),
  fuse.get_variable('libfuse_dep'),
  subproject('spdlog').get_variable('spdlog_headeronly_dep'),
  cmake.subproject('yaml-cpp', cmake_options: ['-DYAML_BUILD_SHARED_LIBS=ON', '-DPOSITION_INDEPENDENT_CODE=True']).dependency('yaml-cpp'),
  # cmake.subproject('aws-s3-cpp').dependency('aws-cpp-sdk-s3'),
  # cmake.subproject('gcs-cpp').dependency('google_cloud_cpp_common')
  # gcs_cpp_dep
]