#include "array_mean.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

#include <stdio.h>
#include <stdbool.h>

float array_mean_u8_naive(unsigned char* data, size_t len) {
    float sum = 0;
    for (size_t i=0; i<len; i++) {
        sum += data[i];
    }
    sum /= len;
    return sum;
}

// u8转u16再转u32再转f32，然后算sum
float array_mean_u8_asimd(unsigned char* data, size_t len) {
    float sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON
    size_t step = 8;
    size_t vec_size = len - len % step;
    float sum_lst[4] = {0.f, 0.f, 0.f, 0.f};
    uint8x8_t v1;
    uint16x8_t v2;
    uint32x4_t v3_low, v3_high;
    float32x4_t v4_low, v4_high;
    float32x4_t v5;
    float32x4_t vsum = vdupq_n_f32(0.f);
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1_u8(data);
        data += step;
        v2 = vmovl_u8(v1);
        v3_low = vmovl_u16(vget_low_u16(v2));
        v3_high = vmovl_u16(vget_high_u16(v2));
        v4_low = vcvtq_f32_u32(v3_low);
        v4_high = vcvtq_f32_u32(v3_high);
        v5 = vaddq_f32(v4_low, v4_high);
        vsum = vaddq_f32(v5, vsum);
    }
    vst1q_f32(sum_lst, vsum);
    sum = sum_lst[0] + sum_lst[1] + sum_lst[2] + sum_lst[3];
    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    sum /= len;
    return sum;
}

float array_mean_u8_asimd2(unsigned char* data, size_t len) {
    float sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON
    size_t step = 32;
    size_t vec_size = len - len % step;
    float sum_lst[4] = {0.f, 0.f, 0.f, 0.f};
    uint8x8x4_t vdata;
    uint16x8_t v21, v22, v23, v24;
    uint32x4_t v31_low, v32_low, v33_low, v34_low;
    uint32x4_t v31_high, v32_high, v33_high, v34_high;
    float32x4_t v41_low, v42_low, v43_low, v44_low;
    float32x4_t v41_high, v42_high, v43_high, v44_high;
    float32x4_t v51, v52, v53, v54;
    float32x4_t vsum1 = vdupq_n_f32(0.f);
    float32x4_t vsum2 = vdupq_n_f32(0.f);
    float32x4_t vsum3 = vdupq_n_f32(0.f);
    float32x4_t vsum4 = vdupq_n_f32(0.f);
    for (size_t i=0; i<vec_size; i+=step) {
        vdata = vld4_u8(data);
        data += step;

        //vdata[0]
        v21 = vmovl_u8(vdata.val[0]);
        v31_low = vmovl_u16(vget_low_u16(v21));
        v31_high = vmovl_u16(vget_high_u16(v21));
        v41_low = vcvtq_f32_u32(v31_low);
        v41_high = vcvtq_f32_u32(v31_high);
        v51 = vaddq_f32(v41_low, v41_high);
        vsum1 = vaddq_f32(v51, vsum1);

        //vdata[1]
        v22 = vmovl_u8(vdata.val[1]);
        v32_low = vmovl_u16(vget_low_u16(v22));
        v32_high = vmovl_u16(vget_high_u16(v22));
        v42_low = vcvtq_f32_u32(v32_low);
        v42_high = vcvtq_f32_u32(v32_high);
        v52 = vaddq_f32(v42_low, v42_high);
        vsum2 = vaddq_f32(v52, vsum2);

        //vdata[2]
        v23 = vmovl_u8(vdata.val[2]);
        v33_low = vmovl_u16(vget_low_u16(v23));
        v33_high = vmovl_u16(vget_high_u16(v23));
        v43_low = vcvtq_f32_u32(v33_low);
        v43_high = vcvtq_f32_u32(v33_high);
        v53 = vaddq_f32(v43_low, v43_high);
        vsum3 = vaddq_f32(v53, vsum3);

        //vdata[3]
        v24 = vmovl_u8(vdata.val[3]);
        v34_low = vmovl_u16(vget_low_u16(v24));
        v34_high = vmovl_u16(vget_high_u16(v24));
        v44_low = vcvtq_f32_u32(v34_low);
        v44_high = vcvtq_f32_u32(v34_high);
        v54 = vaddq_f32(v44_low, v44_high);
        vsum4 = vaddq_f32(v54, vsum4);
    }
    vsum1 = vaddq_f32(vsum1, vsum2);
    vsum1 = vaddq_f32(vsum1, vsum3);
    vsum1 = vaddq_f32(vsum1, vsum4);
    vst1q_f32(sum_lst, vsum1);
    sum = sum_lst[0] + sum_lst[1] + sum_lst[2] + sum_lst[3];
    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    sum /= len;
    return sum;
}

