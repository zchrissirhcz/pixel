cmake_minimum_required(VERSION 3.15)

project(traverse_pixel)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("cmake/utils.cmake")

add_executable(testbed
    #src/traverse_pixels.cpp
    #src/bilinear_resize.cpp
    src/auto_contrast_colorscale.cpp
)

if(ANDROID)
    set(OpenCV_DIR "/home/zz/soft/OpenCV-android-sdk/4.5.0/sdk/native/jni" CACHE PATH "")
endif()

find_package(OpenCV REQUIRED
    #COMPONENTS core imgcodecs
)

target_link_libraries(testbed ${OpenCV_LIBS})