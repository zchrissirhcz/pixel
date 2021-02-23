# improc

[TOC]

## rgb2gray

image size: h=4032, w=3024

| id | implementation | time cost |
| ---| -------------- | ---------- |
| 1  | float       |  42 ms   |
| 2  | fixed point[<sup>1</sup>](#refer-anchor-1) | 11 ms |
| 3  | fixed point + arm neon[<sup>2</sup>](#refer-anchor-2) | 10~13 ms |
| 4  | opencv 4.5.0 | 7~13 ms |
| 5  | float + assembly[<sup>3</sup>](#refer-anchor-3) | 14 ms |

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

| id | implementation | release time cost | debug time cost |
| --- | -------------- | --------- | -------------|
| 1   | naive          | 20.3 ms|    190 ms |
| 2   | index optimized| 13/24 ms |  90 ms |
| 3   | neon intrinsic | 33 ms  |    270 ms |
| 4   | asm            | 19 ms  |  19 ms |
| 5   | opencv         | 16 ms  |  18 ms |

**rgb2bgr_inplace**

| id | implementation | release time cost | debug time cost | 备注 |
| --- | -------------- | --------- | ---------- | -------- |
| 1   | naive          | 10 ms    |   134 ms |  - |
| 2   | naive2         | 11 ms    |    43 ms |  - |
| 3   | asm            | 5.5 ms   |    6 ms  |  **比OpenCV快5倍** |
| 4   | opencv         | 27 ms    |    29 ms | - |

### References

- [性能优化篇（4）：NEON优化案例——图像颜色转换之RGB到BGR（aarch64版）](https://blog.csdn.net/wohenfanjian/article/details/103407259)

## Neon Notes

1. arm64 汇编和 arm32 汇编，写法有差别，要分开写。

2. 汇编里的自增(+=)操作：
   - arm32是`!`，例如 `[%1]!`；
   - arm64指定增加的字节数，例如 st3 存储后，自增 48 字节：
   ```asm
   st3    { v0.16b, v1.16b, v2.16b }, [%0], #48
   ```

3. 如何使用 `float32x4x4_t` 这样的“长”类型：
    把它当做是数组，也就是
    ```c++
    typedef struct float32x4x4_t {
        float32x4_t val[4];
    } float32x4x4_t;
    ```