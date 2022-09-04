cmake_minimum_required(VERSION 3.15)

project(cnnop)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(USE_ASAN "use asan?" OFF)

include("/home/zz/work/overlook/overlook.cmake")

# https://stackoverflow.com/a/65019152/2999096
if(USE_ASAN)
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

add_executable(testbed
    relu_layer.h
    relu_layer.c
    relu_layer_test.cpp
)

target_include_directories(testbed
    PUBLIC
        ${CMAKE_SOURCE_DIR}/..
)