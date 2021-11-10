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

perf_rgb2bgr()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=perf_rgb2bgr

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb push ../assets/sky.jpg $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

perf_flip()
{
    DST_DIR=/data/local/tmp
    EXE_FILE=perf_flip

    adb push $BUILD_DIR/$EXE_FILE $DST_DIR
    adb push ../assets/sky.jpg $DST_DIR
    adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"
}

#perf_rgb2gray
#perf_rgb2bgr
perf_flip
