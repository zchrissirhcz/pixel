# improc

[TOC]

## rgb2gray

image size: h=4032, w=3024

| id | implementation | armv8 release | armv8 debug | armv7 release | armv7 debug |
| ---| -------------- | ---------- | ----------- | ------------ | ------------ |
| 1  | float       |  42 ms   |    |  57 ms  |   208 ms |
| 2  | fixed point[<sup>1</sup>](#refer-anchor-1) | 11 ms |   |  30 ms | 203 ms |
| 3  | fixed point + arm neon[<sup>2</sup>](#refer-anchor-2) | 10~13 ms |  | 9.7 ms | 87 ms |
| 4  | opencv 4.5.0 | 7~13 ms |  |  11 ms | 12 ms |
| 5  | asm     |    |       |     10 ms |  12 ms |
| 6  | float + assembly[<sup>3</sup>](#refer-anchor-3) | 14 ms | - | - | - |


### References

<div id="refer-anchor-1"></div>

- [1] [SSE图像算法优化系列一：一段BGR2Y的SIMD代码解析。](https://www.cnblogs.com/Imageshop/p/6261719.html)<div id="refer-anchor-2"></div>

- [2] [Tencent/ncnn - mat_pixel.cpp](https://github.com/Tencent/ncnn/blob/31bc57b1e293e726b318cf93fdcd6154f2188477/src/mat_pixel.cpp#L780-L790)<div id="refer-anchor-3"></div>

- [3] [Ldpe2G/ArmNeonOptimization](https://github.com/Ldpe2G/ArmNeonOptimization/blob/6f20bfd79327b1b1a5267880651ff2b31b6c15d6/armAssembly/assemblyEx2Rgb2Gray.cpp#L81-L227)

- [4] [【AI PC端算法优化】三，深入优化RGB转灰度图算法](https://zhuanlan.zhihu.com/p/129033980)

- [5] [-01-RGB彩色图像转换为灰度图像【ARM NEON加速】](https://blog.csdn.net/vacajk/article/details/56484061)

- [6] [A survery fast BGRA to grayscale conversion on iPhone](https://computer-vision-talks.com/2011-02-08-a-very-fast-bgra-to-grayscale-conversion-on-iphone/)
## rgb2bgr

image size: h=4032, w=3024

**rgb2bgr**

| id | implementation | armv8 release | armv8 debug |
| --- | -------------- | --------- | -------------|
| 1   | naive          | 20.3 ms|    190 ms |
| 2   | index optimized| 13/24 ms |  90 ms |
| 3   | neon intrinsic | 33 ms  |    270 ms |
| 4   | asm            | 19 ms  |  19 ms |
| 5   | opencv         | 16 ms  |  18 ms |

**rgb2bgr_inplace**

| id | implementation | armv8 release | armv8 debug | 备注 |
| --- | -------------- | --------- | ---------- | -------- |
| 1   | naive          | 10 ms    |   134 ms |  - |
| 2   | naive2         | 11 ms    |    43 ms |  - |
| 3   | asm            | 5.5 ms   |    6 ms  |  **比OpenCV快5倍** |
| 4   | opencv         | 27 ms    |    29 ms | - |

### References

- [性能优化篇（4）：NEON优化案例——图像颜色转换之RGB到BGR（aarch64版）](https://blog.csdn.net/wohenfanjian/article/details/103407259)

## Neon Links

**Intrinscs**:

- [Neon Intrinsics查询 - 官方在线版](https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/intrinsics)

- [Neon Intrinsics查询 - fengbingchun整理版，部分带sse等价解释](https://blog.csdn.net/fengbingchun/article/details/38085781)，部分intrinsics缺失

- [Neon Intrinsics - 按类型整理 - EmSoftEn的博客](https://blog.csdn.net/emsoften/article/details/51718763)

- [gcc在线文档 - ARM-NEON-Intrinsics](https://gcc.gnu.org/onlinedocs/gcc/ARM-NEON-Intrinsics.html) 已失效，gcc最新版已经找不到 ARM NEON Intrinsics了

- [gcc-4.8.4 - ARM-NEON-Intrinsics](https://gcc.gnu.org/onlinedocs/gcc-4.8.4/gcc/ARM-NEON-Intrinsics.html#ARM-NEON-Intrinsics)，可访问

**Assembler**:

- [arm汇编入门帖 - asm volatile形式 - Roland_Sun的博客](https://blog.csdn.net/Roland_Sun/article/details/42921131)

- [arm内联汇编入门(上) - 梁德澎](https://aijishu.com/a/1060000000116427)

- [arm内联汇编入门(下) - 梁德澎](https://aijishu.com/a/1060000000116431)

- [ARM GCC 内联汇编参考手册 - 中文翻译版](https://github.com/tidyjiang8/arm-gcc-inline-assembler/blob/master/src/arm-gcc-inline-assembler.md)

- [gcc在线文档 - 在C中使用汇编](https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html#Using-Assembly-Language-with-C)

- [arm neon常用汇编指令 - 章小龙](https://zhuanlan.zhihu.com/p/64025085)

| 指令类型 | A32/T32  |    A64   |
| -------- | -------- |--------- |
| 访存预取 | pld [%0, #192] |prfm pldl1keep, [%0, #192] |
| 访存读   | vld1.s32 {q0}, [%0]! | ld1 {v1.4s}, [%0], #16 |
| 访存写   | vst1.s32 {q0}, [%0]! | st1 {v1.4s}, [%0], #16 |
| 浮点计算 | vmla.f32 q10, q14, %f18[0]<br/>vmul.f32 q0, q8, %e18[1] | fmla v6.4s, v8.4s, %18.s[0] <br/>fmul v21.4s, v21.4s, %18.s[1] |
| 整型计算 | vmull.s16 q10, %P6, d0[0]<br/>vmlal.s16 q13, %P6, d0[1] | Smull v10.4s, %6.4h, v0.h[0]<br/>Smull2 v10.4s, %6.8h, v0.h[0]<br/>smlal v13.4s, %6.4h, v0.h[1]<br/>smlal2 v5.4s, %6.8h, v1.h[0] |
| 搬运常数<br/>到寄存器 | vmov.f32 q6, #6.0<br/>vmov.s32 q7, #0| FMOV v11.4s, #6.0<br/>MOVI v10.4s, #0 |
| 复制<br/>寄存器 | vdup.f32 q6, d0[0]<br/>vdup.f32 q6,r1|DUP v6.4s,v0.s[0]<br/>DUP v6.4s, w1|
| 浮点最大最小值 | vmin.f32 q0, q0, q6<br/>vmax.f32 q0, q0, q7| FMAX v0.4s, v0.4s, v10.4s<br/>FMIN v0.4s, v0.4s, v11.4s|
| 转置 | vtrn.s32 q0, q1<br/>vtrn.s32 q2, q3 | TRNI v5.4s, v7.4s, v8.4s<br/> TRN2 v6.4s, v7.4s, v8.4s |
| 减法 | subs %0, #1 | subs %w0, %w0, #1 |
| 加法 | Add %0, #16<br/>vadd.f32 q10, q10, q4|Add %x0, %x0, #16<br/>FADD V10.4S, V10.4S, v4.4s|
|S8_2_s16 |vmovl.s8 q1, d0<br/>vmovl.s8 q1, d1| SSHLL V1.8H,V0.8B,#0<br/>SSHLL2 V1.8H,V0.16B,#0 |
|s32_2_f32 | vcvt.f32.s32 q10, q10| scvtf v20.4s, v20.4s |
|s32_2_s32 | vcvtr.s32.f32 s0, s0 | fcvtas v20.4s, v20.4s|
|s32_2_s16 | vqmovn.s32 d20, q0 | sqxtn v7.4h, v20.4s<br/>sqxtn2 v7.8h, v21.4s|


**全能系列**

- [AI移动端优化 - 知乎 - 章小龙 & BBuf & 圈圈虫等](https://www.zhihu.com/people/zxloas/posts)

- [arm官方文档 armv8-a 系列教程](https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/neon-programmers-guide-for-armv8-a)

- [armv7 mix assembly and intrinsic（汇编和intrinsic绑定）- ncnn文档](https://github.com/Tencent/ncnn/wiki/armv7-mix-assembly-and-intrinsic)

- [aarch64 mix assembly and intrinsic（汇编和intrinsic绑定）- ncnn文档](https://github.com/Tencent/ncnn/wiki/aarch64-mix-assembly-and-intrinsic)

- [arm a53 a55 双发射问题 - ncnn文档](https://github.com/Tencent/ncnn/wiki/arm-a53-a55-dual-issue)

**TODO系列**

- 查询 armv7 所有汇编指令？
- 查询 armv8 所有汇编指令？

## Neon Notes

1. arm64 汇编和 arm32 汇编，写法有差别，要分开写。

2. 汇编里的自增(+=)操作：
   - arm32是`!`，例如 `[%1]!`；
   - arm64指定增加的字节数，例如 st3 存储后，自增 48 字节：
   ```asm
   st3    { v0.16b, v1.16b, v2.16b }, [%0], #48
   ```

3. 如何使用 `float32x4x4_t` 这样的“长”类型：
    把它当做是数组，随后访问`.val[i]`字段：
    ```c++
    typedef struct float32x4x4_t {
        float32x4_t val[4];
    } float32x4x4_t;
    ```

4. arm32不支持double，arm64才支持，从`${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/9.0.8/include/arm_neon.h`知道：
```c++
typedef float float32_t;
typedef __fp16 float16_t;
#ifdef __aarch64__
typedef double float64_t;                                                                                                          
#endif
```

5. `poly8_t`和`poly16_t`这样的类型，常规计算用不到，它们是[为加密算法和CRC哈希校验准备的](https://stackoverflow.com/questions/22224282/arm-neon-and-poly8-t-and-poly16-t)。

6. VFP寄存器armv7架构包含16个VFP寄存器（32bit），S0-S15。

7. NEON和VFP的区别: VFP是加速浮点计算的硬件不具备数据并行能力，同时VFP更尽兴双精度浮点数（double）的计算，NEON只有单精度浮点计算能力。更多请参考 [stackoverflow:neon vs vfp](https://link.zhihu.com/?target=http%3A//stackoverflow.com/questions/4097034/arm-cortex-a8-whats-the-difference-between-vfp-and-neon)

8. 区分arm32和arm64的汇编指令：带了前缀v的就是Armv7 32bit指令的标志，不带v的是arm64-v8a的。例如：
```
"vld3.u8    {d0-d2}, [%1]       \n"           // armv7汇编

"ld3    { v0.16b, v1.16b, v2.16b }, [%1]\n"   // armv8汇编
```

9. 关于 ARM NEON 和 VFP 有一些有趣的事实：
    - 很多 arm 处理器都会带一个 DSP，或者类似的定制的硬件处理单元；DSP上没有系统，调试、同步都更加麻烦。
    - ARMv7也不是全支持neon，也不是全支持vfp。如果有neon而没有vfp，那么不能做硬浮点。
    - NEON SIMD比VFP的计算更高效（因为位数多了？），因而从ARMv7开始，VFP被弃用。VFP单元有时候也被叫做FPU，浮点计算单元。
    - 有NEON或VFP的情况下，也不一定有fp16和fma扩展功能
    - VFP的浮点数是严格IEEE754标准；NEON则不是严格IEEE754标准
    - VFP的个别指令，是NEON无法替代的

10. NEON指令和浮点指令，使用的是相同的寄存器