// <del>u8转u16，算sum，再转float</del>
// Note: this implementation requires len < 67372036
// when len > 67372036 and each element is 255, it will cause overflow
// ((2*2147483648-1)/255)*4 = 67372036 (=8000x8421 size gray image)
float array_mean_u8_asimd3(unsigned char* data, size_t len) {
    if (len >= 67372036) {
        fprintf(stderr, "input len too long, may cause overflow\n");
    }
    float sum = 0;
    size_t done = 0;

#ifdef __ARM_NEON

    uint16x8_t result_level1 = vdupq_n_u16(0);
    uint32x4_t result_level2 = vdupq_n_u32(0);
    uint32x4_t vsum = vdupq_n_u32(0);

    uint8x8_t h_vec;

    uint8x8_t x_vec = vdup_n_u8(1);

    uint32_t t0, t1;

    bool flag;
    size_t step = 8;
    size_t vec_size = len - len % step;
    for(size_t i=0; i<vec_size; i+=step) {
        h_vec = vld1_u8(data);
        data += step;
        result_level1 = vmlal_u8(result_level1, h_vec, x_vec);
        flag = false;

        if( i % 256 == 0) {
            //16x8 => 32x4
            t0 = vgetq_lane_u16(result_level1, 0);
            t1 = vgetq_lane_u16(result_level1, 1);
            result_level2 = vsetq_lane_u32(t0+t1, result_level2, 0);

            t0 = vgetq_lane_u16(result_level1, 2);
            t1 = vgetq_lane_u16(result_level1, 3);
            result_level2 = vsetq_lane_u32(t0+t1, result_level2, 1);

            t0 = vgetq_lane_u16(result_level1, 4);
            t1 = vgetq_lane_u16(result_level1, 5);
            result_level2 = vsetq_lane_u32(t0+t1, result_level2, 2);

            t0 = vgetq_lane_u16(result_level1, 6);
            t1 = vgetq_lane_u16(result_level1, 7);
            result_level2 = vsetq_lane_u32(t0+t1, result_level2, 3);

            vsum = vaddq_u32(result_level2, vsum);

            result_level1 = vdupq_n_u16(0);
            flag = true;
        }
    }
    if(!flag) {
        //16x8 => 32x4
        t0 = vgetq_lane_u16(result_level1, 0);
        t1 = vgetq_lane_u16(result_level1, 1);
        result_level2 = vsetq_lane_u32(t0+t1, result_level2, 0);

        t0 = vgetq_lane_u16(result_level1, 2);
        t1 = vgetq_lane_u16(result_level1, 3);
        result_level2 = vsetq_lane_u32(t0+t1, result_level2, 1);

        t0 = vgetq_lane_u16(result_level1, 4);
        t1 = vgetq_lane_u16(result_level1, 5);
        result_level2 = vsetq_lane_u32(t0+t1, result_level2, 2);

        t0 = vgetq_lane_u16(result_level1, 6);
        t1 = vgetq_lane_u16(result_level1, 7);
        result_level2 = vsetq_lane_u32(t0+t1, result_level2, 3);

        vsum = vaddq_u32(result_level2, vsum);

        result_level1 = vdupq_n_u16(0);
        flag = true;
    }

    uint32_t sum_lst[4];
    vst1q_u32(sum_lst, vsum);
    sum += sum_lst[0];
    sum += sum_lst[1];
    sum += sum_lst[2];
    sum += sum_lst[3];

    done = vec_size;
#endif // __ARM_NEON

    for (; done<len; done++) {
        sum += *data;
        data++;
    }

    float avg = sum / len;
    return avg;
}

float array_mean_u8_asimd4(unsigned char* data, size_t len) {
    float sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON

#if __aarch64__
    size_t step = 8*4;
    size_t vec_size = len - len % step;

    uint8x8x4_t vdata;
    uint16x8_t v20, v21, v22, v23;
    uint32_t sum0=0, sum1=0, sum2=0, sum3=0;
    for (size_t i=0; i<vec_size; i+=step) {
        vdata = vld4_u8(data);
        data += step;

        v20 = vmovl_u8(vdata.val[0]);
        sum0 += vaddvq_u16(v20);

        v21 = vmovl_u8(vdata.val[1]);
        sum1 += vaddvq_u16(v21);

        v22 = vmovl_u8(vdata.val[2]);
        sum2 += vaddvq_u16(v22);

        v23 = vmovl_u8(vdata.val[3]);
        sum3 += vaddvq_u16(v23);
    }
    sum += sum0;
    sum += sum1;
    sum += sum2;
    sum += sum3;
#else // 注意：armv7没有vaddv指令，下面的实现是从asimd1拷贝来的。。慢。
    size_t step = 8;
    size_t vec_size = len - len % step;
    float sum_lst[4] = {0.f, 0.f, 0.f, 0.f};
    uint8x8_t v1;
    uint16x8_t v2;
    uint32x4_t v3_low, v3_high;
    float32x4_t v4_low, v4_high;
    float32x4_t v5;
    float32x4_t vsum = vdupq_n_f32(0.f);
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1_u8(data);
        data += step;
        v2 = vmovl_u8(v1);
        v3_low = vmovl_u16(vget_low_u16(v2));
        v3_high = vmovl_u16(vget_high_u16(v2));
        v4_low = vcvtq_f32_u32(v3_low);
        v4_high = vcvtq_f32_u32(v3_high);
        v5 = vaddq_f32(v4_low, v4_high);
        vsum = vaddq_f32(v5, vsum);
    }
    vst1q_f32(sum_lst, vsum);
    sum = sum_lst[0] + sum_lst[1] + sum_lst[2] + sum_lst[3];
