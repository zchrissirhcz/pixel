#include "dotproduct.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

float dotproduct_f32_naive(float* a, float* b, uint32_t len)
{
    float res = 0;
    for (uint32_t i=0; i<len; i++) {
        res += a[i] * b[i];
    }
    return res;
}

// 每次加载4个float，然后4个对位同时相乘，同时累加。
float dotproduct_f32_asimd(float* a, float* b, uint32_t len)
{
    float res = 0;
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 4;
    uint32_t vec_size = len - len % step;
    float32x4_t vres = vdupq_n_f32(0);
    for (uint32_t i=0; i<vec_size; i+=step) {
        float32x4_t va = vld1q_f32(a+i);
        float32x4_t vb = vld1q_f32(b+i);
#if 0   //乘法调用一次，加法调用一次
        float32x4_t vtmp = vmulq_f32(v1, v2);
        vres = vaddq_f32(vtmp, vres);
#elif 0 // 其实可以用fma指令一次做好，速度还快
        vres = vfmaq_f32(vres, va, vb);
#else   // vfmaq和vmlaq基本相同
        vres = vmlaq_f32(vres, va, vb);
#endif
    }
    float res_lst[4];
    vst1q_f32(res_lst, vres);
    res = res_lst[0] + res_lst[1] + res_lst[2] + res_lst[3];
    done = vec_size;
#endif // __ARM_NEON
    for (uint32_t i=done; i<len; i++) {
        res += a[i] * b[i];
    }
    return res;
}

// 基于dotproduct_f32_asimd1，尝试多用一些寄存器
// arm64下测试，并没有提速；arm32则有大幅提速
float dotproduct_f32_asimd2(float* a, float* b, uint32_t len)
{
    float res = 0;
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 4*4;
    uint32_t vec_size = len - len % step;
    float32x4_t vres1 = vdupq_n_f32(0);
    float32x4_t vres2 = vdupq_n_f32(0);
    float32x4_t vres3 = vdupq_n_f32(0);
    float32x4_t vres4 = vdupq_n_f32(0);
    for (uint32_t i=0; i<vec_size; i+=step) {
        float32x4_t va1 = vld1q_f32(a);
        float32x4_t vb1 = vld1q_f32(b);
        vres1 = vmlaq_f32(vres1, va1, vb1);

        float32x4_t va2 = vld1q_f32(a+4);
        float32x4_t vb2 = vld1q_f32(b+4);
        vres2 = vmlaq_f32(vres2, va2, vb2);

        float32x4_t va3 = vld1q_f32(a+8);
        float32x4_t vb3 = vld1q_f32(b+8);
        vres3 = vmlaq_f32(vres3, va3, vb3);

        float32x4_t va4 = vld1q_f32(a+12);
        float32x4_t vb4 = vld1q_f32(b+12);
        vres4 = vmlaq_f32(vres4, va4, vb4);

        a += 16;
        b += 16;
    }

    vres1 = vaddq_f32(vres1, vres2);
    vres1 = vaddq_f32(vres1, vres3);
    vres1 = vaddq_f32(vres1, vres4);

    float res_lst[4];
    vst1q_f32(res_lst, vres1);
    res = res_lst[0] + res_lst[1] + res_lst[2] + res_lst[3];
    done = vec_size;
#endif // __ARM_NEON
    for (uint32_t i=done; i<len; i++) {
        res += a[i] * b[i];
    }
    return res;
}

