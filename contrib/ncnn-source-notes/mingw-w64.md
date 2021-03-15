# MinGW-w64 编译 OpenCV 和 ncnn 并运行 example

## 0x0 说明

纯粹是试一下的目的，真实项目不建议用 MinGW 这套编译工具链，因为：
- 不如 Visual Studio 调试方便
- 依赖库的生态往往容易出幺蛾子

那好，我们抱着试一试的心态，在Windows上，用 MinGW-w64 和 cmake，尝试编译 OpenCV 4.5.0 和 最新 ncnn，并运行 yolov5 的例子。

## 0x1 环境

工具：
- MinGW w64，也就是64位的 mingw
- cmake 3.19.1
- CLion 2020.4

源码：
- OpenCV 4.5.0
- ncnn

终端：
- mingw-w64 terminal，而不是cmd.exe

![](mingw_terminal_1.png)

## 0x2 编译 mingw-w64 版 OpenCV

在 git bash 执行：
```bash
cd /e/dbg/cnn-deploy
git clone https://gitee.com/opencv/opencv -b 4.5.0 opencv-4.5.0
cd opencv-4.5.0
mkdir build
cd build
subl mingw-w64.cmd
```
其中`subl`是sublime，或者你用别的编辑器也可以。build/mingw-w64.cmd内容：

```batch
@echo off

set BUILD_DIR=mingw-w64
if not exist %BUILD_DIR% md %BUILD_DIR%
cd %BUILD_DIR%

set PATH=%PATH%;"E:/soft/mingw-w64/mingw64/bin"

@REM -G "CodeBlocks - MinGW Makefiles" 

cmake -G "CodeBlocks - MinGW Makefiles" ^
    -D BUILD_LIST='core,imgcodecs,imgproc,highgui,videoio' ^
    -D CMAKE_INSTALL_PREFIX='e:/dbg/cnn-deploy/opencv-4.5.0/install' ^
    -D BUILD_TESTS=OFF ^
    -D BUILD_PERF_TESTS=OFF ^
    -D WITH_CUDA=OFF ^
    -D WITH_VTK=OFF ^
    -D WITH_MATLAB=OFF ^
    -D BUILD_DOCS=OFF ^
    -D BUILD_opencv_python3=OFF ^
    -D BUILD_opencv_python2=OFF ^
    -D WITH_IPP=OFF ^
    -D OPENCV_ENABLE_ALLOCATOR_STATS=OFF ^
    ../..

mingw32-make -j4
mingw32-make install
```

注意，其中只变异了core等模块，并且`WITH_IPP=OFF`避免了下载ipp库的网络失败问题，`OPENCV_ENABLE_ALLOCATOR_STATS=OFF`则是避免了编译最后出错。

打开mingw-w64 terminal终端（标题是Run Terminal），执行`build/mingw-w64.cmd`脚本。


## 0x3 编译 mingw-w64 版 ncnn
在 git bash 执行
```batch
cd /e/dbg/cnn-deploy
git clone https://github.com/tencent/ncnn
cd ncnn
mkdir build
cd build
subl mingw-w64.cmd
```

build/mingw-w64.cmd内容：
```batch
@echo off

set BUILD_DIR=mingw-w64

if not exist %BUILD_DIR% md %BUILD_DIR%

cd %BUILD_DIR%

cmake -G "CodeBlocks - MinGW Makefiles" ^
    -DCMAKE_INSTALL_PREFIX=%cd%/install ^
    -DNCNN_SSE2=OFF ^
    -DNCNN_AVX2=OFF ^
    -DCMAKE_BUILD_TYPE=Debug ^
    ../..

mingw32-make -j4
mingw32-make install
```
注意这里关闭了`NCNN_SSE2`和`NCNN_AVX2`：默认是打开的，虽然能编译通过，但执行时会crash在avx相关函数。先关掉避开。

打开mingw-w64 terminal终端（标题是Run Terminal），执行`build/mingw-w64.cmd`脚本。

## 0x4  编译 mingw-w64 版 ncnn-examples 工程

照理，做必要的准备工作：

