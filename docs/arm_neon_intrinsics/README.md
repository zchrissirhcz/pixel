# arm neon intrinsics

intrinsics 可以简单的当作＂内置函数＂来理解．对应的函数声明在 arm_neon.h 文件中 (由 android ndk 或其他编译工具链内置).

**数据类型**

intrinsics 数据类型（固定长度的向量寄存器类型） [data_types](data_types.md)

**neon intrinsics 的性能与汇编的对比**

> The main disadvantage with intrinsics is that it is not possible to get the compiler to output exactly the code you want, so there is still some possibility of improvement when moving to NEON assembler code. 
(https://developer.arm.com/documentation/den0018/a/Compiling-NEON-Instructions/Intrinsics)

即: Intrinsics 在个别情况下和预期可能不一致，由编译器埋雷用户负责踩坑， 但 Intrinsics 通用且大部分时候 OK。

根据经验， ndk arm64 编译器生成的结果和手写汇编的差距不大，　ndk arm32 生成的结果则比手写汇编慢，　手动调流水容易看到效果．

## arm neon intrinsics 使用查询

### 官方文档查询页面
https://developer.arm.com/architectures/instruction-sets/intrinsics/

对于官方之外的整理版，　或多或少有明显能找到的错误， 用的时候需仔细．

### fbc 整理版
https://blog.csdn.net/fengbingchun/article/details/38085781

### EmSoftEn 整理版
https://blog.csdn.net/EmSoftEn/article/details/51718763

### rogerou 整理版
https://github.com/rogerou/Arm-neon-intrinsics

和 EmSoftEn 基本一样

### neon-guide 整理版
https://github.com/thenifty/neon-guide

### ChrisZZ 整理版
暂不对外提供

## arm neon intrinsics 参考实现

### neon2sse
https://github.com/intel/ARM_NEON_2_x86_SSE

### neon_ref
https://github.com/zchrissirhcz/pixel/tree/master/neon_ref

### neon_sim
暂不对外提供

## neon intrinsics 语法和实践
[neon_intrinsics.md](neon_intrinsics.md)

## 调试技巧
intrinsics 数据类型和 C 类型相互转换
    - 打印调试
    - [AOS和SOA的相互转换](https://www.bilibili.com/video/BV1zA41157fE)