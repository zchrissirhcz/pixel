# flip: benchmark

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