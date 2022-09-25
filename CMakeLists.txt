cmake_minimum_required(VERSION 3.20)

project(pixel)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PIXEL_USE_ASAN "turn on address santizer?" ON)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake/modules")

include("cmake/overlook.cmake")
include("cmake/cvbuild.cmake")
include("cmake/deps.cmake")
include("cmake/output_dir.cmake")
include("cmake/brief_summary.cmake")

if(PIXEL_USE_ASAN)
  include("cmake/asan.cmake")
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
# add_subdirectory(perfs)
# add_subdirectory(NaiveCNN)