**添加mingw-w64版opencv的dll所在目录到系统PATH环境变量**

在 ncnn-examples 工程中创建 build/mingw-w64.cmd，内容：
```batch
@echo off

set BUILD_DIR=mingw-w64
if not exist %BUILD_DIR% md %BUILD_DIR%
cd %BUILD_DIR%

cmake -G "CodeBlocks - MinGW Makefiles" ^
    -Dncnn_DIR="E:/dbg/cnn-deploy/ncnn/build/mingw-w64/install/lib/cmake/ncnn" ^
    -DOpenCV_DIR="e:/dbg/cnn-deploy/opencv-4.5.0/install/mingw-w64" ^
    ../..

mingw32-make.exe -j4
```

打开mingw-w64 terminal终端（标题是Run Terminal），执行`build/mingw-w64.cmd`脚本，

**用CLion打开工程**

用Clion import已经存在的工程，目录选择`e:/dbg/cnn-deploy/ncnn-examples/build/mingw-w64`目录，也就是前一步生成的目录，然后选择“cmake”类型的构建系统，就可以在CLion里面编辑代码运行工程了。

添加必要的模型文件，以及图像，然后可以运行yolov5.

![](mingw_ncnn_examples.png)


## 0x5 你可能遇到的报错以及原因

需要进入 mingw-w64 的terminal，而不是随便一个cmd：

否则你会遭遇报错（即使和CLion用了同样的调用cmake的命令，也会报错）：

```
G:\dev\opencv-4.5.0\cmake-build-debug>mingw-w64.cmd
-- The CXX compiler identification is unknown
-- The C compiler identification is unknown
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - failed
-- Check for working CXX compiler: e:/soft/mingw-w64/mingw64/bin/g++.exe
-- Check for working CXX compiler: e:/soft/mingw-w64/mingw64/bin/g++.exe - broken
CMake Error at E:/soft/cmake-3.19.1/share/cmake-3.19/Modules/CMakeTestCXXCompiler.cmake:59 (message):
  The C++ compiler

    "e:/soft/mingw-w64/mingw64/bin/g++.exe"

  is not able to compile a simple test program.

  It fails with the following output:

    Change Dir: G:/dev/opencv-4.5.0/cmake-build-debug/CMakeFiles/CMakeTmp

    Run Build Command(s):e:/soft/mingw-w64/mingw64/bin/mingw32-make.exe cmTC_19209/fast && e:/soft/mingw-w64/mingw64/bin/mingw32-make.exe  -f CMakeFiles\cmTC_19209.dir\build.make CMakeFiles/cmTC_19209.dir/build
    mingw32-make.exe[1]: Entering directory 'G:/dev/opencv-4.5.0/cmake-build-debug/CMakeFiles/CMakeTmp'
    Building CXX object CMakeFiles/cmTC_19209.dir/testCXXCompiler.cxx.obj
    e:\soft\mingw-w64\mingw64\bin\g++.exe    -o CMakeFiles\cmTC_19209.dir\testCXXCompiler.cxx.obj -c G:\dev\opencv-4.5.0\cmake-build-debug\CMakeFiles\CMakeTmp\testCXXCompiler.cxx
    mingw32-make.exe[1]: *** [CMakeFiles\cmTC_19209.dir\build.make:84: CMakeFiles/cmTC_19209.dir/testCXXCompiler.cxx.obj] Error 1
    mingw32-make.exe[1]: Leaving directory 'G:/dev/opencv-4.5.0/cmake-build-debug/CMakeFiles/CMakeTmp'
    mingw32-make.exe: *** [Makefile:139: cmTC_19209/fast] Error 2





  CMake will not be able to correctly generate this project.
Call Stack (most recent call first):
  CMakeLists.txt:109 (enable_language)


-- Configuring incomplete, errors occurred!
See also "G:/dev/opencv-4.5.0/cmake-build-debug/CMakeFiles/CMakeOutput.log".
See also "G:/dev/opencv-4.5.0/cmake-build-debug/CMakeFiles/CMakeError.log".
请按任意键继续. . .
```