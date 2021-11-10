cmake_minimum_required(VERSION 3.20)
project(pixel)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("cmake/overlook.cmake")
include("cmake/deps.cmake")

add_library(pixel STATIC
  src/rgb2gray.h
  src/rgb2gray.c

  src/rgb2bgr.h
  src/rgb2bgr.c

  src/flip.h
  src/flip.c
)
target_include_directories(pixel PUBLIC ${CMAKE_SOURCE_DIR})

macro(pixel_add_test name)
  add_executable(test_${name}  src/${name}_test.cpp)
  target_link_libraries(test_${name} pixel ${OpenCV_LIBS} GTest::gtest)
endmacro()

macro(pixel_add_perf name)
  add_executable(perf_${name}  src/${name}_perf.cpp)
  target_link_libraries(perf_${name} pixel ${OpenCV_LIBS} benchmark::benchmark)
endmacro()

# rgb2gray
pixel_add_test(rgb2gray)
pixel_add_perf(rgb2gray)

# rgb2bgr
pixel_add_test(rgb2bgr)
pixel_add_perf(rgb2bgr)

# flip
pixel_add_test(flip)
pixel_add_perf(flip)