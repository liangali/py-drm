cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -O0")
include_directories( /usr/include/drm )

project(test)
add_executable(test test.cpp)
target_link_libraries(test rt)

project(submit)
add_executable(submit submit.cpp)
target_link_libraries(submit rt)

project(memwrite)
add_executable(memwrite memwrite.cpp)
target_link_libraries(memwrite rt)

project(pydrm)
if(DEFINED ENV{PYTHON_INCLUDE} AND DEFINED ENV{PYBIND11_INCLUDE})
    message(STATUS "INFO: PYTHON_INCLUDE=" $ENV{PYTHON_INCLUDE})
    message(STATUS "INFO: PYBIND11_INCLUDE=" $ENV{PYBIND11_INCLUDE})
else()
    message(FATAL_ERROR "####ERROR: PYTHON_INCLUDE & PYBIND11_INCLUDE is not set, run \"python3 gen_env.py\" to generate \"env.sh\" and run \"source env.sh\"")
endif()
include_directories (pydrm $ENV{PYTHON_INCLUDE} $ENV{PYBIND11_INCLUDE})
add_library(pydrm SHARED pydrm.cpp)
target_link_libraries(pydrm rt)
set_target_properties(pydrm PROPERTIES PREFIX "")