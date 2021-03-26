cmake_minimum_required(VERSION 3.15)

project(flops)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_language(ASM)

#set_source_files_properties(test.S PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")

add_executable(testbed
    testbed.cpp
    #qqc.cpp
)
