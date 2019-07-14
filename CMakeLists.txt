cmake_minimum_required(VERSION 3.14)

project(kube)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_subdirectory(include)
add_subdirectory(tests)
