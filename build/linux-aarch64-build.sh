#!/bin/bash

BUILD_DIR=linux-aarch64-asan
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake ../.. \
    -DCMAKE_TOOLCHAIN_FILE=/home/zz/work/pixel/cmake/aarch64-linux-gnu.toolchain.cmake \
    -DGTest_DIR=/home/zz/artifacts/googletest/master/linux-aarch64-asan/lib/cmake/GTest \
    -DOpenCV_DIR=/home/zz/artifacts/opencv/4.6.0/linux-aarch64-asan/lib/cmake/opencv4

cmake --build . -j2
