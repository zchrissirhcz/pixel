#!/bin/bash

BUILD_DIR=android-arm64
DST_DIR=/data/local/tmp

adb push $BUILD_DIR/testbed $DST_DIR/
adb shell "cd $DST_DIR; chmod +x ./testbed; ./testbed"
