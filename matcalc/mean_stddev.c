#include "mean_stddev.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>

#include "array_mean.h"
#include <stdlib.h>

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

#if 0
void mean_stddev_u8_naive_wrong_by_float(unsigned char* src, size_t len, float* _mean, float* _stddev)
{
    float sum = 0; // 当 sum > 16777215 时，无法准确表示整数，会导致结果（很可能）不正确
    for (size_t i = 0; i < len; i++) {
        sum += src[i];
    }
    
    float mean = sum / len;
    
    sum = 0;
    for (size_t i=0; i<len; i++) {
        float diff = (float)(src[i]) - mean;
        sum += diff * diff;  // 这里用float类型的sum累加，也会导致不准确
    }
    float stddev = sqrt(sum);

    *_mean = mean;
    *_stddev = stddev;
}
#endif

void mean_stddev_u8_naive(unsigned char* src, size_t len, float* _mean, float* _stddev)
{
    uint64_t sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum += src[i];
    }
    
    float mean = sum*1.0 / len;

    double dev = 0;
    for (size_t i = 0; i < len; i++) {
        float diff = (float)(src[i]) - mean;
        dev += diff * diff;
    }
    double stddev = sqrt(dev / len);

    *_mean = mean;
    *_stddev = stddev;
}

//快速计算：根据等价公式： 方差dev=sigma_i[(xi - mean)^2]/N = (sigma_i(xi*xi)/N) - (mean^2)
//算均值的时候，顺带把(sigma_i(xi*xi)/N)算出来，则只需要一次遍历即可
void mean_stddev_u8_fast(unsigned char* src, size_t len, float* _mean, float* _stddev)
{
    uint64_t sum = 0;
    uint16_t* src_square = (uint16_t*)malloc(len*sizeof(uint16_t));
    int16_t s16_val;
    double dev = 0;
    for (size_t i = 0; i < len; i++) {
        sum += src[i];
        s16_val = src[i];
        dev += s16_val * s16_val;
    }
    
    float mean = sum*1.0 / len;
    dev = dev/len - mean * mean;
    double stddev = sqrt(dev);

    *_mean = mean;
    *_stddev = stddev;
}


// 根据mean_stddev_u8_naive 转为arm neon；速度比opencv要慢(3.5 ms vs 2 ms)
void mean_stddev_u8_asimd1(unsigned char* src, size_t len, float* _mean, float* _stddev)
{
    float mean = array_mean_u8_asimd4(src, len);
    double dev = 0;
    
    size_t done = 0;
#ifdef __ARM_NEON
    size_t step = 16;
    size_t vec_size = len - len % step;
    float32x4_t vmean = vdupq_n_f32(mean);
    float32x4_t vdiff1;

    uint16x8_t v2_low;
    uint16x8_t v2_high;
    uint32x4_t v31_low;
    uint32x4_t v31_high;
    uint32x4_t v32_low;
    uint32x4_t v32_high;
    float diff_seg1[4] = {0, 0, 0, 0};
    float diff_seg2[4] = {0, 0, 0, 0};
    float diff_seg3[4] = {0, 0, 0, 0};
    float diff_seg4[4] = {0, 0, 0, 0};
    for (size_t i=0; i<vec_size; i+=step) {
        uint8x16_t v1 = vld1q_u8(src);
        src += step;
        // u8 => u16
        v2_low = vmovl_u8(vget_low_u8(v1));
        v2_high = vmovl_u8(vget_high_u8(v1));
        v31_low = vmovl_u16(vget_low_u16(v2_low));
        v31_high = vmovl_u16(vget_high_u16(v2_low));
        v32_low = vmovl_u16(vget_low_u16(v2_high));
        v32_high = vmovl_u16(vget_high_u16(v2_high));

        float32x4_t v41 = vsubq_f32(vcvtq_f32_u32(v31_low), vmean);
        float32x4_t v42 = vsubq_f32(vcvtq_f32_u32(v31_high), vmean);
        float32x4_t v43 = vsubq_f32(vcvtq_f32_u32(v32_low), vmean);
        float32x4_t v44 = vsubq_f32(vcvtq_f32_u32(v32_high), vmean);

        vdiff1 = vaddq_f32(vdiff1, vmulq_f32(v41, v41));
        vdiff1 = vaddq_f32(vdiff1, vmulq_f32(v42, v42));
        vdiff1 = vaddq_f32(vdiff1, vmulq_f32(v43, v43));
        vdiff1 = vaddq_f32(vdiff1, vmulq_f32(v44, v44));
        // vdiff2 = vaddq_f32(vdiff2, vmulq_f32(v42, v42));
        // vdiff3 = vaddq_f32(vdiff3, vmulq_f32(v43, v43));
        // vdiff4 = vaddq_f32(vdiff4, vmulq_f32(v44, v44));

    }
    done = vec_size;
    vst1q_f32(diff_seg1, vdiff1);
    // vst1q_f32(diff_seg2, vdiff2);
    // vst1q_f32(diff_seg3, vdiff3);
    // vst1q_f32(diff_seg4, vdiff4);
    dev += diff_seg1[0] + diff_seg1[1] + diff_seg1[2] + diff_seg1[3];
    // dev += diff_seg2[0] + diff_seg2[1] + diff_seg2[2] + diff_seg2[3];
    // dev += diff_seg3[0] + diff_seg3[1] + diff_seg3[2] + diff_seg3[3];
    // dev += diff_seg4[0] + diff_seg4[1] + diff_seg4[2] + diff_seg4[3];
#endif
    for (size_t i = done; i < len; i++) {
        float diff = (float)(src[i]) - mean;
        dev += diff * diff;
    }
    double stddev = sqrt(dev / len);

    *_mean = mean;
    *_stddev = stddev;
}

