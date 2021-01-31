cmake_minimum_required(VERSION 3.15)
project(demo)
set(CMAKE_CXX_STANDARD 11)
add_executable(demo
    test.cpp
)
target_include_directories(demo PUBLIC ${CMAKE_SOURCE_DIR})

set(OpenCV_DIR "/usr/local/opencv-4.2/lib/cmake/opencv4" CACHE PATH "")
find_package(OpenCV REQUIRED)
target_link_libraries(demo ${OpenCV_LIBS})
