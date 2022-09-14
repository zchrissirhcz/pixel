cmake_minimum_required(VERSION 3.15)

project(cnnop)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(testbed
    relu_layer.h
    relu_layer.c
    relu_layer_test.cpp
)

target_include_directories(testbed
    PUBLIC
        ${CMAKE_SOURCE_DIR}/..
)