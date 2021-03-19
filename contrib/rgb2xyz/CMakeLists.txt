cmake_minimum_required(VERSION 3.15)

project(rgb2xyz)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("cmake/utils.cmake")
include("cmake/dependencies.cmake")

add_executable(testbed
    testbed.cpp
    "${CMAKE_SOURCE_DIR}/../../matcalc/array_mean.h"
    "${CMAKE_SOURCE_DIR}/../../matcalc/array_mean.c"
)

target_include_directories(testbed PUBLIC "${CMAKE_SOURCE_DIR}/../..")

target_link_libraries(testbed ${OpenCV_LIBS})