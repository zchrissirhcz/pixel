# 单核单精度浮点峰值

## 设备

XiaoMI11: `2.84GHz X1*1 + 2.4GHz A78*3 + 1.8GHz A55*4`

## 理论峰值

**X1**

理论单精度浮点峰值： `4*4*2*2.84=90.88`，解释：
- 第一个因子，4：可以发射4条fma ("Cortex-X1 是 SIMD 4发射" by 虫叔)
- 第二个因子，4：128bit/(8bit*sizeof(float))=128/32=4，每条向量指令有4个float
- 第三个因子，2：每个fma指令，有一次乘法，一次加法；每个lane上，加起来是两次浮点计算
- 第四个因子，2.84：这个是X1 core的最大频率

**A78**

理论单精度浮点峰值： `2*4*2*2.4=38.4`，解释：
- 第一个因子，2：可以发射2条fma ("Cortex-A78 是 SIMD 2发射" by 虫叔)
- 第二个因子，4：128bit/(8bit*sizeof(float))=128/32=4，每条向量指令有4个float
- 第三个因子，2：每个fma指令，有一次乘法，一次加法；每个lane上，加起来是两次浮点计算
- 第四个因子，2.4：这个是A78 core的最大频率

**A55**

理论单精度浮点峰值： `2*3*2*1.8=21.6`
## 指令说明


**向量乘向量形式**

ARM NEON乘加指令，向量乘向量形式，对应的arm64汇编是`FMLA Vd.4S,Vn.4S,Vm.4S`，对应的函数实现是`perf_fmla_vec_vec`

**向量乘标量形式**

ARM NEON乘加指令，向量乘标量形式，对应的arm64汇编是`FMLA Vd.4S,Vn.4S,Vm.S[0]`，对应的函数实现是`perf_fmla_vec_scalar`

## 测试结果

| id  | core | 指令      | 理论值 | armv8 GFLOPS   | armv7 GFLOPS |
| --- | ---- | --------- | ----- | -------------- | ------------ |
| 1   | X1   | 向量乘向量 | 90.88 |  71.23(78.4%)  | 44.69    |
| 2   | X1   | 向量乘标量 | 90.88 |  71.02(78.1%)  | 42.62    |
| 3   | A78  | 向量乘向量 | 38.4  |  38.34(99.8%)  | 38.34    |
| 4   | A78  | 向量乘标量 | 38.4  |  38.25(99.6%)  | 37.06    |
| 5   | A55  | 向量乘向量 | 21.6  |  13.73(63.6%)  | 13.32    |
| 6   | A55  | 向量乘标量 | 21.6  |                |          |

## Refs

- "圈圈虫语录"
- [FLOPS per cycle for sandy-bridge and haswell SSE2/AVX/AVX2](https://stackoverflow.com/questions/15655835/flops-per-cycle-for-sandy-bridge-and-haswell-sse2-avx-avx2)
- [如何判断算法是否有可优化空间？](https://zhuanlan.zhihu.com/p/268925243)
- [浮点峰值那些事儿](https://zhuanlan.zhihu.com/p/28226956)
- https://github.com/BBuf/how-to-optimize-gemm/blob/master/armv8a/armv8afp/test.S
- [移动端arm cpu优化学习笔记第3弹--绑定cpu(cpu affinity)](https://zhuanlan.zhihu.com/p/138905432)
- [手机处理器寿命多长，为什么一般手机不允许超频工作？](https://www.zhihu.com/question/335518974) 关于手机CPU睿频的讨论