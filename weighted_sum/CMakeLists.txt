cmake_minimum_required(VERSION 3.15)

project(demo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(demo
    src/main.cpp
)

target_include_directories(demo PUBLIC ${CMAKE_SOURCE_DIR}/..)

