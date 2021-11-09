cmake_minimum_required(VERSION 3.20)
project(pixel)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("cmake/overlook.cmake")
include("cmake/deps.cmake")

add_library(pixel STATIC
  imgproc/rgb2gray.h
  imgproc/rgb2gray.c

  imgproc/rgb2bgr.h
  imgproc/rgb2bgr.c
)
target_include_directories(pixel PUBLIC ${CMAKE_SOURCE_DIR})

#------------------------------
# rgb2gray
#------------------------------
add_executable(test_rgb2gray
  imgproc/rgb2gray_test.cpp
)

add_executable(perf_rgb2gray
  imgproc/rgb2gray_perf.cpp
)

target_link_libraries(test_rgb2gray pixel ${OpenCV_LIBS} GTest::gtest)
target_link_libraries(perf_rgb2gray pixel ${OpenCV_LIBS} benchmark::benchmark)

#------------------------------
# rgb2bgr
#------------------------------
add_executable(test_rgb2bgr
  imgproc/rgb2bgr_test.cpp
)

add_executable(perf_rgb2bgr
  imgproc/rgb2bgr_perf.cpp
)

target_link_libraries(test_rgb2bgr pixel ${OpenCV_LIBS} GTest::gtest)
target_link_libraries(perf_rgb2bgr pixel ${OpenCV_LIBS} benchmark::benchmark)
