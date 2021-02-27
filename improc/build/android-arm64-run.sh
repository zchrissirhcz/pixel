#!/bin/bash

BUILD_DIR=android-arm64
DST_DIR=/data/pixel
EXE_FILE=testbed

adb shell "mkdir -p $DST_DIR"
adb push $BUILD_DIR/$EXE_FILE $DST_DIR
adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

adb pull /data/pixel/sky_rgb_naive.bmp ./
adb pull /data/pixel/sky_rgb_idxopt.bmp ./
adb pull /data/pixel/sky_rgb_asimd.bmp ./
adb pull /data/pixel/sky_rgb_asm.bmp ./
adb pull /data/pixel/sky_rgb_opencv.bmp ./

adb pull /data/pixel/sky_rgb_inplace_naive.bmp ./
adb pull /data/pixel/sky_rgb_inpalce_naive2.bmp ./
adb pull /data/pixel/sky_rgb_inplace_asm.bmp ./
adb pull /data/pixel/sky_rgb_inplace_opencv.bmp ./