// 既然要充分利用寄存器，对于armv7，它有16个寄存器，dotproduct_f32_asimd2只用了12个
// 这次我们用15个
float dotproduct_f32_asimd3(float* a, float* b, uint32_t len)
{
    float res = 0;
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 4*5;
    uint32_t vec_size = len - len % step;
    float32x4_t vres1 = vdupq_n_f32(0);
    float32x4_t vres2 = vdupq_n_f32(0);
    float32x4_t vres3 = vdupq_n_f32(0);
    float32x4_t vres4 = vdupq_n_f32(0);
    float32x4_t vres5 = vdupq_n_f32(0);

    float32x4_t va1, va2, va3, va4, va5;
    float32x4_t vb1, vb2, vb3, vb4, vb5;
    for (uint32_t i=0; i<vec_size; i+=step) {
        va1 = vld1q_f32(a);
        vb1 = vld1q_f32(b);
        vres1 = vmlaq_f32(vres1, va1, vb1);

        va2 = vld1q_f32(a+4);
        vb2 = vld1q_f32(b+4);
        vres2 = vmlaq_f32(vres2, va2, vb2);

        va3 = vld1q_f32(a+8);
        vb3 = vld1q_f32(b+8);
        vres3 = vmlaq_f32(vres3, va3, vb3);

        va4 = vld1q_f32(a+12);
        vb4 = vld1q_f32(b+12);
        vres4 = vmlaq_f32(vres4, va4, vb4);

        va5 = vld1q_f32(a+16);
        vb5 = vld1q_f32(b+16);
        vres5 = vmlaq_f32(vres5, va5, vb5);

        a += 20;
        b += 20;
    }

    vres1 = vaddq_f32(vres1, vres2);
    vres1 = vaddq_f32(vres1, vres3);
    vres1 = vaddq_f32(vres1, vres4);
    vres1 = vaddq_f32(vres1, vres5);

    float res_lst[4];
    vst1q_f32(res_lst, vres1);
    res = res_lst[0] + res_lst[1] + res_lst[2] + res_lst[3];
    done = vec_size;
#endif // __ARM_NEON
    for (uint32_t i=done; i<len; i++) {
        res += a[i] * b[i];
    }
    return res;
}

//----------------------------------------------------------------------

uint64_t dotproduct_u8_naive(uint8_t* a, uint8_t* b, uint32_t len)
{
    uint64_t res = 0;
    for(uint32_t i=0; i<len; i++) {
        res += a[i] * b[i];
    }
    return res;
}

uint64_t dotproduct_u8_asimd(uint8_t* a, uint8_t* b, uint32_t len)
{
    uint64_t res = 0;
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 16;
    uint32_t vec_size = len - len % step;
    uint8x16_t va1, vb1;
    uint16x8_t v11, v12;
    uint32x4_t v21, v22;
    uint32x4_t v2 = vdupq_n_u32(0);
    uint64x2_t vres = vdupq_n_u64(0);
    for (uint32_t i=0; i<vec_size; i+=step) {
        va1 = vld1q_u8(a);
        vb1 = vld1q_u8(b);

        v11 = vmull_u8(vget_low_u8(va1), vget_low_u8(vb1));
        v12 = vmull_u8(vget_high_u8(va1), vget_high_u8(vb1));
        
        v21 = vaddl_u16(vget_low_u16(v11), vget_high_u16(v11));
        v22 = vaddl_u16(vget_low_u16(v12), vget_high_u16(v12));
        
        v21 = vaddq_u32(v21, v22); // max: pow(2, 18)
        v2 = vaddq_u32(v2, v21); // 累加u32结果

        if (i%16384==0) { // pow(2, 14)  // u32结果溢出的临界时刻，倒出到u64
            vres = vaddq_u64(vres, vaddl_u32(vget_low_u32(v2), vget_high_u32(v2)));
            v2 = vdupq_n_u32(0);
        }

        a += step;
        b += step;
    }
    if (vec_size%16384!=0) { // vec_size不是临界长度的倍数，v2不为0，需要累加到vres上
        vres = vaddq_u64(vres, vaddl_u32(vget_low_u32(v2), vget_high_u32(v2)));
    }
    uint64_t res_lst[2];
    vst1q_u64(res_lst, vres);
    res = res_lst[0] + res_lst[1];

    done = vec_size;
#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        res += (*a) * (*b);
    }
    return res;
}

