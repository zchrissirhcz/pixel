#!/bin/bash

# For performance test

BUILD_DIR=android-arm64
DST_DIR=/data/local/tmp

perf_rgb2gray()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=perf_rgb2gray

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb push ../assets/sky.jpg $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

perf_rgb2gray

