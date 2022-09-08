是在Windows下尝试的。


build/android-arm64.cmd 内容如下：
```batch
@echo off

set ANDROID_NDK=E:/soft/Android/ndk-r21b
set TOOLCHAIN=%ANDROID_NDK%/build/cmake/android.toolchain.cmake
set MAKE=%ANDROID_NDK%/prebuilt/windows-x86_64/bin/make.exe

set BUILD_DIR=android-arm64
if not exist %BUILD_DIR% md %BUILD_DIR%
cd %BUILD_DIR%

cmake -G "CodeBlocks - MinGW Makefiles" ^
    -DCMAKE_MAKE_PROGRAM=%MAKE% ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_TOOLCHAIN_FILE=%TOOLCHAIN% ^
    -DANDROID_LD=lld ^
    -DANDROID_ABI="arm64-v8a" ^
    -DANDROID_PLATFORM=android-24 ^
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ^
    -DOpenCV_DIR=E:/soft/Android/opencv-4.5.0-android-sdk/sdk/native/jni ^
    ../..

cmake --build .

cd ..
```

要点：
1. 指定generator为"CodeBlocks - MinGW Makefiles"，会生成 xxx.cbp，意思是 CodeBlock project。
2. 指定`-DCMAKE_MAKE_PROGRAM=%MAKE%`，否则cmake阶段报错。