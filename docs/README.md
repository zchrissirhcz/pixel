# Learn ARM NEON step by step

## level 1: 环境搭建
- 设备准备： Android 手机为主。可选： ARM 开发板 / Apple M1
- 设备连接： 数据线， udev 规则， 开发者模式， 允许 USB 调试
- 程序构建： NDK， CMake， ninja
- 程序编辑： VSCode 为主。 可选： Vim, Kate, CLion
- 程序运行： adb shell, `/data/local/tmp` 目录. 可选: PATH, LD_LIBRARY_PATH

## level 2: intrinsics
把 intrinsics 当做 C 函数来使用

- intrinsics 数据类型
- intrinsics 数据类型和 C 类型相互转换， 打印调试
- 简单的 C 代码翻译为 intrinsics

> The main disadvantage with intrinsics is that it is not possible to get the compiler to output exactly the code you want, so there is still some possibility of improvement when moving to NEON assembler code. 
(https://developer.arm.com/documentation/den0018/a/Compiling-NEON-Instructions/Intrinsics)

Intrinsics 在个别情况下和预期可能不一致，由编译器埋雷用户负责踩坑， 但 Intrinsics 通用且大部分时候 OK。

## level 3: vector registers
- arm 的向量寄存器
- arm 的普通寄存器
- 数据类型和寄存器
- inline assembly 基本写法

## level 3: debugging
- 保留/去除符号信息
- Address Sanitizer
- Valgrind
- overlook.cmake

## level 4: 汇编
- intrinsics 和 arm32 汇编的相互转换
- intrinsics 和 arm64 汇编的相互转换

## level 5: optimization
- Profile, autotimer
- Release 模式
- 多发射， 流水重排