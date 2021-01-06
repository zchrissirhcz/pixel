# VSCode 设置: 用于研读 ncnn arm 代码

[TOC]

怎样让 VSCode 识别 `__ARM_NEON`、`vld3_u8`等宏，并跳转到定义？

尝试了一番，勉强能用（跳转、识别__ARM_NEON宏），但不完美（armv8完美使用，armv7 with neon时 intrinsic 报红色），CLion能识别也能编译似乎是更好选择，neovim + coc.vim 似乎也值得尝试。

## 配置步骤
### 步骤1：cmake 推荐 >= 3.15

低于 3.9 也能用但不推荐。

可用 pip 快速安装：
```bash
pip install cmake
```
### 步骤2：生成 compile_commands.json 文件

CMakeLists.txt里添加`set(CMAKE_EXPORT_COMPILE_COMMANDS ON)`。

或调用cmake时传入`-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`。

Windows/Linux 平台 armeabi-v7a with neon 时会遇到 VSCode 不识别 `__ARM_NEON` 宏，在项目 CMakeLists.txt 添加如下内容即可：
```cmake
if (ANDROID_ARM_NEON)
    add_definitions(-D__ARM_NEON=1)
endif()
```

### 步骤3：VSCode 插件： C/C++ 和 CMake

CMake 插件用来给 `CMakeLists.txt` 和 `*.cmake` 做语法高亮。

C/C++ 插件用于提供 IntelliSense 做语法高亮、调试、函数跳转等。

### 步骤4：改 .vscode/c_cpp_properties.json

添加 NDK-r21b-armeabi-v7a 和 NDK-r21b-arm64-v8a 的配置项。

例如在Windows下我的配置：
```json
{
    "configurations": [
        {
            "name": "Win32",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [
                "_DEBUG",
                "UNICODE",
                "_UNICODE"
            ],
            "windowsSdkVersion": "10.0.18362.0",
            "compilerPath": "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/bin/Hostx64/x64/cl.exe",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "msvc-x64",
            "compileCommands": "${workspaceFolder}/build/vs2019-x64/compile_commands.json"
        },
        {
            "name": "NDK-r21b-arm64-v8a",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "E:/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe --target=aarch64-linux-android",
            "cStandard": "c11",
            "cppStandard": "c++11",
            "intelliSenseMode": "clang-arm64",
            "compileCommands": "${workspaceFolder}/build/android-arm64/compile_commands.json"
        },
        {
            "name": "NDK-r21b-armeabi-v7a",
            "includePath": [
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "E:/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe --target=armv7-none-linux-androideabi24",
            "cStandard": "c11",
            "cppStandard": "c++11",
            "intelliSenseMode": "clang-arm",
            "compileCommands": "${workspaceFolder}/build/android-arm32/compile_commands.json"
        }
    ],
    "version": 4
}
```

重要字段解释：

- `compilerPath`:编译器路径。对于NDK来说，同一个编译器有多种abi（armeabi-v7a, arm64-v8a, x86, x86_64等），指定`target`后才能让 Intelli Sense 正常工程
```json
//"compilerPath": "E:/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe", // 这样会报找不到 stdio.h 
```

- `"intelliSenseMode"`：告诉Intelli Sense使用哪种Parser

- `compileCommands`：指定 comile_commands.json 文件路径


然后VSCode里打开任意一个.h/.c/.cpp文件，右下角状态栏选择NDK-r21b

![](VSCode_UseNdk_1.png)

![](VSCode_UseNdk_3.png)

或者，关掉VSCode，删除.vscode目录，重新打开，过几秒会自动弹窗提示选择compile_commands.json文件：

![](VSCode_UseNdk_2.png)

只不过这种情况下，`c_cpp_properties.json`里的编译器可能还是x86平台的编译器而不是NDK里的。


## 一些问题排查
### 问题1：32位编译器不识别 `__ARM_NEON` 宏？

前面在 `.vscode/c_cpp_properties.json` 中创建的 NDK-r21b-armeabi-v7a 配置项，使用了`clang-arm`，在调用 `build/android-arm32-build.cmd` 生成 `compile_commands.json` 后，VSCode 仍不识别`__ARM_NEON`宏。

搜索了一番，大概确定了原因：IntelliSense 在非 MSVC 平台下，对编译器内置宏、内置include路径的识别，可能仍然存在bug。

我们考虑 `ANDROID` 这个宏，验证它是编译器内置的：

```bash
/e/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe --target=armv7-none-linux-androideabi24 -dM -E - < /dev/null | ag 'android'
```

结果中确实有 ANDROID，说明是编译器内置的：
```c++
#define __ANDROID_API__ 24
#define __ANDROID__ 1
#define __VERSION__ "4.2.1 Compatible Android (6317467 based on r365631c1) Clang 9.0.8 (https://android.googlesource.com/toolchain/llvm-project e0caee08e5f09b374a27a676d04978c81fcb1928)"
#define __clang_version__ "9.0.8 (https://android.googlesource.com/toolchain/llvm-project e0caee08e5f09b374a27a676d04978c81fcb1928)"
```

