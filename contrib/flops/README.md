# 单核单精度浮点峰值

## 设备

XiaoMI8:  `2.8GHz * 4 + 1.8GHz * 4`

XiaoMI11: `2.84GHz X1*1 + 2.4GHz A78*3 + 1.8GHz A55*4`

假设只考虑性能最好的 X1 core，理论单精度浮点峰值应该是： `2*4*2*2.84=45.44`，解释：
- 第一个因子，2：可以发射两条fma
- 第二个因子，4：128bit/(8bit*sizeof(float))=128/32=4，每条向量指令有4个float
- 第三个因子，2：每个fma指令，有一次乘法，一次加法；每个lane上，加起来是两次浮点计算
- 第四个因子，2.84：这个是X1 core的最大频率

## TEST1
(`FMLA Vd.4S,Vn.4S,Vm.4S`)

| id  | device   | SoC     | armv8 GFLOPS | armv7 GFLOPS |
| --- | -------- | ------- | ------------ | ----------- |
| 1   | XiaoMI8  | QCOM845 | 22.152212    | 21.865858   |
| 2   | XiaoMI11 | QCOM888 | 55.142088    | 44.537808   |

疑问1：`55.142088`比理论值`45.44`还大？

疑问2：算GFLOPS时，为啥要乘以80？
```c++
#define OP_FLOATS (80)
...
printf("perf: %.6lf \n", LOOP*OP_FLOATS*1.0 * 1e-9 / time_used);
```

## TEST2
(`FMLA Vd.4S,Vn.4S,Vm.S[0]`)

| id  | device   | SoC     | armv8 GFLOPS | armv7 GFLOPS |
| --- | -------- | ------- | ------------ | ----------- |
| 1   | XiaoMI8  | QCOM845 | 22.231225    | 20.047187   |
| 2   | XiaoMI11 | QCOM888 | 44.651512    | 36.854709   |

## Refs

- [FLOPS per cycle for sandy-bridge and haswell SSE2/AVX/AVX2](https://stackoverflow.com/questions/15655835/flops-per-cycle-for-sandy-bridge-and-haswell-sse2-avx-avx2)
- [如何判断算法是否有可优化空间？](https://zhuanlan.zhihu.com/p/268925243)
- [浮点峰值那些事儿](https://zhuanlan.zhihu.com/p/28226956)
- https://github.com/BBuf/how-to-optimize-gemm/blob/master/armv8a/armv8afp/test.S
