# Learn ARM NEON step by step

## level 1: 环境搭建
- 设备准备： Android 手机 / 开发板
- 设备连接： 数据线， udev 规则， 开发者模式， 允许 USB 调试
- 程序构建： NDK， CMake， ninja
- 程序编辑： VSCode， CLion， Vim
- 程序运行： adb shell, `/data/local/tmp` 目录， PATH, LD_LIBRARY_PATH

## level 2: intrinsics
把 intrinsics 当做 C 函数来使用

- intrinsics 数据类型
- intrinsics 数据类型和 C 类型相互转换， 打印调试
- 简单的 C 代码翻译为 intrinsics

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