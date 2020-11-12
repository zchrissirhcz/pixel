cmake_minimum_required(VERSION 3.15)

project(demo)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# build cpufeatures as a static lib
add_library(cpufeatures STATIC
            ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c)

set(neon_SRCS src/helloneon-intrinsics.cpp)
add_definitions("-DHAVE_NEON=1")


add_executable(demo
    src/helloneon.cpp
    src/fastMalloc.cpp
    ${neon_SRCS}
)

target_include_directories(demo PRIVATE
    ${ANDROID_NDK}/sources/android/cpufeatures)
target_link_libraries(demo cpufeatures)