// armv7下，和dotproduct_u8_asimd1保持相同
// armv8下，尝试用vaddv指令来加速。具体说是vaddlvq_u16
uint64_t dotproduct_u8_asimd2(uint8_t* a, uint8_t* b, uint32_t len)
{
    uint64_t res = 0;
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 16;
    uint32_t vec_size = len - len % step;
    uint8x16_t va1, vb1;
    uint16x8_t v11, v12;
    uint32x4_t v21, v22;
    uint32x4_t v2 = vdupq_n_u32(0);
    uint64x2_t vres = vdupq_n_u64(0);
    for (uint32_t i=0; i<vec_size; i+=step) {
        va1 = vld1q_u8(a);
        vb1 = vld1q_u8(b);

        v11 = vmull_u8(vget_low_u8(va1), vget_low_u8(vb1));
        v12 = vmull_u8(vget_high_u8(va1), vget_high_u8(vb1));

#if __aarch64__
        res += vaddlvq_u16(v11);
        res += vaddlvq_u16(v12);
#else
        v21 = vaddl_u16(vget_low_u16(v11), vget_high_u16(v11));
        v22 = vaddl_u16(vget_low_u16(v12), vget_high_u16(v12));
        
        v21 = vaddq_u32(v21, v22); // max: pow(2, 18)
        v2 = vaddq_u32(v2, v21); // 累加u32结果

        if (i%16384==0) { // pow(2, 14)  // u32结果溢出的临界时刻，倒出到u64
            vres = vaddq_u64(vres, vaddl_u32(vget_low_u32(v2), vget_high_u32(v2)));
            v2 = vdupq_n_u32(0);
        }
#endif // __aarch64__

        a += step;
        b += step;
    }
#if __aarch64__
    // nop
#else
    if (vec_size%16384!=0) { // vec_size不是临界长度的倍数，v2不为0，需要累加到vres上
        vres = vaddq_u64(vres, vaddl_u32(vget_low_u32(v2), vget_high_u32(v2)));
    }
    uint64_t res_lst[2];
    vst1q_u64(res_lst, vres);
    res = res_lst[0] + res_lst[1];
#endif // __aarch64__

    done = vec_size;
#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        res += (*a) * (*b);
        a++;
        b++;
    }
    return res;
}

// armv7和armv8下，和dotproduct_u8_asimd2保持相同
// armv8.2下，尝试用sdot指令加速。代码是可以少写了，速度没变化。
uint64_t dotproduct_u8_asimd3(uint8_t* a, uint8_t* b, uint32_t len)
{
    uint64_t res = 0;
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 16;
    uint32_t vec_size = len - len % step;
    uint8x16_t va1, vb1;
    uint16x8_t v11, v12;
    uint32x4_t v21, v22;
    uint32x4_t v2 = vdupq_n_u32(0);
    uint64x2_t vres = vdupq_n_u64(0);


#if __ARM_FEATURE_DOTPROD
    for (uint32_t i=0; i<vec_size; i+=step) {
        va1 = vld1q_u8(a);
        vb1 = vld1q_u8(b);

        v2 = vdotq_u32(v2, va1, vb1);

        if (i%16384==0) {
            res += vaddlvq_u32(v2);
            v2 = vdupq_n_u32(0);
        }

        a += step;
        b += step;
    }
    if (vec_size%16384!=0) { // vec_size不是临界长度的倍数，v2不为0，需要累加到vres上
        res += vaddlvq_u32(v2);
    }
#else

    for (uint32_t i=0; i<vec_size; i+=step) {
        va1 = vld1q_u8(a);
        vb1 = vld1q_u8(b);

        v11 = vmull_u8(vget_low_u8(va1), vget_low_u8(vb1));
        v12 = vmull_u8(vget_high_u8(va1), vget_high_u8(vb1));
#if __aarch64__
        res += vaddlvq_u16(v11);
        res += vaddlvq_u16(v12);
#else
        v21 = vaddl_u16(vget_low_u16(v11), vget_high_u16(v11));
        v22 = vaddl_u16(vget_low_u16(v12), vget_high_u16(v12));
        
        v21 = vaddq_u32(v21, v22); // max: pow(2, 18)
        v2 = vaddq_u32(v2, v21); // 累加u32结果

        if (i%16384==0) { // pow(2, 14)  // u32结果溢出的临界时刻，倒出到u64
            vres = vaddq_u64(vres, vaddl_u32(vget_low_u32(v2), vget_high_u32(v2)));
            v2 = vdupq_n_u32(0);
        }
#endif // __aarch64__

        a += step;
        b += step;
    }
#if __aarch64__
    // nop
#else
    if (vec_size%16384!=0) { // vec_size不是临界长度的倍数，v2不为0，需要累加到vres上
        vres = vaddq_u64(vres, vaddl_u32(vget_low_u32(v2), vget_high_u32(v2)));
    }
    uint64_t res_lst[2];
    vst1q_u64(res_lst, vres);
    res = res_lst[0] + res_lst[1];
#endif // __aarch64__

#endif // __ARM_FEATURE_DOTPROD

    done = vec_size;
#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        res += (*a) * (*b);
        a++;
        b++;
    }
    return res;
}


