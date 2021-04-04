#include "relu_layer.h"

#if __ARM_NEON
#include <arm_neon.h>
#endif

void relu_f32_forward_naive(float* src, float* dst, uint32_t len)
{
    for(uint32_t i=0; i<len; i++) {
        dst[i] = src[i]>0 ? src[i]: 0;
    }
}

void relu_f32_forward_asimd(float* src, float* dst, uint32_t len)
{
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 4;
    uint32_t vec_size = len - len % step;
    float32x4_t vlimit = vdupq_n_f32(0.0f);
    float32x4_t v1;
    for(uint32_t i=0; i<vec_size; i+=step) {
        v1 = vld1q_f32(src);
        v1 = vmaxq_f32(v1, vlimit);
        vst1q_f32(dst, v1);

        src += step;
        dst += step;
    }
    done = vec_size;
#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        *dst = (*src)>0 ? (*src): 0;
        dst++;
        src++;
    }
}

// vcltq_f32 + vbslq_f32 替代 vmaxq_f32, armv7下更快，armv8持平。
void relu_f32_forward_asimd2(float* src, float* dst, uint32_t len)
{
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 4;
    uint32_t vec_size = len - len % step;
    float32x4_t vlimit = vdupq_n_f32(0.0f);
    float32x4_t v1;
    float32x4_t v2;
    float32x4_t mask;
    for(uint32_t i=0; i<vec_size; i+=step) {
        v1 = vld1q_f32(src);
        mask = vcltq_f32(v1, vlimit);
        v2 = vbslq_f32(mask, vlimit, v1);
        vst1q_f32(dst, v2);

        src += step;
        dst += step;
    }
    done = vec_size;
#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        *dst = (*src)>0 ? (*src): 0;
        dst++;
        src++;
    }
}

// 基于relu_f32_forward_asimd；尝试用intrinsic搞双发射
// 
void relu_f32_forward_asimd3(float* src, float* dst, uint32_t len)
{
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 4*4;
    uint32_t vec_size = len - len % step;
    float32x4_t vlimit = vdupq_n_f32(0.0f);
    float32x4_t v1;
    float32x4_t v2;
    float32x4_t v3;
    float32x4_t v4;
    for(uint32_t i=0; i<vec_size; i+=step) {
        v1 = vld1q_f32(src);
        v2 = vld1q_f32(src+4);
        v3 = vld1q_f32(src+8);
        v4 = vld1q_f32(src+12);

        v1 = vmaxq_f32(v1, vlimit);
        v2 = vmaxq_f32(v2, vlimit);
        v3 = vmaxq_f32(v3, vlimit);
        v4 = vmaxq_f32(v4, vlimit);

        vst1q_f32(dst, v1);
        vst1q_f32(dst+4, v2);
        vst1q_f32(dst+8, v3);
        vst1q_f32(dst+12, v4);

        src += step;
        dst += step;
    }
    done = vec_size;
#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        *dst = (*src)>0 ? (*src): 0;
        dst++;
        src++;
    }
}

// 基于relu_f32_forward_asimd；尝试用内联汇编写
void relu_f32_forward_asm(float* src, float* dst, uint32_t len)
{
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 4;
    uint32_t vec_size = len - len % step;
    uint32_t neon_len = vec_size / step;
#if __aarch64__
    asm volatile(
        "movi    v1.4s, #0 \n"
        "0: \n"
        "ld1     {v0.4s}, [%0], #16 \n"
        "fmax    v0.4s, v0.4s, v1.4s \n"
        "st1     {v0.4s}, [%1], #16 \n"
        "subs    %w2, %w2, #1 \n"
        "bne     0b \n"
        : "=r"(src), //%0
          "=r"(dst), //%1
          "=r"(neon_len) //%2
        : "0"(src),
          "1"(dst),
          "2"(neon_len)
        : "cc", "memory", "v0", "v1"
    );
#else
    asm volatile(
        "vmov.i32 q15, #0 \n" //limit
        "0: \n"
        "vld1.32 {q0}, [%0]! \n"
        "vmax.f32 q0, q0, q15 \n"
        "vst1.32 {q0}, [%1]! \n"
        "subs %2, #1 \n"
        "bne 0b \n"
        : "=r"(src), //%0
          "=r"(dst), //%1
          "=r"(neon_len) //%2
        : "0"(src),
          "1"(dst),
          "2"(neon_len)
        : "cc", "memory", "q0", "q15"
    );
#endif // __aarch64__
    done = vec_size;

#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        *dst = (*src)>0 ? (*src): 0;
        dst++;
        src++;
    }
}

