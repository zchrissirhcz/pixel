#!/bin/bash


BUILD_DIR=linux
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake ../..
make
cd ..
