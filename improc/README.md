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
## rgb2bgr



### References

- [性能优化篇（4）：NEON优化案例——图像颜色转换之RGB到BGR（aarch64版）](https://blog.csdn.net/wohenfanjian/article/details/103407259)