#!/bin/bash


BUILD_DIR=linux
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake ../.. -DCMAKE_BUILD_TYPE=Release
make
cd ..
