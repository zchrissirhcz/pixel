## Environment
```
arch                Android ARMv8
compiler            NDK-r21b
device              XiaoMi 11
host                Linux x64 (ubuntu20.04), or Win10 / MacOSX
correctness test    google test
performance test    google benchmark
```

Only Android ARMv8 results are given, without binding to big CPU clusters.

For each performance result item, you may refer to its correspoinding `xxx_perf.cpp`. For example, `Rgb2grayFixture/fixed_asimd` refers to `rgb2gray_perf.cpp`.

## rgb2gray
```
----------------------------------------------------------------------
Benchmark                            Time             CPU   Iterations
----------------------------------------------------------------------
Rgb2grayFixture/naive             25.8 ms         25.7 ms           27
Rgb2grayFixture/fixed             2.74 ms         2.72 ms          251
Rgb2grayFixture/fixed_asimd       2.75 ms         2.73 ms          253
Rgb2grayFixture/opencv            4.74 ms         4.66 ms          129
Rgb2grayFixture/fixed_asimd       2.98 ms         2.95 ms          236
```

## rgb2bgr
```
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
Rgb2bgrFixture/naive                     11.9 ms         11.8 ms           55
Rgb2bgrFixture/idxopt                    2.46 ms         2.44 ms          282
Rgb2bgrFixture/asimd                     11.9 ms         11.8 ms           55
Rgb2bgrFixture/asm                       11.9 ms         11.8 ms           55
Rgb2bgrFixture/opencv                    3.35 ms         3.26 ms          190
Rgb2bgrInplaceFixture/naive              7.95 ms         7.88 ms           88
Rgb2bgrInplaceFixture/naive2             7.81 ms         7.75 ms           90
Rgb2bgrInplaceFixture/asm                7.93 ms         7.87 ms           89
Rgb2bgrInplaceFixture/opencv             20.0 ms         19.8 ms           37
Rgb2bgrInplaceFixture/opencv_trick       2.55 ms         2.49 ms          251
```

## flip
```
--------------------------------------------------------------------------------
Benchmark                                      Time             CPU   Iterations
--------------------------------------------------------------------------------
FlipHorizonRgbFixture/naive                 4.87 ms         4.81 ms          121
FlipHorizonRgbFixture/idxopt                12.5 ms         12.4 ms           52
FlipHorizonRgbFixture/asimd                 4.97 ms         4.93 ms          112
FlipHorizonRgbFixture/opencv                10.4 ms         10.3 ms           63

FlipHorizonGrayFixture/naive                1.03 ms         1.02 ms          628
FlipHorizonGrayFixture/asimd                1.01 ms         1.00 ms          651
FlipHorizonGrayFixture/opencv               1.76 ms         1.74 ms          402

FlipVerticalRgbFixture/naive                4.57 ms         4.51 ms          134
FlipVerticalRgbFixture/opencv               4.34 ms         4.29 ms          130
FlipVerticalRgbFixture/opencv_inplace       2.99 ms         2.96 ms          227
FlipVerticalRgbFixture/bylines              4.71 ms         4.64 ms          128

FlipVerticalGrayFixture/naive               1.38 ms         1.37 ms          498
FlipVerticalGrayFixture/bylines             1.21 ms         1.19 ms          549
FlipVerticalGrayFixture/opencv              1.30 ms         1.28 ms          482
```