@echo off

set BUILD_DIR=android-arm64
@REM set DST_DIR=/data/pixel
set DST_DIR=/data/local/tmp
set EXE_FILE=testbed

adb shell "mkdir -p %DST_DIR%"
adb push %BUILD_DIR%/%EXE_FILE% %DST_DIR%
adb shell "cd %DST_DIR%; chmod +x %DST_DIR%/%EXE_FILE%; ./%EXE_FILE%"

@REM adb pull %DST_DIR%/colorhouse_flip_opencv.png ./
@REM adb pull %DST_DIR%/colorhouse_flip_naive.png  ./
@REM adb pull %DST_DIR%/colorhouse_flip_idxopt.png  ./
@REM adb pull %DST_DIR%/colorhouse_flip_asimd.png  ./