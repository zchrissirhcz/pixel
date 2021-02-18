cmake_minimum_required(VERSION 3.15)

project(assignment3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")

add_executable(testbed
    main.cpp
)
target_include_directories(testbed
    PUBLIC
        ${CMAKE_SOURCE_DIR}/..
        ${CMAKE_SOURCE_DIR}/MIPP/src
        ${CMAKE_SOURCE_DIR}/eigen
        ${CMAKE_SOURCE_DIR}/xsimd/include
)

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        set(OpenCV_DIR "/home/zz/soft/opencv-4.5.1/lib/cmake/opencv4" CACHE PATH "")
    elseif (ANDROID)
        set(OpenCV_DIR "/home/zz/soft/opencv-4.5.1-android/sdk/native/jni" CACHE PATH "")
    endif()
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    if (CMAKE_SYSTEM_NAME MATCHES "Windows")
        set(OpenCV_DIR "D:/lib/opencv/4.5.1-minimal-v" CACHE PATH "")
    elseif (ANDROID)
        set(OpenCV_DIR "d:/soft/Android/opencv-4.5.0-android-sdk/sdk/native/jni" CACHE PATH "")
    endif()
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Darwin")
    if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
        set(OpenCV_DIR "/usr/local/opencv-4.2/lib/cmake/opencv4" CACHE PATH "")
    elseif (ANDROID)
        set(OpenCV_DIR "/Users/chris/soft/opencv-4.5.0-android-sdk/sdk/native/jni" CACHE PATH "")
    endif()
endif()
find_package(OpenCV REQUIRED)
target_link_libraries(testbed ${OpenCV_LIBS})
