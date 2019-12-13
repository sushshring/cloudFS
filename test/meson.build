test_src = [
  'test_main.cpp',
  'test_lib/cloudfs_test.cpp',
  'test_lib/cloudfs_types/fd/fd_manager_test.cpp'
]
gtest = subproject('gtest')
test_deps = [
  gtest.get_variable('gtest_dep'),
  gtest.get_variable('gmock_dep')
]

test_exe = executable(
  'cloudfs_test',
  test_src,
  dependencies: test_deps,
#  link_with: lib,
  install: true)
test('cloudfs tests', test_exe)