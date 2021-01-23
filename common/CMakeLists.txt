cmake_minimum_required(VERSION 3.15)

project(demo)

add_executable(demo
    log.cpp
)

if (ANDROID)
    target_link_libraries(demo log)
endif()