同时也在buitins里检索`neon`:
```
$ /e/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe --target=armv7-none-linux-androideabi24 -dM -E - < /dev/null | ag 'neon'
#define __ARM_NEON 1
#define __ARM_NEON_FP 0x4
#define __ARM_NEON__ 1
```


而在 Android NDK r21b 中，`cmake/build/android.toolchain.cmake`确显示地、再次定义了`ANDROID`宏：
```cmake
# Generic flags.
list(APPEND ANDROID_COMPILER_FLAGS
  -g
  -DANDROID
  -fdata-sections
  -ffunction-sections
  -funwind-tables
  -fstack-protector-strong
  -no-canonical-prefixes)
```

这解释了，为什么 build/android-arm64-build.cmd 生成的 compile_commands.json 中，单个.cpp的编译命令中，有`-DANDROID`:
```json
{
  "directory": "E:/github/ncnn-fork3/build/android-arm32",
  "command": "E:\\soft\\Android\\ndk-r21b\\toolchains\\llvm\\prebuilt\\windows-x86_64\\bin\\clang++.exe --target=armv7-none-linux-androideabi24 --gcc-toolchain=E:/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64 --sysroot=E:/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64/sysroot -I../../src/layer/arm -I../../src -Isrc -I../../src/layer -g -DANDROID -fdata-sections -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D__ARM_NEON -D_FORTIFY_SOURCE=2 -march=armv7-a -mthumb -Wformat -Werror=format-security   -Oz -DNDEBUG  -fPIC -DNCNN_STATIC_DEFINE -fopenmp -Wall -Wextra -Wno-unused-function -Ofast -ffast-math -fvisibility=hidden -fvisibility-inlines-hidden -fno-rtti -fno-exceptions -o src\\CMakeFiles\\ncnn.dir\\blob.cpp.o -c E:\\github\\ncnn-fork3\\src\\blob.cpp",
  "file": "E:/github/ncnn-fork3/src/blob.cpp"
},
```
于是，纯粹是为了 VSCode 看 ARM NEON 代码舒服、手动解决 IntelliSense 的bug，不妨在 `cmake/build/android.toolchain.cmake` 里添加 `__ARM_NEON` 宏的定义：

```cmake
# 这一段是本来就有的内容
# Generic flags.
list(APPEND ANDROID_COMPILER_FLAGS
  -g
  -DANDROID
  -fdata-sections
  -ffunction-sections
  -funwind-tables
  -fstack-protector-strong
  -no-canonical-prefixes)

# 以下是新增
if (ANDROID_ARM_NEON)
  list(APPEND ANDROID_COMPILER_FLAGS
    -D__ARM_NEON=1
  )
endif()
```

而如果随时换其他电脑上的环境，则手动在CMakeLists.txt里增加一句：
```cmake
if (ANDROID_ARM_NEON)
    add_definitions(-D__ARM_NEON=1)
endif()
```

P.S. 我在vscode 官方 issue 也提出问题了，https://github.com/microsoft/vscode-cpptools/issues/6742 ，还是比较期待 Intelli Sense 官方修复 armeabi-v7a with neon 的 parser。


### 问题2：arm neon intrinsic 跳转定义，有两个？

实际上 NDK armv7-eabi，一般用的时候，编译器内置了宏`__LITTLE_ENDIAN__`，定义等于1。

```
$ /e/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64/bin/clang++.exe --target=armv7-none-linux-androideabi24 -dM -E - < /dev/null | ag 'endian'
#define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#define __ORDER_BIG_ENDIAN__ 4321
#define __ORDER_LITTLE_ENDIAN__ 1234
#define __ORDER_PDP_ENDIAN__ 3412
```

因此，类似于 `__ARM_NEON`，这也是内置宏，也是没有被 Intelli Sense 识别。遗憾的是，这次不能通过在 当前项目的 CMakeLists.txt 中定义宏来让 Intelli Sense知道了。


### 问题3：消除红色波浪线

Linux和早些时候的Windows上，VSCode不识别`__fp16`和`vld3_u8`等关键字，显示为红色波浪下划线。

VSCode->文件->首选项->设置，搜索 `Intelli Sense Engine`，把 `Default` 改成 `Tag Parser`，红色波浪线就消失了！
![](VSCode_Intelli_Sense_Engine.jpg)

（其实是Intelli Sense的Parser的问题，https://github.com/microsoft/vscode-cpptools/issues/6506）

红色波浪线虽然消失带来新的问题：`__aarch64__`也被识别而不是灰色了。看来这不是我们需要的。


### 问题4：未知寄存器名"q0"

例如如下代码：
```c++
#include <arm_neon.h>
#include <iostream>
using namespace std;

static void print_uint8x8(uint8x8_t data) {
    static uint8_t p[8];

    // void vst1_u8 (uint8_t * ptr, uint8x8_t val)
    vst1_u8(p, data);
    for (int i=0; i<8; i++) {
        printf("%2d ", p[i]);
    }
    printf("\n");
}

int main() {
    uint8x8_t data1 = vdup_n_u8(255);
    print_uint8x8(data1);

    return 0;
}
```

这个问题是 Intelli Sense Parser 的问题，目前只能等官方修复。  帖子：https://github.com/microsoft/vscode-cpptools/issues/6506