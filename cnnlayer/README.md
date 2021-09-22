# cnnlayer

尝试实现和优化一些 CNN 层的前向推理计算。

## 性能评价方法

用比例形式`Told / Tnew`来表述，也就是优化前的时间开销`Told`，除以优化后的时间开销`Tnew`。如果有所改进，比值应该大于1。
（参照 CS·APP 一书 Chap1 的建议）

例如原来的时间开销是 100 ms, 优化后时间开销是 50 ms，表述为：优化后的性能是原版性能的 2 倍。

## Relu f32

优化方法参考 MNN 作者姜霄棠的博客 [SIMD优化之ARM纯汇编开发](https://blog.csdn.net/jxt1234and2010/article/details/104012746)，armv8汇编为自行实现。

核心实现代码实现在 [relu_layer.c](relu_layer.c)，性能测试代码在 [relu_layer_test.cpp](relu_layer_test.cpp)。

和原文的区别：
- 没用纯汇编，依然是asm volatile内联方式
- 测试环境是Android XiaoMI 11，没考虑iOS

数组长度： `100000 * 4`;
循环次数： `10000`

| id | method               | armv8 release  | armv7 release |
| -- | -------------------- | -------------- | ------------- |
| 1  | naive                | 657.5531 ms    | 4347.0384 ms  |
| 2  | asimd，简单intrinsic | 684.8943 ms    | 1713.7074 ms  |
| 3  | asimd2,另个简单intrinsic | 684.8943 ms| 1178.6791 ms  |
| 4  | asimd3,简单的双发射  | 637.5869 ms    |  724.4534 ms  |
| 5  | asm，asimd2直接翻译  | 644.7484 ms    | 1088.3278 ms  |
| 6  | asm2, 简单的双发射   | 646.8378 ms    |  675.0179 ms  |
| 7  | asm3,`Loop[AB]`改`A->Loop[BA]->B`| 643.7671 ms | 657.0258 ms |

结论：
- armv7下asm3方案性能最佳，相比于简单的intrinsic方法asimd，性能为2.6倍(1713/657);对比naive实现，性能为6.6倍(4347/657)
- armv8下asimd3方案性能最佳，相比简单的intrinsic方法asimd，性能为1.07倍；相对比naive实现，性能为1.03倍。