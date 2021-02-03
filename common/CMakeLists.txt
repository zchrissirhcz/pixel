cmake_minimum_required(VERSION 3.15)

project(demo)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)


include("utils.cmake")

option(USE_ASAN "use address sanitizer?" OFF)
option(USE_TIDY "use clang-tidy?" OFF)

if (USE_ASAN)
    # https://stackoverflow.com/a/65019152/2999096
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

if (USE_TIDY)
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

add_executable(demo
    pixel_log.h
    #pixel_log_test.c
    
    pixel_bmp.c
    pixel_bmp.h
    #pixel_bmp_test.cpp

    pixel_jpg.c
    pixel_jpg.h
    #pixel_jpg_test.cpp

    pixel_rng.h
    pixel_rng_test.cpp
)

set(dep_libs "")

if (ANDROID)
    list(APPEND dep_libs log)
endif()

if (ANDROID)
    set(OpenCV_DIR "/Users/chris/soft/opencv-4.5.0-android-sdk/sdk/native/jni" CACHE PATH "")
else()
    set(OpenCV_DIR "/usr/local/opencv-4.2/lib/cmake/opencv4" CACHE PATH "")
endif()

find_package(OpenCV REQUIRED)
list(APPEND dep_libs ${OpenCV_LIBS})

target_link_libraries(demo ${dep_libs})