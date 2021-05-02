cmake_minimum_required(VERSION 3.15)

project(xadd)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(testbed
    #testbed3.cpp
    #array_v9_std_atomic.cpp
    #array_v9_std_mutex.cpp
    array_v9_system_api.cpp
)