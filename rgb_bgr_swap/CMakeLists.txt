cmake_minimum_required(VERSION 3.15)

project(demo)

set(CMAKE_CXX_STANDARD 11)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-tree-vectorize")

add_executable(demo
    src/main.cpp
)

target_include_directories(demo PUBLIC ${CMAKE_SOURCE_DIR}/..)