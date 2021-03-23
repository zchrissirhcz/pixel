# matcalc

matrix calculation related stuffs, including basic blas-like operations.

## u8 matrix column max (u8 矩阵逐列求最大值)

image info: height=4032, width=3024 (river_bank2.png)

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a  |
| -- | ----------- | ---------- | ------- |
| 1  | opencv      |  166 ms    |  86 ms  |
| 2  | naive       |  6.8 ms    |  75 ms  |
| 3  | 缓存行       |  6.4 ms    | 5.3 ms  |
| 4  | 缓存行 asimd |  -         | 0.45 ms |

android-armv8加速比：time(opencv)/time(缓存行asimd)=**191倍**


## u8 matrix column max and idx (u8 矩阵逐列求最大值和对应索引)

image info: height=4032, width=3024 (river_bank2.png)

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a  |
| -- | ----------- | ---------- | ------- |
| 1  | opencv      |  237 ms    |   88 ms  |
| 2  | naive       |  167 ms    |   78 ms  |
| 3  | 缓存行       |  10 ms    |  5.3 ms  |
| 4  | 缓存行 asimd |  -         | 1.46 ms |

android-armv8加速比：time(opencv)/time(缓存行asimd)=**60倍**


## mean std dev, input u8 (uchar输入类型，算均值和方差)

image info: name=river_bank2.png, height=3024, width=4032

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a   |
| -- | ----------- | ---------- | -------- |
| 1  | opencv      |  5.6 ms    |  2 ms    |
| 2  | naive       |  11.2 ms   |  15 ms   |
| 3  | asimd1      |   -        |  3.5 ms  |
| 4  | 等价公式     |    9 ms    |  11 ms   |
| 5  | 等价公式 asimd2 | -       |  2.2 ms  |

## array mean u8 (uchar输入类型，计算均值)

image info: name=river_bank2.png, height=3024, width=4032

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a   | armv7a    |
| -- | ----------- | ---------- | -------- | --------- |
| 1  | opencv      |  4.21 ms   |0.6/0.8 ms|  1.29 ms  |
| 2  | naive       |  2.53 ms   |  3.5 ms  |  8.64 ms  |
| 3  | asimd1      |  -         |  1.95 ms |  3.28 ms  |
| 4  | asimd2      |  -         |  1.42 ms |  4.62 ms  |
| 5  | asimd3      |  -         |  1.08 ms |  2.69 ms  |
| 6  | asimd4      |  -         |  1.04 ms |  3.79 ms  |
| 7  | asimd5      |  -         |  1.07 ms |  1.79 ms  |
| 8  | asimd6      |  -         |  0.57 ms |  -        |
| 8  | asimd7      |  -         |  0.65 ms |  1.28 ms  |

**References**

https://zhuanlan.zhihu.com/p/128640600