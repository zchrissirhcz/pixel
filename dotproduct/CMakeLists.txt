cmake_minimum_required(VERSION 3.15)

project(assignment3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

add_executable(testbed
    main.cpp
)

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(OpenCV_DIR "/home/zz/soft/opencv-4.5.1/lib/cmake/opencv4" CACHE PATH "")
elseif (ANDROID)
    set(OpenCV_DIR "/home/zz/soft/opencv-4.5.1-android/sdk/native/jni" CACHE PATH "")
endif()
find_package(OpenCV REQUIRED)
target_link_libraries(testbed ${OpenCV_LIBS})
