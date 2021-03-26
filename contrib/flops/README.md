# 单核单精度浮点峰值

## 设备

XiaoMI8:  `2.8GHz * 4 + 1.8GHz * 4`

XiaoMI11: `2.84GHz X1*1 + 2.4GHz A78*3 + 1.8GHz A55*4`

假设只考虑性能最好的 X1 core，理论单精度浮点峰值应该是： `4*4*2*2.84=90.88`，解释：
- 第一个因子，4：可以发射4条fma ("Cortex-X1 是 SIMD 4发射" by 虫叔)
- 第二个因子，4：128bit/(8bit*sizeof(float))=128/32=4，每条向量指令有4个float
- 第三个因子，2：每个fma指令，有一次乘法，一次加法；每个lane上，加起来是两次浮点计算
- 第四个因子，2.84：这个是X1 core的最大频率

**TODO：绑核再测量**

## TEST1
(`FMLA Vd.4S,Vn.4S,Vm.4S`)

| id  | device   | SoC     | armv8 GFLOPS | armv7 GFLOPS |
| --- | -------- | ------- | ------------ | ----------- |
| 1   | XiaoMI8  | QCOM845 |     |    |
| 2   | XiaoMI11 | QCOM888 | 71.489574    | 44.649627  |

## TEST2
(`FMLA Vd.4S,Vn.4S,Vm.S[0]`)

| id  | device   | SoC     | armv8 GFLOPS | armv7 GFLOPS |
| --- | -------- | ------- | ------------ | ----------- |
| 1   | XiaoMI8  | QCOM845 |     |   |
| 2   | XiaoMI11 | QCOM888 |  69.079704    |   37.253853  |

## Refs

- "圈圈虫语录"
- [FLOPS per cycle for sandy-bridge and haswell SSE2/AVX/AVX2](https://stackoverflow.com/questions/15655835/flops-per-cycle-for-sandy-bridge-and-haswell-sse2-avx-avx2)
- [如何判断算法是否有可优化空间？](https://zhuanlan.zhihu.com/p/268925243)
- [浮点峰值那些事儿](https://zhuanlan.zhihu.com/p/28226956)
- https://github.com/BBuf/how-to-optimize-gemm/blob/master/armv8a/armv8afp/test.S
- [手机处理器寿命多长，为什么一般手机不允许超频工作？](https://www.zhihu.com/question/335518974) 关于手机CPU睿频的讨论