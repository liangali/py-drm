cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -O0")

project(test)
add_executable(test test.cpp)
target_link_libraries(test va va-drm)