// armv7和armv8下，和dotproduct_u8_asimd2保持相同
// armv8.2下，多塞几条sdot指令。
uint64_t dotproduct_u8_asimd4(uint8_t* a, uint8_t* b, uint32_t len)
{
    uint64_t res = 0;
    uint32_t done = 0;

#if __ARM_NEON

#if __ARM_FEATURE_DOTPROD

    uint32_t step = 16*4;
    uint32_t vec_size = len - len % step;
    uint8x16x4_t va1, vb1;
    uint32x4x4_t v2;
    v2.val[0] = vdupq_n_u32(0);
    v2.val[1] = vdupq_n_u32(0);
    v2.val[2] = vdupq_n_u32(0);
    v2.val[3] = vdupq_n_u32(0);
    uint64x2x4_t vres;
    vres.val[0] = vdupq_n_u64(0);
    vres.val[1] = vdupq_n_u64(0);
    vres.val[2] = vdupq_n_u64(0);
    vres.val[3] = vdupq_n_u64(0);

    for (uint32_t i=0; i<vec_size; i+=step) {
        va1 = vld4q_u8(a);
        vb1 = vld4q_u8(b);

        v2.val[0] = vdotq_u32(v2.val[0], va1.val[0], vb1.val[0]);
        v2.val[1] = vdotq_u32(v2.val[1], va1.val[1], vb1.val[1]);
        v2.val[2] = vdotq_u32(v2.val[2], va1.val[2], vb1.val[2]);
        v2.val[3] = vdotq_u32(v2.val[3], va1.val[3], vb1.val[3]);

        if (i%16384==0) {
            res += vaddlvq_u32(v2.val[0]);
            v2.val[0] = vdupq_n_u32(0);

            res += vaddlvq_u32(v2.val[1]);
            v2.val[1] = vdupq_n_u32(0);

            res += vaddlvq_u32(v2.val[2]);
            v2.val[2] = vdupq_n_u32(0);

            res += vaddlvq_u32(v2.val[3]);
            v2.val[3] = vdupq_n_u32(0);
        }

        a += step;
        b += step;
    }
    if (vec_size%16384!=0) { // vec_size不是临界长度的倍数，v2不为0，需要累加到vres上
        res += vaddlvq_u32(v2.val[0]);
        v2.val[0] = vdupq_n_u32(0);

        res += vaddlvq_u32(v2.val[1]);
        v2.val[1] = vdupq_n_u32(0);

        res += vaddlvq_u32(v2.val[2]);
        v2.val[2] = vdupq_n_u32(0);

        res += vaddlvq_u32(v2.val[3]);
        v2.val[3] = vdupq_n_u32(0);
    }
#else

    uint32_t step = 16;
    uint32_t vec_size = len - len % step;
    uint8x16_t va1, vb1;
    uint16x8_t v11, v12;
    uint32x4_t v21, v22;
    uint32x4_t v2 = vdupq_n_u32(0);
    uint64x2_t vres = vdupq_n_u64(0);

    for (uint32_t i=0; i<vec_size; i+=step) {
        va1 = vld1q_u8(a);
        vb1 = vld1q_u8(b);

        v11 = vmull_u8(vget_low_u8(va1), vget_low_u8(vb1));
        v12 = vmull_u8(vget_high_u8(va1), vget_high_u8(vb1));
#if __aarch64__
        res += vaddlvq_u16(v11);
        res += vaddlvq_u16(v12);
#else
        v21 = vaddl_u16(vget_low_u16(v11), vget_high_u16(v11));
        v22 = vaddl_u16(vget_low_u16(v12), vget_high_u16(v12));
        
        v21 = vaddq_u32(v21, v22); // max: pow(2, 18)
        v2 = vaddq_u32(v2, v21); // 累加u32结果

        if (i%16384==0) { // pow(2, 14)  // u32结果溢出的临界时刻，倒出到u64
            vres = vaddq_u64(vres, vaddl_u32(vget_low_u32(v2), vget_high_u32(v2)));
            v2 = vdupq_n_u32(0);
        }
#endif // __aarch64__

        a += step;
        b += step;
    }
#if __aarch64__
    // nop
#else
    if (vec_size%16384!=0) { // vec_size不是临界长度的倍数，v2不为0，需要累加到vres上
        vres = vaddq_u64(vres, vaddl_u32(vget_low_u32(v2), vget_high_u32(v2)));
    }
    uint64_t res_lst[2];
    vst1q_u64(res_lst, vres);
    res = res_lst[0] + res_lst[1];
#endif // __aarch64__

#endif // __ARM_FEATURE_DOTPROD

    done = vec_size;
#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        res += (*a) * (*b);
        a++;
        b++;
    }
    return res;
}