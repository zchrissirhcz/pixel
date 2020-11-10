#!/bin/bash

BUILD_DIR=android-arm64
DST_DIR=/data/pixel
EXE_FILE=demo

adb shell "mkdir -p $DST_DIR"
adb push $BUILD_DIR/$EXE_FILE $DST_DIR
adb push 000001.jpg $DST_DIR
adb shell "cd $DST_DIR; chmod +x $DST_DIR/$EXE_FILE; ./$EXE_FILE"

adb pull $DST_DIR/neon.jpg ./
adb pull $DST_DIR/none_neon.jpg ./
adb pull $DST_DIR/none_neon2.jpg ./
