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
| 4  | 缓存行 asimd |  -         | 3.5 ms |

android-armv8加速比：time(opencv)/time(缓存行asimd)=**25倍**


