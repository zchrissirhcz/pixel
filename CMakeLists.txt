cmake_minimum_required(VERSION 3.20)

project(pixel)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("cmake/overlook.cmake")
include("cmake/deps.cmake")
include("cmake/output_dir.cmake")

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
