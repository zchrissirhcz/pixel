cmake_minimum_required(VERSION 3.15)

project(x)

add_executable(testbed
    testbed.cpp
)
target_include_directories(testbed PRIVATE ${CMAKE_SOURCE_DIR})