// 基于relu_f32_forward_asm，利用双发射特性，做一次循环展开
// 实际上，原来每次加载4个元素(step=4)，现在每次加载16个元素
// armv7(A55)有明显加速，armv8(X1)则速度没有明显变化
void relu_f32_forward_asm2(float* src, float* dst, uint32_t len)
{
    uint32_t done = 0;
#if __ARM_NEON
    uint32_t step = 4*4;
    uint32_t vec_size = len - len % step;
    uint32_t neon_len = vec_size / step;
#if __aarch64__
    asm volatile(
        "movi    v15.4s, #0 \n"
        "0: \n"
        "ld2     {v0.4s, v1.4s}, [%0], #32 \n"
        "ld2     {v2.4s, v3.4s}, [%0], #32 \n"
        "fmax    v0.4s, v0.4s, v15.4s \n"
        "fmax    v1.4s, v1.4s, v15.4s \n"
        "fmax    v2.4s, v2.4s, v15.4s \n"
        "fmax    v3.4s, v3.4s, v15.4s \n"
        "st2     {v0.4s, v1.4s}, [%1], #32 \n"
        "st2     {v2.4s, v3.4s}, [%1], #32 \n"
        "subs    %w2, %w2, #1 \n"
        "bne     0b \n"
        : "=r"(src), //%0
          "=r"(dst), //%1
          "=r"(neon_len) //%2
        : "0"(src),
          "1"(dst),
          "2"(neon_len)
        : "cc", "memory", "v0", "v1", "v2", "v3", "v15"
    );
#else
    asm volatile(
        "vmov.i32 q15, #0 \n" //limit
        "0: \n"
        "vld1.32 {q0, q1}, [%0]! \n"
        "vld1.32 {q2, q3}, [%0]! \n"
        "vmax.f32 q0, q0, q15 \n"
        "vmax.f32 q1, q1, q15 \n"
        "vmax.f32 q2, q2, q15 \n"
        "vmax.f32 q3, q3, q15 \n"
        "vst1.32 {q0, q1}, [%1]! \n"
        "vst1.32 {q2, q3}, [%1]! \n"
        "subs %2, #1 \n"
        "bne 0b \n"
        : "=r"(src), //%0
          "=r"(dst), //%1
          "=r"(neon_len) //%2
        : "0"(src),
          "1"(dst),
          "2"(neon_len)
        : "cc", "memory", "q0", "q1", "q2", "q3", "q15"
    );
#endif // __aarch64__
    done = vec_size;

#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        *dst = (*src)>0 ? (*src): 0;
        dst++;
        src++;
    }
}


// 基于relu_f32_forward_asm2，尝试把Loop[AB]拆分重组为A->Loop[BA]->B
// MNN的作者说，原先的 Loop[AB] 就变成了 A->Loop[BA]->B，然后 BA 由于顺序颠倒，可以实现错排并发。
void relu_f32_forward_asm3(float* src, float* dst, uint32_t len)
{
    uint32_t done = 0;
#if __ARM_NEON
#if __aarch64__
    uint32_t step = 4*4;
    uint32_t vec_size = len - len % step;
    uint32_t neon_len = vec_size / step;
    asm volatile(
        "movi    v15.4s, #0 \n"
        "L4: \n"
        "ld2     {v0.4s, v1.4s}, [%0], #32 \n"
        "fmax    v0.4s, v0.4s, v15.4s \n"
        "ld2     {v2.4s, v3.4s}, [%0], #32 \n"
        "fmax    v1.4s, v1.4s, v15.4s \n"
        "subs    %w2, %w2, #1 \n"
        "ble L4End \n"
        "L4Loop: \n"
        "st2     {v0.4s, v1.4s}, [%1], #32 \n"
        "fmax    v2.4s, v2.4s, v15.4s \n"
        "ld2     {v0.4s, v1.4s}, [%0], #32 \n"
        "fmax    v3.4s, v3.4s, v15.4s \n"
        "st2     {v2.4s, v3.4s}, [%1], #32 \n"
        "fmax    v0.4s, v0.4s, v15.4s \n"
        "ld2     {v2.4s, v3.4s}, [%0], #32 \n"
        "fmax    v1.4s, v1.4s, v15.4s \n"
        "subs    %w2, %w2, #1 \n"
        "bne     L4Loop \n"
        "L4End: \n"
        "st2     {v0.4s, v1.4s}, [%1], #32 \n"
        "fmax    v2.4s, v2.4s, v15.4s \n"
        "fmax    v3.4s, v3.4s, v15.4s \n"
        "st2     {v2.4s, v3.4s}, [%1], #32 \n"
        : "=r"(src), //%0
          "=r"(dst), //%1
          "=r"(neon_len) //%2
        : "0"(src),
          "1"(dst),
          "2"(neon_len)
        : "cc", "memory", "v0", "v1", "v2", "v3", "v15"
    );
#else
    uint32_t step = 4*4;
    uint32_t vec_size = len - len % step;
    uint32_t neon_len = vec_size / step;
    asm volatile(
        "vmov.i32 q15, #0 \n" //limit
        "L4: \n"
        "vld1.32 {q0, q1}, [%0]! \n"
        "vmax.f32 q0, q0, q15 \n"
        "vld1.32 {q2, q3}, [%0]! \n"
        "vmax.f32 q1, q1, q15 \n"
        "subs %2, #1 \n"
        "ble L4End \n"
        "L4Loop: \n"
        "vst1.32 {q0, q1}, [%1]! \n"
        "vmax.f32 q2, q2, q15 \n"
        "vld1.32 {q0, q1}, [%0]! \n"
        "vmax.f32 q3, q3, q15 \n"
        "vst1.32 {q2, q3}, [%1]! \n"
        "vmax.f32 q0, q0, q15 \n"
        "vld1.32 {q2, q3}, [%0]! \n"
        "subs %2, #1 \n"
        "vmax.f32 q1, q1, q15 \n"
        "bne L4Loop \n"
        "L4End: \n"
        "vst1.32 {q0, q1}, [%1]! \n"
        "vmax.f32 q2, q2, q15 \n"
        "vmax.f32 q3, q3, q15 \n"
        "vst1.32 {q2, q3}, [%1]! \n"
        : "=r"(src), //%0
          "=r"(dst), //%1
          "=r"(neon_len) //%2
        : "0"(src),
          "1"(dst),
          "2"(neon_len)
        : "cc", "memory", "q0", "q1", "q2", "q3", "q15"
    );
#endif // __aarch64__
    done = vec_size;

#endif // __ARM_NEON
    for(uint32_t i=done; i<len; i++) {
        *dst = (*src)>0 ? (*src): 0;
        dst++;
        src++;
    }
}
