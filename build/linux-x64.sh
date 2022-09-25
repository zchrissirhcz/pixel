#!/bin/bash

BUILD_DIR=linux-x64
CVBUILD_PLATFORM=linux
CVBUILD_ARCH=x64
mkdir -p $BUILD_DIR
cd $BUILD_DIR

unset CC
unset CXX

cmake ../..
cmake --build . -j
cd ..

