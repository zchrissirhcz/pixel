## Environment
- arch: Android ARMv8
- compiler: NDK-r21b
- device: XiaoMi 11
- host: Linux x64 (ubuntu20.04), or Win10 / MacOSX
- correctness test: google test
- performance test: google benchmark

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