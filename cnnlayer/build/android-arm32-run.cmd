@echo off

set BUILD_DIR=android-arm32
@REM set DST_DIR=/data/pixel
set DST_DIR=/data/local/tmp
set EXE_FILE=testbed

adb shell "mkdir -p %DST_DIR%"
adb push %BUILD_DIR%/%EXE_FILE% %DST_DIR%
adb shell "cd %DST_DIR%; chmod +x %DST_DIR%/%EXE_FILE%; ./%EXE_FILE%"

@REM adb pull /data/pixel/sky_gray_naive.bmp ./
@REM adb pull /data/pixel/sky_gray_fixed.bmp ./
@REM adb pull /data/pixel/sky_gray_asimd.bmp ./
@REM adb pull /data/pixel/sky_gray_opencv.bmp ./
@REM adb pull /data/pixel/sky_gray_fixed_asm0.bmp ./
@REM adb pull /data/pixel/sky_gray_fixed_asm.bmp ./

pause