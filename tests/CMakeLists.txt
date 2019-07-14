project(tests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(tests
  kube_test.cpp
)
target_link_libraries(tests
  gtest
  gtest_main
)
