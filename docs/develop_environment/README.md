# arm neon develop environment

## 写代码
- 程序构建： 基于 NDK 的交叉编译(CMake， ninja)
- 程序编辑： VSCode 为主。 可选： Vim, Kate, CLion


## 连接真机
真机能够基本上反映出实际运行速度．

- 设备准备： Android 手机为主。可选： ARM 开发板 / Apple M1 等 CPU 为 ARM-A 系列的设备．
- 设备连接： 
    - Windows/mac: 手机上开启开发者模式，并运行USB调试, 数据线连接当前电脑（第一次时需同意调试）
    - Linux: 添加 udev 规则文件; 选择"文件传输模式"
- 程序运行： 
    - adb 命令 (pull, push, shell)
    - 目录: 可执行文件放在　`/data/local/tmp`，　无需 root
    - 目录: 其他目录，需要 root 过
    - 可选: PATH, LD_LIBRARY_PATH 的设置


## 真机之外的选择
android 虚拟机．

qemu.

在 PC (x86) 平台，　通过使用 neon2sse.h 替代 arm_neon.h ，　将 neon intrinsics 翻译为 sse intrinsics, 从而在 PC 上编译和运行．
优点是方便调试　intrinsics, 缺点是无法运行 arm 汇编，　并且有些语法不严格相同导致 PC　上能编译和运行，　在设备上可能编译不过，或运行结果不对．