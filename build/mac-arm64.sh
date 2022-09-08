#!/bin/bash

BUILD_DIR=mac-arm64
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake ../.. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j
cd ..