#endif // __aarch64__
    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    sum /= len;
    return sum;
}

/*
//TODO
float array_mean_u8_asimd5(unsigned char* data, size_t len) {
    float sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON

    size_t step = 8*4;
    size_t vec_size = len - len % step;

    uint8x8x4_t vdata;
    uint16x8x4_t vsum;
    uint8x8_t ones = vdup_n_u8(1);
    uint32_t sum_lst[4] = {0};
    for (size_t i=0; i<vec_size; i+=step) {
        vdata = vld4_u8(data);
        data += step;

        vsum.val[0] = vmlal_u8(vsum.val[0], vdata.val[0], ones);
        vsum.val[1] = vmlal_u8(vsum.val[1], vdata.val[1], ones);
        vsum.val[2] = vmlal_u8(vsum.val[2], vdata.val[2], ones);
        vsum.val[3] = vmlal_u8(vsum.val[3], vdata.val[3], ones);

        if (i%256==0) {
            // TODO: convert u16 to u32, refer to asimd3
        }
    }
    sum += sum0;
    sum += sum1;
    sum += sum2;
    sum += sum3;

    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    sum /= len;
    return sum;
}
*/

//-----
float array_mean_s8_naive(int8_t* data, size_t len) {
    float res = 0;
    for(size_t i=0; i<len; i++) {
        res += data[i];
    }
    res /= len;
    return res;
}

float array_mean_s8_asimd(int8_t* data, size_t len)
{
#ifdef __ARM_NEON
    int16x8_t result_level1 = vdupq_n_s16(0);
    int32x4_t result_level2 = vdupq_n_s32(0);
    int32x4_t result_vec = vdupq_n_s32(0);
    
    int8x8_t h_vec;

    int8x8_t x_vec = vdup_n_s8(1);

    int t0, t1;

    bool flag;
    for(int i=0; i<len/8; i++) {
        h_vec = vld1_s8(&data[i*8]);
        result_level1 = vmlal_s8(result_level1, h_vec, x_vec);
        flag = false;

        if( (i*8) % 256 == 0) {
            //16x8 => 32x4
            t0 = vgetq_lane_s16(result_level1, 0);
            t1 = vgetq_lane_s16(result_level1, 1);
            result_level2 = vsetq_lane_s32(t0+t1, result_level2, 0);

            t0 = vgetq_lane_s16(result_level1, 2);
            t1 = vgetq_lane_s16(result_level1, 3);
            result_level2 = vsetq_lane_s32(t0+t1, result_level2, 1);

            t0 = vgetq_lane_s16(result_level1, 4);
            t1 = vgetq_lane_s16(result_level1, 5);
            result_level2 = vsetq_lane_s32(t0+t1, result_level2, 2);

            t0 = vgetq_lane_s16(result_level1, 6);
            t1 = vgetq_lane_s16(result_level1, 7);
            result_level2 = vsetq_lane_s32(t0+t1, result_level2, 3);

            result_vec = vaddq_s32(result_level2, result_vec);

            result_level1 = vdupq_n_s16(0);
            flag = true;
        }
    }
    if(!flag) {
        //16x8 => 32x4
        t0 = vgetq_lane_s16(result_level1, 0);
        t1 = vgetq_lane_s16(result_level1, 1);
        result_level2 = vsetq_lane_s32(t0+t1, result_level2, 0);

        t0 = vgetq_lane_s16(result_level1, 2);
        t1 = vgetq_lane_s16(result_level1, 3);
        result_level2 = vsetq_lane_s32(t0+t1, result_level2, 1);

        t0 = vgetq_lane_s16(result_level1, 4);
        t1 = vgetq_lane_s16(result_level1, 5);
        result_level2 = vsetq_lane_s32(t0+t1, result_level2, 2);

        t0 = vgetq_lane_s16(result_level1, 6);
        t1 = vgetq_lane_s16(result_level1, 7);
        result_level2 = vsetq_lane_s32(t0+t1, result_level2, 3);

        result_vec = vaddq_s32(result_level2, result_vec);

        result_level1 = vdupq_n_s16(0);
        flag = true;
    }

    float sum = 0;
    sum += vgetq_lane_s32(result_vec, 0);
    sum += vgetq_lane_s32(result_vec, 1);
    sum += vgetq_lane_s32(result_vec, 2);
    sum += vgetq_lane_s32(result_vec, 3);

    if(len%8!=0) {
        for(int k=len-(len%8); k<len; k++) {
            sum += data[k];
        }
    }

    float avg = sum / len;
    return avg;
#endif
    //TODO
    return 0;
}