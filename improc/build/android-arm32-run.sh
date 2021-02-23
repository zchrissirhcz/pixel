#!/bin/bash

BUILD_DIR=android-arm32
DST_DIR=/data/pixel
EXE_FILE=testbed

adb shell "mkdir -p $DST_DIR"
adb push $BUILD_DIR/$EXE_FILE $DST_DIR
adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

adb pull /data/pixel/sky_gray_naive.bmp ./
adb pull /data/pixel/sky_gray_fixed.bmp ./
adb pull /data/pixel/sky_gray_asimd.bmp ./
adb pull /data/pixel/sky_gray_opencv.bmp ./
adb pull /data/pixel/sky_gray_fixed_asm0.bmp ./
adb pull /data/pixel/sky_gray_fixed_asm.bmp ./