cmake_minimum_required(VERSION 3.15)

project(perf_mat)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 11)

include("cmake/utils.cmake")
include("cmake/dependencies.cmake")

add_executable(testbed
    testbed.cpp
)

target_link_libraries(testbed ${OpenCV_LIBS})
target_include_directories(testbed PUBLIC
    ${CMAKE_SOURCE_DIR}/eigen
    ${CMAKE_SOURCE_DIR}/../common
)





add_executable(eigen_test
    eigen_test.cpp
)

target_include_directories(eigen_test PUBLIC
    ${CMAKE_SOURCE_DIR}/eigen
    ${CMAKE_SOURCE_DIR}/../common
)

message(STATUS "--- CMAKE_CXX_FLALGS are: ${CMAKE_CXX_FLAGS}")
message(STATUS "--- CMAKE_CXX_FLAGS_RELEASE are: ${CMAKE_CXX_FLAGS_RELEASE}")