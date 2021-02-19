/*
好吧，为什么单独弄个simple.c出来？
因为vfmaq和vmlaq，在arm64下，按说是同样的效果。（Nihui说的）。
然而实际发现，mla方式的intrinsic，和naive实现得到的float结果相同。
而如果是fma方式的intrinsics，和naive实现的结果“不一样”。

怎么不一样？`a==b`形式的浮点比较，结果不一样！

于是引申到浮点数的比较。然而完美的基于C的浮点数比较，暂时没找到。需要满足：
1. 相对性。两个很大浮点数的diff也很大，是要容忍，还是一定要很小的epsilon？
2. 如果两个浮点数都非常小，以至于它们差值比上较大的那个，超级超级小；但是a b本身的数量级不对的。怎么算？
3. 如果a 和 b 有一个是nan/inf，怎么办？

好吧，实在无聊的话，可以用这个simple.c生成汇编然后比较。。


E:\dbg\zz\pixel\dotproduct\build\android-arm64>E:\\soft\\Android\\ndk-r21b\\toolchains\\llvm\\prebuilt\\windows-x86_64\\bin\\clang.exe --target=aarch64-none
-linux-android24 --gcc-toolchain=E:/soft/Android/ndk-r21b/toolchains/llvm/prebuilt/windows-x86_64 --sysroot=E:/soft/Android/ndk-r21b/toolchains/llvm/prebuil
E:\dbg\zz\pixel\dotproduct\build\android-arm64>ons -ffunction-sections -funwind-tables -fstack-protector-strong -no-canonical-prefixes -D_FORTIFY_SOURCE=2 -
Wformat -Werror=format-security   -Werror=implicit-function-declaration -Werror=implicit-int -Werror=incompatible-pointer-types -Werror=return-type -Werror=
E:\dbg\zz\pixel\dotproduct\build\android-arm64>nitialized -Werror=format -Werror=int-conversion -Werror=array-bounds -Werror=pointer-arith -fno-common -Werr
or=int-to-pointer-cast -Werror=unknown-escape-sequence -Werror=comment -Werror=unused-value -Werror=unused-comparison -O3 -DNDEBUG  -fPIC -o CMakeFiles\\sim
E:\dbg\zz\pixel\dotproduct\build\android-arm64>l\\dotproduct\\simple.c
*/

#include <arm_neon.h>


void weighted_sum_asimd_fast(size_t len, const float* arr1, const float weight1, const float* arr2, const float weight2, float* res)
{
    size_t vec_size = 0;
#ifdef __ARM_NEON
    const float* ptr1 = arr1;
    const float* ptr2 = arr2;
    float* pres = res;
    //const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    const int step = 16;
    vec_size = len - len % step;
    for (size_t i=0; i<vec_size; i+=step) {

        float32x4x4_t v1 = vld4q_f32(ptr1); //第一个数组的数据
        float32x4x4_t v2 = vld4q_f32(ptr2); //第二个数组的数据
        float32x4_t v_weight1 = vdupq_n_f32(weight1); // 权值向量化
        float32x4_t v_weight2 = vdupq_n_f32(weight2); // 第二个权值向量化
        float32x4x4_t vout; // 存结果
        
    #if 0 //如下4句，结果正确
        vout.val[0] = vaddq_f32(vmulq_f32(v1.val[0], v_weight1), vmulq_f32(v2.val[0], v_weight2));
        vout.val[1] = vaddq_f32(vmulq_f32(v1.val[1], v_weight1), vmulq_f32(v2.val[1], v_weight2));
        vout.val[2] = vaddq_f32(vmulq_f32(v1.val[2], v_weight1), vmulq_f32(v2.val[2], v_weight2));
        vout.val[3] = vaddq_f32(vmulq_f32(v1.val[3], v_weight1), vmulq_f32(v2.val[3], v_weight2));
    #else //这4句，结果不对了。  fma 这样用似乎不对？
        // vout.val[0] = vmlaq_f32(vmulq_f32(v1.val[0], v_weight1), v2.val[0], v_weight2);
        // vout.val[1] = vmlaq_f32(vmulq_f32(v1.val[1], v_weight1), v2.val[1], v_weight2);
        // vout.val[2] = vmlaq_f32(vmulq_f32(v1.val[2], v_weight1), v2.val[2], v_weight2);
        // vout.val[3] = vmlaq_f32(vmulq_f32(v1.val[3], v_weight1), v2.val[3], v_weight2);

        vout.val[0] = vfmaq_f32(vmulq_f32(v1.val[0], v_weight1), v2.val[0], v_weight2);
        vout.val[1] = vfmaq_f32(vmulq_f32(v1.val[1], v_weight1), v2.val[1], v_weight2);
        vout.val[2] = vfmaq_f32(vmulq_f32(v1.val[2], v_weight1), v2.val[2], v_weight2);
        vout.val[3] = vfmaq_f32(vmulq_f32(v1.val[3], v_weight1), v2.val[3], v_weight2);


        // float32x4_t t0 = vmulq_f32(v1.val[0], v_weight1);
        // vout.val[0] = vfmaq_f32(t0, v2.val[0], v_weight2);
        // float32x4_t t1 = vmulq_f32(v1.val[1], v_weight1);
        // vout.val[1] = vfmaq_f32(t1, v2.val[1], v_weight2);
        // float32x4_t t2 = vmulq_f32(v1.val[2], v_weight1);
        // vout.val[2] = vfmaq_f32(t2, v2.val[2], v_weight2);
        // float32x4_t t3 = vmulq_f32(v1.val[3], v_weight1);
        // vout.val[3] = vfmaq_f32(t3, v2.val[3], v_weight2);

        vst4q_f32(pres, vout);
    #endif
        ptr1 += 16;  ptr2 += 16; pres += 16;
    }
#endif
    for (size_t i=vec_size; i<len; i++) {
        res[i] = arr1[i]*weight1 + arr2[i]*weight2;
    }
}