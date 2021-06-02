cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -O0")
include_directories( /usr/include/drm )

project(test)
add_executable(test test.cpp)
target_link_libraries(test rt)

project(submit)
add_executable(submit submit.cpp)
target_link_libraries(submit rt)