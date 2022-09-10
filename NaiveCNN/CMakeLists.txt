cmake_minimum_required(VERSION 3.12)

project(CNN)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_C_STANDARD 11)

include("overlook.cmake")

add_library(cnn
    src/cnn.c
    src/cnn.h
    src/mat.c
    src/mat.h
    
    src/loadbmp.h

    src/naive_config.h
    
    src/naive_cnn.h
    src/naive_cnn.c

    src/naive_train.c
    src/naive_infer.c

    src/naive_convolution.h
    src/naive_convolution.c
    src/naive_pooling.h
    src/naive_pooling.c
    src/naive_relu.h
    src/naive_relu.c
)

add_executable(example-mnist
    examples/mnist.c
    examples/mnist.h
)
target_include_directories(example-mnist PRIVATE "src")

if (CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_link_libraries(example-mnist m)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_compile_definitions(cnn PRIVATE -D_CRT_SECURE_NO_WARNINGS)
endif()


target_link_libraries(example-mnist cnn)