// arm neon version of mean_stddev_u8_fast
void mean_stddev_u8_asimd2(unsigned char* src, size_t len, float* _mean, float* _stddev)
{
    uint64_t sum = 0;
    size_t done = 0;
    double dev = 0;
#ifdef __ARM_NEON

#if __aarch64__
    size_t step = 8;
    size_t vec_size = len - len % step;

    uint8x8_t vdata;
    uint16x8_t v2;
    uint16x8_t v3;

    uint64x2_t v50 = vdupq_n_u64(0);
    uint64x2_t v51 = vdupq_n_u64(0);
    uint64x2_t v52 = vdupq_n_u64(0);
    uint64x2_t v53 = vdupq_n_u64(0);
    for (size_t i=0; i<vec_size ;i+=step) {
        vdata = vld1_u8(src);
        src += step;

        v2 = vmovl_u8(vdata);
        sum += vaddvq_u16(v2);
        v3 = vmulq_u16(v2, v2);
        
        uint32x4_t v40 = vmovl_u16(vget_low_u16(v3));
        uint32x4_t v41 = vmovl_u16(vget_high_u16(v3));

        v50 = vaddq_u64(vmovl_u32(vget_low_u32(v40)), v50);
        v51 = vaddq_u64(vmovl_u32(vget_high_u32(v40)), v51);
        v52 = vaddq_u64(vmovl_u32(vget_low_u32(v41)), v52);
        v53 = vaddq_u64(vmovl_u32(vget_high_u32(v41)), v53);
    }
    
    v50 = vaddq_u64(v50, v51);
    v50 = vaddq_u64(v50, v52);
    v50 = vaddq_u64(v50, v53);

    uint64_t dev_lst[2];
    vst1q_u64(dev_lst, v50);
    dev += dev_lst[0] + dev_lst[1];
    
    done = vec_size;
#else
    //TODO
    printf("error! not implemented yet!\n");
#endif // __aarch64__

#endif // __ARM_NEON

    uint16_t s16_val;
    for (size_t i = done; i < len; i++) {
        sum += src[i];
        s16_val = src[i];
        dev += s16_val * s16_val;
    }
    float mean = sum * 1.0 / len;
    dev = dev/len - mean * mean;
    double stddev = sqrt(dev);

    *_mean = mean;
    *_stddev = stddev;
}