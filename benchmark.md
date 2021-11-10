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
-----------------------------------------------------------------------------
Benchmark                                   Time             CPU   Iterations
-----------------------------------------------------------------------------
Rgb2bgrFixture/naive                     11.9 ms         11.8 ms           56
Rgb2bgrFixture/idxopt                    2.55 ms         2.53 ms          271
Rgb2bgrFixture/asimd                     12.0 ms         11.9 ms           54
Rgb2bgrFixture/asm                       11.8 ms         11.7 ms           54
Rgb2bgrFixture/opencv                    3.69 ms         3.62 ms          199
Rgb2bgrInplaceFixture/naive              8.03 ms         7.96 ms           87
Rgb2bgrInplaceFixture/naive2             7.77 ms         7.70 ms           89
Rgb2bgrInplaceFixture/asm                8.07 ms         8.00 ms           86
Rgb2bgrInplaceFixture/opencv             23.8 ms         23.5 ms           30
Rgb2bgrInplaceFixture/opencv_trick       3.00 ms         2.95 ms          246
```