cmake_minimum_required(VERSION 3.5)

project(test)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -O0")

include_directories( /usr/include/drm )

add_executable(test test.cpp)
target_link_libraries(test rt)

