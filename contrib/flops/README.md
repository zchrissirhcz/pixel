# GFLOPS table

## TEST1
(`FMLA Vd.4S,Vn.4S,Vm.4S`)

| id  | device   | SoC     | armv8 GFLOPS | armv7 GFLOPS |
| --- | -------- | ------- | ------------ | ----------- |
| 1   | XiaoMI8  | QCOM845 | 22.152212    | 21.865858   |
| 2   | XiaoMI11 | QCOM888 | 55.142088    | 44.537808   |

## TEST2
(`FMLA Vd.4S,Vn.4S,Vm.S[0]`)

| id  | device   | SoC     | armv8 GFLOPS | armv7 GFLOPS |
| --- | -------- | ------- | ------------ | ----------- |
| 1   | XiaoMI8  | QCOM845 | 22.231225    | 20.047187   |
| 2   | XiaoMI11 | QCOM888 | 44.651512    | 36.854709   |

## Refs

- [如何判断算法是否有可优化空间？](https://zhuanlan.zhihu.com/p/268925243)
- [浮点峰值那些事儿](https://zhuanlan.zhihu.com/p/28226956)
- https://github.com/BBuf/how-to-optimize-gemm/blob/master/armv8a/armv8afp/test.S
