
https://community.arm.com/developer/research/b/articles/posts/the-arm-scalable-vector-extension-sve

https://community.arm.com/developer/tools-software/hpc/b/hpc-blog/posts/technology-update-the-scalable-vector-extension-sve-for-the-armv8-a-architecture


>Q: 为什么Arm的ComputeLibrary v20.11 说“We have deprecated the NEON and OpenCL computer vision functions and they will be removed in 21.05”？

>A: 之后都用 SVE了吧

>A: Neon 已经很老了

>Q: SVE是什么

>A: Scalar vector extension ISA

>A: SVE 从 2015年开始就被广泛支持了

SVE的直观理解：NEON是固定长度（128位）向量寄存器，SVE则是变长的SIMD指令，有更好的scalabiility.

**SVE是ARM服务器方面的技术，对于Android/iOS等端侧设备来说比较少有；也不能认为SVE是取代NEON的技术，SVE是对NEON的补充。**
