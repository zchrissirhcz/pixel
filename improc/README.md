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

- [1] [SSE图像算法优化系列一：一段BGR2Y的SIMD代码解析。](https://www.cnblogs.com/Imageshop/p/6261719.html)

<div id="refer-anchor-2"></div>

- [2] [Tencent/ncnn - mat_pixel.cpp](https://github.com/Tencent/ncnn/blob/31bc57b1e293e726b318cf93fdcd6154f2188477/src/mat_pixel.cpp#L780-L790)

<div id="refer-anchor-3"></div>

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

- [Neon Intrinsics查询 - fengbingchun整理版，部分带sse等价解释](https://blog.csdn.net/fengbingchun/article/details/38085781)

- [Neon Intrinsics - 按类型整理 - EmSoftEn的博客](https://blog.csdn.net/emsoften/article/details/51718763)

**Assembler**:

- [arm汇编入门帖 - asm volatile形式 - Roland_Sun的博客](https://blog.csdn.net/Roland_Sun/article/details/42921131)

- [arm内联汇编入门(上) - 梁德澎](https://aijishu.com/a/1060000000116427)

- [arm内联汇编入门(下) - 梁德澎](https://aijishu.com/a/1060000000116431)

- [ARM GCC 内联汇编参考手册 - 中文翻译版](https://github.com/tidyjiang8/arm-gcc-inline-assembler/blob/master/src/arm-gcc-inline-assembler.md)

**全能系列**

- [AI移动端优化 - 知乎 - 章小龙 & BBuf & 圈圈虫等](https://www.zhihu.com/people/zxloas/posts)

- [arm官方文档 armv8-a 系列教程](https://developer.arm.com/architectures/instruction-sets/simd-isas/neon/neon-programmers-guide-for-armv8-a)

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