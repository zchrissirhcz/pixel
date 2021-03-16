# matcalc

matrix calculation related stuffs, including basic blas-like operations.

## u8 matrix column max (u8 矩阵逐列求最大值)

image info: height=4032, width=3024

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a  | 加速比 |
| -- | ----------- | ---------- | ------- | ------ |
| 1  | opencv      |  156 ms    |  68 ms  |        |
| 2  | naive       |  6.7 ms    |  55 ms  |        |
| 3  | 相邻行       |  6.5 ms    |  5 ms   |       |
| 4  | 相邻行 asimd |  -         | 0.46 ms | **148倍** |



