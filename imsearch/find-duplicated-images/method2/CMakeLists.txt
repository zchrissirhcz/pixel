cmake_minimum_required(VERSION 3.15)

project(demo)
set(CMAKE_CXX_STANDARD 11)

set(OpenCV_DIR "/usr/local/opencv-4.2/lib/cmake/opencv4" CACHE PATH "")
find_package(OpenCV REQUIRED)

add_library(phash STATIC phash.cpp)
target_link_libraries(phash PUBLIC ${OpenCV_LIBS})

add_executable(main
    main.cpp
)

target_link_libraries(main phash)

