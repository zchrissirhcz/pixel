#!/bin/bash

BUILD_DIR=linux
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    ../..

cmake --build .

cd ..