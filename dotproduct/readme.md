# dotproduct

I would like to calculate the dot-product result of two float32 array:
```c++
float dotproduct(size_t len, float* va, float* vb)
```

I implement this function by calling SIMD wrapper functions, from 4 libraries:
- [OpenCV universal intrinsics](https://docs.opencv.org/master/df/d91/group__core__hal__intrin.html)
- [MIPP](https://github.com/aff3ct/MIPP)
- [Eigen](http://gitlab.com/libeigen/eigen)
- [xsimd](https://github.com/xtensor-stack/xsimd)

With `/AVX2` enabled, I test the performance of these implementations in both Release and Debug mode. Specifically, I use `len=200000000`, and got:

Release Mode:
>time cost for random assign is 2438.699400 ms
impl1(Naive),   result is -5290.376953, time cost is 223.822600 ms
impl2(OpenCV),  result is -5291.503906, time cost is 73.623500 ms
impl4(MIPP),    result is -5291.504395, time cost is 68.850600 ms
impl5(Eigen),   result is -5290.747070, time cost is 65.757400 ms
impl6(xsimd),   result is -5291.503906, time cost is 66.209500 ms

Debug Mode:
>time cost for random assign is 11423.765500 ms
impl1(Naive),   result is -5290.376953, time cost is 506.001200 ms
impl2(OpenCV),  result is -5291.520020, time cost is 1650.197400 ms
impl4(MIPP),    result is -5291.520508, time cost is 3944.401800 ms
impl5(Eigen),   result is -5290.748047, time cost is 3000.024400 ms
impl6(xsimd),   result is -5291.520508, time cost is 7662.899600 ms

对比发现，在 Debug 模式下，各种 SIMD Wrapper 都比 naive 实现要慢 3~15 倍。

一种快速的优化方法是开启**内联优化**, `/Ob3` (VS2019才支持, [ref](https://docs.microsoft.com/fr-fr/cpp/build/reference/ob-inline-function-expansion?view=msvc-160))

After setting `/Ob3` for MSVC16 project property, debug mode result:
```
time cost for random assign is 11280.405600 ms
impl1(Naive),   result is -5290.376953, time cost is 503.579000 ms
impl2(OpenCV),  result is -5291.520020, time cost is 172.810900 ms
impl4(MIPP),    result is -5291.520508, time cost is 1005.270700 ms
impl5(Eigen),   result is -5290.748047, time cost is 260.357800 ms
impl6(xsimd),   result is -5291.520508, time cost is 1266.117400 ms
```

如果只开SSE，在Debug模式，`/Ob3`内联优化：
```
time cost for random assign is 11483.795500 ms
impl1(Naive),   result is -5290.376953, time cost is 519.258500 ms
impl2(OpenCV),  result is -5289.963867, time cost is 328.571300 ms
impl4(MIPP),    result is -5290.376953, time cost is 1139.231900 ms
impl5(Eigen),   result is -5291.520508, time cost is 493.596100 ms
impl6(xsimd),   result is -5289.963379, time cost is 1114.844500 ms
impl7(sse),     result is -5289.963379, time cost is 243.068300 ms
```

## 暂时没尝试的
- [highway](https://github.com/google/highway)
- [Simd](https://github.com/ermig1979/Simd)
- [OpenBLAS](https://github.com/xianyi/OpenBLAS)
- [Vc](https://github.com/VcDevel/Vc)
- [libsimdpp](https://github.com/p12tic/libsimdpp)
- [std-simd](https://github.com/VcDevel/std-simd)
- [psimd](https://github.com/Maratyszcza/psimd) 只有128位，NNPACK作者出品；似乎比较简单；但很久不更新了
- [pure_simd](https://github.com/eatingtomatoes/pure_simd) 需要C++17
- [generic_simd](https://github.com/genericsimd/generic_simd) 很大程度上用C实现的。但不支持neon
- [kr8md](https://github.com/njroussel/Kr8md/blob/master/include/kr8md/intrinsic_types.h) 实现比较简单，似乎可以参考
- [tsimd](https://github.com/jeffamstutz/tsimd) 作者有在2018 cppcon做演讲
---

## Deprecated results
### no -mavx2
CV_SIMD is : 1
CV_SIMD_WIDTH is : 16
CV_SIMD128 is : 1
CV_SIMD256 is : 0
CV_SIMD512 is : 0
sizeof(v_uint8) = 16
sizeof(v_int32) = 16
sizeof(v_float32) = 16
time cost for random assign is 2348.799711 ms
impl1, result is -5290.376953, time cost is 213.656658 ms
step is 4
impl2, result is -5289.430176, time cost is 68.728935 ms


### -mavx2

CV_SIMD is : 1
CV_SIMD_WIDTH is : 32
CV_SIMD128 is : 1
CV_SIMD256 is : 1
CV_SIMD512 is : 0
sizeof(v_uint8) = 32
sizeof(v_int32) = 32
sizeof(v_float32) = 32
time cost for random assign is 2214.837345 ms
impl1, result is -5290.376953, time cost is 215.468339 ms
step is 8
impl2, result is -5290.986328, time cost is 61.731527 ms

### android armv8
CV_SIMD is : 1
CV_SIMD_WIDTH is : 16
CV_SIMD128 is : 1
CV_SIMD256 is : 0
CV_SIMD512 is : 0
sizeof(v_uint8) = 16
sizeof(v_int32) = 16
sizeof(v_float32) = 16
time cost for random assign is 2231.197447 ms
impl1, result is -5290.376953, time cost is 346.908750 ms
step is 4
impl2, result is -5289.429688, time cost is 191.786354 ms


## References
[使用OpenCV中的universal intrinsics为算法提速 (1)](https://mp.weixin.qq.com/s?__biz=MjM5NTE3NjY5MA==&mid=2247484025&idx=1&sn=132d0fc0a242df11bd5b59cd22eaad99&chksm=a6fdcbe4918a42f2dae5c150541b8e243bfe4d70e126813852c0d8b108d37af28e9713a4699e&scene=21#wechat_redirect)

[使用OpenCV中的universal intrinsics为算法提速 (2)](https://mp.weixin.qq.com/s?__biz=MjM5NTE3NjY5MA==&mid=2247484072&idx=1&sn=e04b079225776cfde7c400d319f58448&chksm=a6fdcb35918a4223ce56f1b79359c0159000ec0b5d674cb7e820aaa6d5a1010f4ba2fe022ccc&scene=21#wechat_redirect)

[使用OpenCV中的universal intrinsics为算法提速 (3)](https://mp.weixin.qq.com/s?src=11&timestamp=1613301941&ver=2890&signature=NacoD9gG2TtepdxxvAzYPATCU-fW7DHosaJFqEg7u0fXcAUiVwnhjScV43Zkf*NI-rr5nLI0e2l7epFcz3g4Gv*VnnPosSTTtLt0kR2cdlhf8wqSN7Jq8DbJDLLKah3x&new=1)

[Universal intrinsics - OpenCV Docs Page](https://docs.opencv.org/master/df/d91/group__core__hal__intrin.html)
