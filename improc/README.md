# improc

Image processing algorithm implementations.

[TOC]

## rgb2gray

### Benchmark
image size: h=4032, w=3024
android device: XiaoMI8(QCOM845)
OpenCV 4.5.1

| id | implementation            | PC Release    | armv8 release | armv8 debug | armv7 release | armv7 debug |
| ---| ------------------------- | ------------- | ------------- | ----------- | ------------  | ----------- |
| 1  | float加权                 |     25 ms     |     42 ms     |   157 ms    |    59 ms      |   152 ms    |
| 2  | opencv                    |      2 ms     |   7-18 ms     | 10-20 ms    |    11 ms      |    11 ms    |
| 3  | 定点加权                  |     20 ms     |     11 ms     |   128 ms    |    30 ms      |   119 ms    |
| 4  | neon intrinsic(定点加权)  |     -         |  10~13 ms     |    81 ms    |   9.5 ms      |    80 ms    |
| 5  | neon asm(定点加权)        |     -         |    9.4 ms     |   9.4 ms    |    10 ms      |    10 ms    |
| 6  | float平均                 |      9 ms     |    9.4 ms     |    63 ms    |    66 ms      |    66 ms    |

`gray = (r+g+b)/3`在armv8上加速明显。

### References

<div id="refer-anchor-1"></div>

