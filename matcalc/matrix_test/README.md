OpenCV与Eigen运行效率对比

https://zhuanlan.zhihu.com/p/61216321

## setup
```bash
git clone https://gitlab.com/libeigen/eigen.git eigen
```

## 3x3, run 10000 times
**VS2019-x64 Release**
```
Opencv Multiple 10000 cost 8.240100 ms.
Result:
[210000, 50000, 10000;
 620000, 310000, 300000;
 320000, 540000, 550000]
Opencv Add 10000 cost 6.342800 ms.
Result:
[10000, 20000, 40000;
 40000, 100000, 120000;
 140000, 90000, 0]
Eigen Multiple 10000 cost 0.603000 ms.
Result:
210000  50000  10000
620000 310000 300000
320000 540000 550000
Eigen Add 10000 cost 0.006800 ms.
Result:
 10000  20000  40000
 40000 100000 120000
140000  90000      0
```

**NDK-r21b arm64-v8a QCOM845(Xiaomi8) Release**

```
Opencv Multiple 10000 cost 9.832917 ms.
Result:
[210000, 50000, 10000;
 620000, 310000, 300000;
 320000, 540000, 550000]
Opencv Add 10000 cost 8.583750 ms.
Result:
[10000, 20000, 40000;
 40000, 100000, 120000;
 140000, 90000, 0]
Eigen Multiple 10000 cost 0.230157 ms.
Result:
210000  50000  10000
620000 310000 300000
320000 540000 550000
Eigen Add 10000 cost 0.011250 ms.
Result:
 10000  20000  40000
 40000 100000 120000
140000  90000      0
```

## 100x100, run 100 times
**vs2019-x64**
```
Opencv Multiple 100 cost 191.892100 ms.
Opencv Add 100 cost 0.370900 ms.
Eigen Multiple 100 cost 0.006400 ms.
Eigen Add 100 cost 0.000100 ms.
```

**android arm64**
```
Opencv Multiple 100 cost 147.661719 ms.
Opencv Add 100 cost 4.003958 ms.
Eigen Multiple 100 cost 0.002448 ms.
Eigen Add 100 cost 0.000052 ms.
```

## 1000x1000, run 10 times
**vs2019-x64**
```
Opencv Multiple 10 cost 5618.482900 ms.
Opencv Add 10 cost 23.869200 ms.
Eigen Multiple 10 cost 0.001000 ms.
Eigen Add 10 cost 0.000000 ms.
```

**android arm64**
```
Opencv Multiple 10 cost 7647.752028 ms.
Opencv Add 10 cost 45.626875 ms.
Eigen Multiple 10 cost 0.001042 ms.
Eigen Add 10 cost 0.000052 ms.
```