- [1] [SSE图像算法优化系列一：一段BGR2Y的SIMD代码解析。](https://www.cnblogs.com/Imageshop/p/6261719.html)<div id="refer-anchor-2"></div>

- [2] [Tencent/ncnn - mat_pixel.cpp](https://github.com/Tencent/ncnn/blob/31bc57b1e293e726b318cf93fdcd6154f2188477/src/mat_pixel.cpp#L780-L790)<div id="refer-anchor-3"></div>

- [3] [Ldpe2G/ArmNeonOptimization](https://github.com/Ldpe2G/ArmNeonOptimization/blob/6f20bfd79327b1b1a5267880651ff2b31b6c15d6/armAssembly/assemblyEx2Rgb2Gray.cpp#L81-L227)

- [4] [【AI PC端算法优化】三，深入优化RGB转灰度图算法](https://zhuanlan.zhihu.com/p/129033980)

- [5] [-01-RGB彩色图像转换为灰度图像【ARM NEON加速】](https://blog.csdn.net/vacajk/article/details/56484061)

- [6] [A survery fast BGRA to grayscale conversion on iPhone](https://computer-vision-talks.com/2011-02-08-a-very-fast-bgra-to-grayscale-conversion-on-iphone/)
## rgb2bgr

### Benchmark

image size: h=4032, w=3024
android device: XiaoMI8(QCOM845)
OpenCV 4.5.1

**rgb2bgr**

| id | implementation | armv8 release | armv8 debug  | armv7 release | armv7 debug |
| --- | -------------- | ------------ | -------------| ------------- | ----------- |
| 1   | naive          |  25 ms       |  26 ms       |  27 ms        |  117 ms     |
| 2   | index optimized|  19 ms       |  18 ms       |  25 ms        |  92 ms      |
| 3   | neon intrinsic |  19 ms       |  19 ms       |  19 ms        |  41 ms      |
| 4   | asm            |  19 ms       |  18 ms       |  20 ms        |  22 ms      |
| 5   | opencv         |  15 ms       |  14 ms       |  15 ms        |  29 ms      |

**rgb2bgr_inplace**

| id | implementation | armv8 release | armv8 debug | armv7 release | armv7 debug | 备注    |
| --- | --------------| ------------- | ----------- | ------------- | ----------- | ------- |
| 1   | naive         | 12 ms         |   12 ms     |  14 ms        |  88 ms      |         |
| 2   | naive2        | 12 ms         |   12 ms     |  18 ms        |  40 ms      |         |
| 3   | asm           | 6 ms          |   6 ms      |  6 ms         |  6 ms       |  **比OpenCV快4~5倍** |
| 4   | opencv        | 26 ms         |   24 ms     |  30 ms        |  33 ms      |         |

为什么 OpenCV 的**原地** rgb2bgr 比 naive 实现都要慢 5 倍？它的实现在 `3rdparty/carotene/hal/tegra_hal.hpp`：

```c++
#define TEGRA_CVTBGRTOBGR(src_data, src_step, dst_data, dst_step, width, height, depth, scn, dcn, swapBlue) \
( \
    depth == CV_8U && CAROTENE_NS::isSupportedConfiguration() ? \
        scn == 3 ? \
            dcn == 3 ? \
                swapBlue ? \
                    parallel_for_(Range(0, height), \
                    TegraCvtColor_rgb2bgr_Invoker(src_data, src_step, dst_data, dst_step, width, height), \
                    (width * height) / static_cast<double>(1<<16)), \
                    CV_HAL_ERROR_OK : \
                    CV_HAL_ERROR_NOT_IMPLEMENTED : \
            dcn == 4 ? \
                (swapBlue ? \
                    parallel_for_(Range(0, height), \
                    TegraCvtColor_rgb2bgrx_Invoker(src_data, src_step, dst_data, dst_step, width, height), \
                    (width * height) / static_cast<double>(1<<16)) : \
                    parallel_for_(Range(0, height), \
                    TegraCvtColor_rgb2rgbx_Invoker(src_data, src_step, dst_data, dst_step, width, height), \
                    (width * height) / static_cast<double>(1<<16)) ), \
                CV_HAL_ERROR_OK : \
            CV_HAL_ERROR_NOT_IMPLEMENTED : \
        scn == 4 ? \
            dcn == 3 ? \
                (swapBlue ? \
                    parallel_for_(Range(0, height), \
                    TegraCvtColor_rgbx2bgr_Invoker(src_data, src_step, dst_data, dst_step, width, height), \
                    (width * height) / static_cast<double>(1<<16)) : \
                    parallel_for_(Range(0, height), \
                    TegraCvtColor_rgbx2rgb_Invoker(src_data, src_step, dst_data, dst_step, width, height), \
                    (width * height) / static_cast<double>(1<<16)) ), \
                CV_HAL_ERROR_OK : \
            dcn == 4 ? \
                swapBlue ? \
                    parallel_for_(Range(0, height), \
                    TegraCvtColor_rgbx2bgrx_Invoker(src_data, src_step, dst_data, dst_step, width, height), \
                    (width * height) / static_cast<double>(1<<16)), \
                    CV_HAL_ERROR_OK : \
                    CV_HAL_ERROR_NOT_IMPLEMENTED : \
            CV_HAL_ERROR_NOT_IMPLEMENTED : \
        CV_HAL_ERROR_NOT_IMPLEMENTED \
    : CV_HAL_ERROR_NOT_IMPLEMENTED \
)
```
进而会调用`CAROTENE_NS::rgb2bgr`，它的实现在 `3rdparty/carotene/src/colorconvert.cpp`:
```c++
void rgb2bgr(const Size2D &size,
             const u8 * srcBase, ptrdiff_t srcStride,
             u8 * dstBase, ptrdiff_t dstStride)
{
    internal::assertSupportedConfiguration();
#ifdef CAROTENE_NEON
#if !(!defined(__aarch64__) && defined(__GNUC__) && defined(__arm__))
    size_t roiw16 = size.width >= 15 ? size.width - 15 : 0;
#endif
    size_t roiw8 = size.width >= 7 ? size.width - 7 : 0;

    for (size_t i = 0u; i < size.height; ++i)
    {
        const u8 * src = internal::getRowPtr(srcBase, srcStride, i);
        u8 * dst = internal::getRowPtr(dstBase, dstStride, i);
        size_t sj = 0u, dj = 0u, j = 0u;


#if !defined(__aarch64__) && defined(__GNUC__) && defined(__arm__)
        for (; j < roiw8; sj += 24, dj += 24, j += 8)
        {
            internal::prefetch(src + sj);
            __asm__ (
                "vld3.8 {d0, d1, d2}, [%[in0]]             \n\t"
                "vswp d0, d2                               \n\t"
                "vst3.8 {d0, d1, d2}, [%[out0]]            \n\t"
                : /*no output*/
                : [out0] "r" (dst + dj),
                  [in0]  "r" (src + sj)
                : "d0","d1","d2"
            );
        }
#else
        for (; j < roiw16; sj += 48, dj += 48, j += 16)
        {
            internal::prefetch(src + sj);
            uint8x16x3_t vals0 = vld3q_u8(src + sj);

            std::swap(vals0.val[0], vals0.val[2]);

            vst3q_u8(dst + dj, vals0);
        }

        if (j < roiw8)
        {
            uint8x8x3_t vals = vld3_u8(src + sj);
            std::swap(vals.val[0], vals.val[2]);
            vst3_u8(dst + dj, vals);
            sj += 24; dj += 24; j += 8;
        }
#endif

        for (; j < size.width; ++j, sj += 3, dj += 3)
        {
            u8 b = src[sj + 2];//Handle src == dst case
            dst[dj + 2] = src[sj    ];
            dst[dj + 1] = src[sj + 1];
            dst[dj    ] = b;
        }
    }
#else
    (void)size;
    (void)srcBase;
    (void)srcStride;
    (void)dstBase;
    (void)dstStride;
#endif
}
```
经打印验证，确实定义了`CAROTENE_NEON`宏，NEON相关代码也很直白不会导致降速。注释掉整个函数，最终耗时从14ms降低为12ms，看起来还是外部的调用导致的。。。在哪里呢？？

排除了“switch里case太多导致慢”的可能，只调用`cvtColorBGR2BGR(_src, _dst, dcn, swapBlue(code));`仍然慢；

modules/imgproc/src/color.simd_helpers.hpp
```c++
        if (_src.getObj() == _dst.getObj()) {
            dst = _dst.getMat();
            printf("--- helper case 1\n");
        }
        else {
            _dst.create(dstSz, CV_MAKETYPE(depth, dcn));
            dst = _dst.getMat();
            printf("--- helper case 2\n");
        }
```

https://github.com/opencv/opencv/pull/6760

原因找到了，做了 copyTo 导致的。需要研究一下 copyTo 的源码，为啥这么慢？alalek 说是为了避免编译器优化，我表示没有理解：到底要检查 InputArray/OutputArray 的 obj 是否相等（判断的是两个Mat是否相等），还是应该判断两个 Mat 的 data 是否相等？

https://github.com/opencv/opencv/pull/7819

反正，如果只是为了 cpu 上跑 rgb2bgr 的 inplace 操作，假设不会遇到编译优化问题，那么可以这样写：
```c++
cv::Mat image = cv::read("rgb.png");
cv::Mat image_copy = image;
cv::cvtColor(image_copy, image, cv::COLOR_BGR2RGB);
```
则**耗时从 14ms 减少到 不到 3ms。**

Note: 在 OpenCL 编程中， `image2d_t` 和 `image3d_t` 类型，需要有 `__read_only` 和 `__write_only` 修饰（但不能全都有），这导致 `cvtColor()` 需要检查冲突。

### References

- [性能优化篇（4）：NEON优化案例——图像颜色转换之RGB到BGR（aarch64版）](https://blog.csdn.net/wohenfanjian/article/details/103407259)

- [Arm blog - coding-for-neon---part-1-load-and-stores](https://community.arm.com/developer/ip-products/processors/b/processors-ip-blog/posts/coding-for-neon---part-1-load-and-stores)

- [intrinsics-neon-swap-elements-in-vector - StackOverFlow](https://stackoverflow.com/questions/39514952/intrinsics-neon-swap-elements-in-vector/39519421#39519421)

- [On iOS how to quickly convert RGB24 to BGR24?](https://stackoverflow.com/a/11684331/2999096)


## threshold
### Benchmark

image size: h=4032, w=3024
android=XiaoMI8(QCOM845),NDK-r21b
PC=i5-8500, ubuntu20.04, clang11.0
OpenCV 4.5.1

**threshold_gray**

input is gray, output is gray.

| id | implementation | PC release | armv8 release | armv8 debug | armv7 release | armv7 debug |
| -- | ------------   | ---------  | ------------  | ----------- | ------------  | ----------- |
| 1  | naive          |  4 ms      |     6 ms      |    62 ms    |   19 ms       |     58 ms   |
| 2  | opencv         |  1~2 ms    |     6 ms      |     6 ms    |   6 ms        |     6 ms    |
| 3  | neon intrinsic |  -         |     6 ms      |    25 ms    |   6 ms        |     25 ms   |
| 4  | neon asm       |  -         |     7 ms      |     7 ms    |   7 ms        |     7 ms    |

**threshold_rgb**

input is rgb, output is gray

| id | implementation | PC release | armv8 release | armv8 debug | armv7 release | armv7 debug |
| -- | ------------   | ---------  | ------------  | ----------- | ------------  | ----------- |
| 1  | naive, 浮点    |  28 ms     |  50 ms        |   123 ms    |     42 ms     |  141 ms    |
| 2  | opencv （两步）|  3~8 ms    |  23 ms        |    23 ms    |     25 ms     |   25  ms   |
| 3  | naive, 取平均  |  15 ms     |  15 ms        |   107 ms    |    217 ms     |   102 ms   |
| 4  | naive, 定点化  |  21 ms     |  18 ms        |   103 ms    |     24 ms     |   105 ms   |
| 5  | neon intrinsic(定点) |  -   |  10 ms        |   113 ms    |     11 ms     |   110 ms   |
| 7  | neon asm (定点)      |  -   |               |             |     10 ms     |    10 ms   |


## histogram

### Benchmark

**histogram_gray**

image size: h=4032, w=3024
android=XiaoMI8(QCOM845),NDK-r21b
PC=i5-8500, ubuntu20.04, clang11.0
OpenCV 4.5.1

| id | implementation | PC release | armv8 release | armv7 release |
| -- | -------------- | ---------- | ------------- | ------------- |
| 1  |  naive         |   5 ms     |    18 ms      |     21 ms     |

**histogram_rgb**

| id | implementation | PC release | armv8 release | armv7 release |
| -- | -------------- | ---------- | ------------- | ------------- |
| 1  |naive, gray(平均)|   11 ms   |   58 ms       |     62 ms     |
| 2  |  naive, r/g/b  |   6  ms    |   18 ms       |     21 ms     |

## flip

### Benchmark

**flip horizontally, rgb**

image info: height=4032, width=3024

涉及指令为 vrev64_u8

| id | name   | armv7 debug |  armv7 release | armv8 release |
| ---| -----  |-----------  | -------------- | ------------- |
| 1  | opencv |  42.5385 ms |  42.4799 ms    |  24.1896 ms   |
| 2  | naive  | 157.2149 ms |  24.7365 ms    |  17.3522 ms   |
| 3  | idxopt | 141.7304 ms |  26.1346 ms    |  26.0233 ms   |
| 4  | asimd  |  50.3773 ms |  21.0564 ms    |  21.3476 ms   |

尝试实现 rgb 图的水平翻转，比较naive的实现，发现armv8编译器O2, 速度比手写neon intrinsic快一点。

**flip horizontally, gray**

| id | name   | armv8 release | armv7 release |
| -- | ------ | ------------- | ------------- |
| 1  | opencv | 10.4322 ms    |   7.4855 ms   |
| 2  | naive  |  8.6991 ms    |  13.9475 ms   |
| 3  | asimd  |  4.2636 ms    |   4.4192 ms   |


**flip vertically, rgb**

| id | name    | armv8 release | armv7 release |
| -- | ------  | ------------- | ------------- |
| 1 | opencv   | 11.5798 ms    | 11.6609 ms    |
| 2 | naive    | 10.9293 ms    | 21.6688 ms    |
| 3 | by lines | 11.1698 ms    | 11.0645 ms    |
| 4 | opencv inplace |  3.8 ms | -             |

**flip vertically, gray**

| id | name    | armv8 release | armv7 release |
| -- | ------ | ------------- | ------------- |
| 1 | opencv   | 3.9922 ms     | 4.0637 ms     |
| 2 | naive    | 3.8403 ms     | 11.7752 ms    |
| 3 | by lines | 3.8624 ms     | 3.5497 ms     |


TODO: flip inplace

### References

- [NEON加速之memcpy在ARM平台的优化](https://www.jianshu.com/p/7b3bfc3aed12), flip vert by lines的潜在优化方案

## sobel

**References**

- [【AI PC端算法优化】四，一步步将Sobel边缘检测加速22倍](https://zhuanlan.zhihu.com/p/131319128)

## magnitude

TBD


## boxfilter

TBD
### References

- [移动端arm cpu优化学习笔记----一步步优化盒子滤波（Box Filter）](https://zhuanlan.zhihu.com/p/64522357)

- [一份朴实无华的移动端盒子滤波算法优化笔记](https://zhuanlan.zhihu.com/p/170611395)

- [基于NCNN的3x3可分离卷积再思考盒子滤波](https://zhuanlan.zhihu.com/p/181734867)

## Neon Links

### Examples

[rgba2gray - neon](https://github.com/carlj/NEON-ASM-BGRA-to-Grayscale-conversion/blob/master/Classes/NEON_ASMViewController.m)

### Intrinscs

- [Neon Intrinsics查询 - 官方在线版](https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/intrinsics)
 
- [Neon Intrinsics查询 - fengbingchun整理版，部分带sse等价解释](https://blog.csdn.net/fengbingchun/article/details/38085781)，部分intrinsics缺失

- [Neon Intrinsics - 按类型整理 - EmSoftEn的博客](https://blog.csdn.net/emsoften/article/details/51718763)

- [gcc在线文档 - ARM-NEON-Intrinsics](https://gcc.gnu.org/onlinedocs/gcc/ARM-NEON-Intrinsics.html) 已失效，gcc最新版已经找不到 ARM NEON Intrinsics了

- [gcc-4.8.4 - ARM-NEON-Intrinsics](https://gcc.gnu.org/onlinedocs/gcc-4.8.4/gcc/ARM-NEON-Intrinsics.html#ARM-NEON-Intrinsics)，可访问

### Assembler

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
| 复制<br/>寄存器 | vdup.f32 q6, d0[0]<br/>vdup.f32 q6,r1(似乎写错了)|DUP v6.4s,v0.s[0]<br/>DUP v6.4s, w1|
| 浮点最大最小值 | vmin.f32 q0, q0, q6<br/>vmax.f32 q0, q0, q7| FMAX v0.4s, v0.4s, v10.4s<br/>FMIN v0.4s, v0.4s, v11.4s|
| 转置 | vtrn.s32 q0, q1<br/>vtrn.s32 q2, q3 | TRNI v5.4s, v7.4s, v8.4s<br/> TRN2 v6.4s, v7.4s, v8.4s |
| 减法 | subs %0, #1 | subs %w0, %w0, #1 |
| 加法 | Add %0, #16<br/>vadd.f32 q10, q10, q4|Add %x0, %x0, #16<br/>FADD V10.4S, V10.4S, v4.4s|
|S8_2_s16 |vmovl.s8 q1, d0<br/>vmovl.s8 q1, d1| SSHLL V1.8H,V0.8B,#0<br/>SSHLL2 V1.8H,V0.16B,#0 |
|s32_2_f32 | vcvt.f32.s32 q10, q10| scvtf v20.4s, v20.4s |
|s32_2_s32 | vcvtr.s32.f32 s0, s0 | fcvtas v20.4s, v20.4s|
|s32_2_s16 | vqmovn.s32 d20, q0 | sqxtn v7.4h, v20.4s<br/>sqxtn2 v7.8h, v21.4s|

- [Confusion about different clobber description for arm inline assembly - StackOverFlow](https://stackoverflow.com/questions/65056624/confusion-about-different-clobber-description-for-arm-inline-assembly)


### 全能系列

- [移动端arm cpu优化学习笔记第4弹--内联汇编入门](https://zhuanlan.zhihu.com/p/143328317)

- [ARM Neon Intrinsics 学习指北：从入门、进阶到学个通透](https://zhuanlan.zhihu.com/p/358603760)

- [AI移动端优化 - 知乎 - 章小龙 & BBuf & 圈圈虫等](https://www.zhihu.com/people/zxloas/posts)

- [arm官方文档 armv8-a 系列教程](https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/neon-programmers-guide-for-armv8-a)

- [armv7 mix assembly and intrinsic（汇编和intrinsic绑定）- ncnn文档](https://github.com/Tencent/ncnn/wiki/armv7-mix-assembly-and-intrinsic)

- [aarch64 mix assembly and intrinsic（汇编和intrinsic绑定）- ncnn文档](https://github.com/Tencent/ncnn/wiki/aarch64-mix-assembly-and-intrinsic)

- [arm a53 a55 双发射问题 - ncnn文档](https://github.com/Tencent/ncnn/wiki/arm-a53-a55-dual-issue)

- [NEON™ Programmer's Guide 1.0 (armv7 neon文档)](https://developer.arm.com/documentation/den0018/a/)

### 优化的上限在哪里？

- [浮点峰值那些事儿](https://zhuanlan.zhihu.com/p/28226956)

- [如何判断算法是否有可优化空间？](https://mp.weixin.qq.com/s?__biz=MzA4MjY4NTk0NQ==&mid=2247490615&idx=1&sn=4fca4b963adcba01553371c6d566a597&scene=21)

- [性能优化篇（4）：NEON优化案例——图像颜色转换之RGB到BGR（aarch64版）](https://blog.csdn.net/wohenfanjian/article/details/103407259)

### TODO系列

- 查询 armv7 所有 neon intrinsic 对应的汇编指令语法，见 [Neon Programmer's Guide 1.0 (DEN0018A_neon_programmers_guide.pdf)](https://developer.arm.com/documentation/den0018/a/)
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

4. <del>arm32不支持double，arm64才支持</del> arm32/64都支持double；但float保存>=16777216的整数时可能不准确，考虑用double或int64/uint64。

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

11. 一些指令的含义：

- `asr`: arithmetic shift right, store the last bit shifted out in the carry flag(if instruction uses the S suffix)
- `lsl`: logical shift left, store the last bit shifted out in the carry flag (if instruction uses the S suffix)
- `lsr`: logical shift right, store the last bit shifted out in the carry flag(if instruction uses the S suffix)
- `ror`: rotate right, place original bit n - 1 into the carry flag
- `rrx`: rotate right exactly one bit (this operation does not accept a shift amount), treat the register as a 33-bit register with the carry flag acting as the LSB
- `ldr`: 从内存读取到寄存器: `ldr <register> <memory address>`
- `str`: 从寄存器写入到内存: `str <register> <memory address>`
- 更多arm汇编基本指令，见《Embedded Systems - ARM Programming and Optimization》第一章最后一节(P50)。

12. 用来区分arm neon, armv7, armv8, armv8.2的宏

>2020年开始，新手机 CPU 几乎都是 armv8.2 架构，这个架构引入了新的 fp16 运算和 int8 dot 指令，优化得当就能大幅加速深度学习框架的推理效率。类似于 x86 CPU 的 AVX，不是全部 CPU 都支持，得考虑兼容性，做到老CPU上用老指令，新CPU用新指令。

```
#if __ARM_NEON
    #if __aarch64__
        #if __ARM_FEATURE_FP16_VECTOR_ARITHMETIC
            printf("armv8.2\n");
        #else
            printf("armv8\n");
        #endif
    #else
        printf("armv7\n");
    #endif
#endif // __ARM_NEON
```

Ref: [ncnn avx2/armv8.2 基础架构](https://zhuanlan.zhihu.com/p/149702006)

13. 寄存器个数

| id | case描述        | 寄存器个数       | 寄存器位数 |
| -- | --------------- | ---------------- | ---------- |
| 1  | armv7通用寄存器 | 16个(r0-r15)     | 32位       |
| 2  | armv8通用寄存器 | 31个(x0-x30)     | 64位       |
| 3  | armv7向量寄存器 | 16个(q0-q15)     | 128位      |
| 4  | armv8向量寄存器 | 32个(v0-v15)     | 128位      |