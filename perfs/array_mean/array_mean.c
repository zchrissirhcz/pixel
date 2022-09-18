#include "array_mean.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

#include <stdio.h>
#include <stdbool.h>

float array_mean_u8_naive(unsigned char* data, size_t len) {
    uint64_t sum = 0; // note: if use float type for `sum`, when sum>=16777216 and as integer, sum would be incorrect
    for (size_t i=0; i<len; i++) {
        sum += data[i];
    }
    return sum*1.0 / len;
}

// u8转u16再转u32再转u64，然后算sum
float array_mean_u8_asimd1(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON
    size_t step = 8;
    size_t vec_size = len - len % step;

    uint8x8_t v1;
    uint16x8_t v2;
    uint32x4_t v30, v31;

    uint64x2_t v40 = vdupq_n_u64(0);
    uint64x2_t v41 = vdupq_n_u64(0);
    uint64x2_t v42 = vdupq_n_u64(0);
    uint64x2_t v43 = vdupq_n_u64(0);

    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1_u8(data);
        data += step;
        v2 = vmovl_u8(v1);
        v30 = vmovl_u16(vget_low_u16(v2));
        v31 = vmovl_u16(vget_high_u16(v2));

        v40 = vaddq_u64(v40, vmovl_u32(vget_low_u32(v30)));
        v41 = vaddq_u64(v41, vmovl_u32(vget_high_u32(v30)));
        v42 = vaddq_u64(v42, vmovl_u32(vget_low_u32(v31)));
        v43 = vaddq_u64(v43, vmovl_u32(vget_high_u32(v31)));
    }
    v40 = vaddq_u64(v40, v41);
    v40 = vaddq_u64(v40, v42);
    v40 = vaddq_u64(v40, v43);

    uint64_t sum_lst[2];
    vst1q_u64(sum_lst, v40);
    sum += sum_lst[0] + sum_lst[1];

    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    return sum * 1.0 / len;
}

// asimd1的增强版，step从8变为8x4
float array_mean_u8_asimd2(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON
    size_t step = 8 * 4;
    size_t vec_size = len - len % step;

    uint8x8x4_t v1;
    uint16x8x4_t v2;
    uint32x4x4_t v30, v31;

    uint64x2x4_t v40, v41, v42, v43;
    v40.val[0] = vdupq_n_u64(0);
    v40.val[1] = vdupq_n_u64(0);
    v40.val[2] = vdupq_n_u64(0);
    v40.val[3] = vdupq_n_u64(0);

    v41.val[0] = vdupq_n_u64(0);
    v41.val[1] = vdupq_n_u64(0);
    v41.val[2] = vdupq_n_u64(0);
    v41.val[3] = vdupq_n_u64(0);

    v42.val[0] = vdupq_n_u64(0);
    v42.val[1] = vdupq_n_u64(0);
    v42.val[2] = vdupq_n_u64(0);
    v42.val[3] = vdupq_n_u64(0);

    v43.val[0] = vdupq_n_u64(0);
    v43.val[1] = vdupq_n_u64(0);
    v43.val[2] = vdupq_n_u64(0);
    v43.val[3] = vdupq_n_u64(0);

    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld4_u8(data);
        data += step;

        v2.val[0] = vmovl_u8(v1.val[0]);
        v30.val[0] = vmovl_u16(vget_low_u16(v2.val[0]));
        v31.val[0] = vmovl_u16(vget_high_u16(v2.val[0]));
        v40.val[0] = vaddq_u64(v40.val[0], vmovl_u32(vget_low_u32(v30.val[0])));
        v41.val[0] = vaddq_u64(v41.val[0], vmovl_u32(vget_high_u32(v30.val[0])));
        v42.val[0] = vaddq_u64(v42.val[0], vmovl_u32(vget_low_u32(v31.val[0])));
        v43.val[0] = vaddq_u64(v43.val[0], vmovl_u32(vget_high_u32(v31.val[0])));

        v2.val[1] = vmovl_u8(v1.val[1]);
        v30.val[1] = vmovl_u16(vget_low_u16(v2.val[1]));
        v31.val[1] = vmovl_u16(vget_high_u16(v2.val[1]));
        v40.val[1] = vaddq_u64(v40.val[1], vmovl_u32(vget_low_u32(v30.val[1])));
        v41.val[1] = vaddq_u64(v41.val[1], vmovl_u32(vget_high_u32(v30.val[1])));
        v42.val[1] = vaddq_u64(v42.val[1], vmovl_u32(vget_low_u32(v31.val[1])));
        v43.val[1] = vaddq_u64(v43.val[1], vmovl_u32(vget_high_u32(v31.val[1])));

        v2.val[2] = vmovl_u8(v1.val[2]);
        v30.val[2] = vmovl_u16(vget_low_u16(v2.val[2]));
        v31.val[2] = vmovl_u16(vget_high_u16(v2.val[2]));
        v40.val[2] = vaddq_u64(v40.val[2], vmovl_u32(vget_low_u32(v30.val[2])));
        v41.val[2] = vaddq_u64(v41.val[2], vmovl_u32(vget_high_u32(v30.val[2])));
        v42.val[2] = vaddq_u64(v42.val[2], vmovl_u32(vget_low_u32(v31.val[2])));
        v43.val[2] = vaddq_u64(v43.val[2], vmovl_u32(vget_high_u32(v31.val[2])));

        v2.val[3] = vmovl_u8(v1.val[3]);
        v30.val[3] = vmovl_u16(vget_low_u16(v2.val[3]));
        v31.val[3] = vmovl_u16(vget_high_u16(v2.val[3]));
        v40.val[3] = vaddq_u64(v40.val[3], vmovl_u32(vget_low_u32(v30.val[3])));
        v41.val[3] = vaddq_u64(v41.val[3], vmovl_u32(vget_high_u32(v30.val[3])));
        v42.val[3] = vaddq_u64(v42.val[3], vmovl_u32(vget_low_u32(v31.val[3])));
        v43.val[3] = vaddq_u64(v43.val[3], vmovl_u32(vget_high_u32(v31.val[3])));
    }
    v40.val[0] = vaddq_u64(v40.val[0], v41.val[0]);
    v40.val[0] = vaddq_u64(v40.val[0], v42.val[0]);
    v40.val[0] = vaddq_u64(v40.val[0], v43.val[0]);

    v40.val[1] = vaddq_u64(v40.val[1], v41.val[1]);
    v40.val[1] = vaddq_u64(v40.val[1], v42.val[1]);
    v40.val[1] = vaddq_u64(v40.val[1], v43.val[1]);

    v40.val[2] = vaddq_u64(v40.val[2], v41.val[2]);
    v40.val[2] = vaddq_u64(v40.val[2], v42.val[2]);
    v40.val[2] = vaddq_u64(v40.val[2], v43.val[2]);

    v40.val[3] = vaddq_u64(v40.val[3], v41.val[3]);
    v40.val[3] = vaddq_u64(v40.val[3], v42.val[3]);
    v40.val[3] = vaddq_u64(v40.val[3], v43.val[3]);

    v40.val[0] = vaddq_u64(v40.val[0], v40.val[1]);
    v40.val[0] = vaddq_u64(v40.val[0], v40.val[2]);
    v40.val[0] = vaddq_u64(v40.val[0], v40.val[3]);

    uint64_t sum_lst[2];
    vst1q_u64(sum_lst, v40.val[0]);
    sum += sum_lst[0] + sum_lst[1];

    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    return sum * 1.0 / len;
}

float array_mean_u8_asimd3(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;

#ifdef __ARM_NEON
    size_t step = 8;
    size_t vec_size = len - len % step;

    uint16x8_t result_level1 = vdupq_n_u16(0);
    uint32x4_t result_level2 = vdupq_n_u32(0);
    uint64x2_t vsum = vdupq_n_u64(0);

    uint8x8_t h_vec;
    uint8x8_t x_vec = vdup_n_u8(1);
    uint32_t t0, t1;
    bool flag;
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

            vsum = vaddq_u64(vsum, vmovl_u32(vget_low_u32(result_level2)));
            vsum = vaddq_u64(vsum, vmovl_u32(vget_high_u32(result_level2)));

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

        vsum = vaddq_u64(vsum, vmovl_u32(vget_low_u32(result_level2)));
        vsum = vaddq_u64(vsum, vmovl_u32(vget_high_u32(result_level2)));

        result_level1 = vdupq_n_u16(0);
        flag = true;
    }
    uint64_t sum_lst[2];
    vst1q_u64(sum_lst, vsum);
    sum = sum_lst[0] + sum_lst[1];

    done = vec_size;
#endif // __ARM_NEON

    for (; done<len; done++) {
        sum += *data;
        data++;
    }

    return sum * 1.0 / len;
}

float array_mean_u8_asimd4(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON

#if __aarch64__
    size_t step = 8*4;
    size_t vec_size = len - len % step;

    uint8x8x4_t vdata;
    uint16x8_t v20, v21, v22, v23;
    uint64_t sum0=0, sum1=0, sum2=0, sum3=0;
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

    uint8x8_t v1;
    uint16x8_t v2;
    uint32x4_t v30, v31;

    uint64x2_t v40 = vdupq_n_u64(0);
    uint64x2_t v41 = vdupq_n_u64(0);
    uint64x2_t v42 = vdupq_n_u64(0);
    uint64x2_t v43 = vdupq_n_u64(0);

    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1_u8(data);
        data += step;
        v2 = vmovl_u8(v1);
        v30 = vmovl_u16(vget_low_u16(v2));
        v31 = vmovl_u16(vget_high_u16(v2));

        v40 = vaddq_u64(v40, vmovl_u32(vget_low_u32(v30)));
        v41 = vaddq_u64(v41, vmovl_u32(vget_high_u32(v30)));
        v42 = vaddq_u64(v42, vmovl_u32(vget_low_u32(v31)));
        v43 = vaddq_u64(v43, vmovl_u32(vget_high_u32(v31)));
    }
    v40 = vaddq_u64(v40, v41);
    v40 = vaddq_u64(v40, v42);
    v40 = vaddq_u64(v40, v43);

    uint64_t sum_lst[2];
    vst1q_u64(sum_lst, v40);
    sum += sum_lst[0] + sum_lst[1];
#endif // __aarch64__
    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    return sum * 1.0 / len;
}

// asimd3的优化
float array_mean_u8_asimd5(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;

#ifdef __ARM_NEON
    size_t step = 8*4;
    size_t vec_size = len - len % step;

    uint16x8x4_t result_level1;
    result_level1.val[0] = vdupq_n_u16(0);
    result_level1.val[1] = vdupq_n_u16(0);
    result_level1.val[2] = vdupq_n_u16(0);
    result_level1.val[3] = vdupq_n_u16(0);

    uint32x4x4_t result_level2;
    result_level2.val[0] = vdupq_n_u32(0);
    result_level2.val[1] = vdupq_n_u32(0);
    result_level2.val[2] = vdupq_n_u32(0);
    result_level2.val[3] = vdupq_n_u32(0);

    uint64x2x4_t vsum;
    vsum.val[0] = vdupq_n_u64(0);
    vsum.val[1] = vdupq_n_u64(0);
    vsum.val[2] = vdupq_n_u64(0);
    vsum.val[3] = vdupq_n_u64(0);

    uint8x8x4_t h_vec;
    uint8x8x4_t x_vec;
    x_vec.val[0] = vdup_n_u8(1);
    x_vec.val[1] = vdup_n_u8(1);
    x_vec.val[2] = vdup_n_u8(1);
    x_vec.val[3] = vdup_n_u8(1);

    uint32_t t0, t1;
    bool flag;
    for(size_t i=0; i<vec_size; i+=step) {
        h_vec = vld4_u8(data);
        data += step;
        flag = false;

        result_level1.val[0] = vmlal_u8(result_level1.val[0], h_vec.val[0], x_vec.val[0]);
        result_level1.val[1] = vmlal_u8(result_level1.val[1], h_vec.val[1], x_vec.val[1]);
        result_level1.val[2] = vmlal_u8(result_level1.val[2], h_vec.val[2], x_vec.val[2]);
        result_level1.val[3] = vmlal_u8(result_level1.val[3], h_vec.val[3], x_vec.val[3]);

        if( i % 256 == 0) {
            //16x8 => 32x4

            // .val[0]
            t0 = vgetq_lane_u16(result_level1.val[0], 0);
            t1 = vgetq_lane_u16(result_level1.val[0], 1);
            result_level2.val[0] = vsetq_lane_u32(t0+t1, result_level2.val[0], 0);

            t0 = vgetq_lane_u16(result_level1.val[0], 2);
            t1 = vgetq_lane_u16(result_level1.val[0], 3);
            result_level2.val[0] = vsetq_lane_u32(t0+t1, result_level2.val[0], 1);

            t0 = vgetq_lane_u16(result_level1.val[0], 4);
            t1 = vgetq_lane_u16(result_level1.val[0], 5);
            result_level2.val[0] = vsetq_lane_u32(t0+t1, result_level2.val[0], 2);

            t0 = vgetq_lane_u16(result_level1.val[0], 6);
            t1 = vgetq_lane_u16(result_level1.val[0], 7);
            result_level2.val[0] = vsetq_lane_u32(t0+t1, result_level2.val[0], 3);

            vsum.val[0] = vaddq_u64(vsum.val[0], vmovl_u32(vget_low_u32(result_level2.val[0])));
            vsum.val[0] = vaddq_u64(vsum.val[0], vmovl_u32(vget_high_u32(result_level2.val[0])));

            result_level1.val[0] = vdupq_n_u16(0);

            // .val[1]
            t0 = vgetq_lane_u16(result_level1.val[1], 0);
            t1 = vgetq_lane_u16(result_level1.val[1], 1);
            result_level2.val[1] = vsetq_lane_u32(t0+t1, result_level2.val[1], 0);

            t0 = vgetq_lane_u16(result_level1.val[1], 2);
            t1 = vgetq_lane_u16(result_level1.val[1], 3);
            result_level2.val[1] = vsetq_lane_u32(t0+t1, result_level2.val[1], 1);

            t0 = vgetq_lane_u16(result_level1.val[1], 4);
            t1 = vgetq_lane_u16(result_level1.val[1], 5);
            result_level2.val[1] = vsetq_lane_u32(t0+t1, result_level2.val[1], 2);

            t0 = vgetq_lane_u16(result_level1.val[1], 6);
            t1 = vgetq_lane_u16(result_level1.val[1], 7);
            result_level2.val[1] = vsetq_lane_u32(t0+t1, result_level2.val[1], 3);

            vsum.val[1] = vaddq_u64(vsum.val[1], vmovl_u32(vget_low_u32(result_level2.val[1])));
            vsum.val[1] = vaddq_u64(vsum.val[1], vmovl_u32(vget_high_u32(result_level2.val[1])));

            result_level1.val[1] = vdupq_n_u16(0);

            // .val[2]
            t0 = vgetq_lane_u16(result_level1.val[2], 0);
            t1 = vgetq_lane_u16(result_level1.val[2], 1);
            result_level2.val[2] = vsetq_lane_u32(t0+t1, result_level2.val[2], 0);

            t0 = vgetq_lane_u16(result_level1.val[2], 2);
            t1 = vgetq_lane_u16(result_level1.val[2], 3);
            result_level2.val[2] = vsetq_lane_u32(t0+t1, result_level2.val[2], 1);

            t0 = vgetq_lane_u16(result_level1.val[2], 4);
            t1 = vgetq_lane_u16(result_level1.val[2], 5);
            result_level2.val[2] = vsetq_lane_u32(t0+t1, result_level2.val[2], 2);

            t0 = vgetq_lane_u16(result_level1.val[2], 6);
            t1 = vgetq_lane_u16(result_level1.val[2], 7);
            result_level2.val[2] = vsetq_lane_u32(t0+t1, result_level2.val[2], 3);

            vsum.val[2] = vaddq_u64(vsum.val[2], vmovl_u32(vget_low_u32(result_level2.val[2])));
            vsum.val[2] = vaddq_u64(vsum.val[2], vmovl_u32(vget_high_u32(result_level2.val[2])));

            result_level1.val[2] = vdupq_n_u16(0);

            // .val[3]
            t0 = vgetq_lane_u16(result_level1.val[3], 0);
            t1 = vgetq_lane_u16(result_level1.val[3], 1);
            result_level2.val[3] = vsetq_lane_u32(t0+t1, result_level2.val[3], 0);

            t0 = vgetq_lane_u16(result_level1.val[3], 2);
            t1 = vgetq_lane_u16(result_level1.val[3], 3);
            result_level2.val[3] = vsetq_lane_u32(t0+t1, result_level2.val[3], 1);

            t0 = vgetq_lane_u16(result_level1.val[3], 4);
            t1 = vgetq_lane_u16(result_level1.val[3], 5);
            result_level2.val[3] = vsetq_lane_u32(t0+t1, result_level2.val[3], 2);

            t0 = vgetq_lane_u16(result_level1.val[3], 6);
            t1 = vgetq_lane_u16(result_level1.val[3], 7);
            result_level2.val[3] = vsetq_lane_u32(t0+t1, result_level2.val[3], 3);

            vsum.val[3] = vaddq_u64(vsum.val[3], vmovl_u32(vget_low_u32(result_level2.val[3])));
            vsum.val[3] = vaddq_u64(vsum.val[3], vmovl_u32(vget_high_u32(result_level2.val[3])));

            result_level1.val[3] = vdupq_n_u16(0);

            flag = true;
        }
    }
    if(!flag) {
        //16x8 => 32x4

        // .val[0]
        t0 = vgetq_lane_u16(result_level1.val[0], 0);
        t1 = vgetq_lane_u16(result_level1.val[0], 1);
        result_level2.val[0] = vsetq_lane_u32(t0+t1, result_level2.val[0], 0);

        t0 = vgetq_lane_u16(result_level1.val[0], 2);
        t1 = vgetq_lane_u16(result_level1.val[0], 3);
        result_level2.val[0] = vsetq_lane_u32(t0+t1, result_level2.val[0], 1);

        t0 = vgetq_lane_u16(result_level1.val[0], 4);
        t1 = vgetq_lane_u16(result_level1.val[0], 5);
        result_level2.val[0] = vsetq_lane_u32(t0+t1, result_level2.val[0], 2);

        t0 = vgetq_lane_u16(result_level1.val[0], 6);
        t1 = vgetq_lane_u16(result_level1.val[0], 7);
        result_level2.val[0] = vsetq_lane_u32(t0+t1, result_level2.val[0], 3);

        vsum.val[0] = vaddq_u64(vsum.val[0], vmovl_u32(vget_low_u32(result_level2.val[0])));
        vsum.val[0] = vaddq_u64(vsum.val[0], vmovl_u32(vget_high_u32(result_level2.val[0])));

        result_level1.val[0] = vdupq_n_u16(0);

        // .val[1]
        t0 = vgetq_lane_u16(result_level1.val[1], 0);
        t1 = vgetq_lane_u16(result_level1.val[1], 1);
        result_level2.val[1] = vsetq_lane_u32(t0+t1, result_level2.val[1], 0);

        t0 = vgetq_lane_u16(result_level1.val[1], 2);
        t1 = vgetq_lane_u16(result_level1.val[1], 3);
        result_level2.val[1] = vsetq_lane_u32(t0+t1, result_level2.val[1], 1);

        t0 = vgetq_lane_u16(result_level1.val[1], 4);
        t1 = vgetq_lane_u16(result_level1.val[1], 5);
        result_level2.val[1] = vsetq_lane_u32(t0+t1, result_level2.val[1], 2);

        t0 = vgetq_lane_u16(result_level1.val[1], 6);
        t1 = vgetq_lane_u16(result_level1.val[1], 7);
        result_level2.val[1] = vsetq_lane_u32(t0+t1, result_level2.val[1], 3);

        vsum.val[1] = vaddq_u64(vsum.val[1], vmovl_u32(vget_low_u32(result_level2.val[1])));
        vsum.val[1] = vaddq_u64(vsum.val[1], vmovl_u32(vget_high_u32(result_level2.val[1])));

        result_level1.val[1] = vdupq_n_u16(0);

        // .val[2]
        t0 = vgetq_lane_u16(result_level1.val[2], 0);
        t1 = vgetq_lane_u16(result_level1.val[2], 1);
        result_level2.val[2] = vsetq_lane_u32(t0+t1, result_level2.val[2], 0);

        t0 = vgetq_lane_u16(result_level1.val[2], 2);
        t1 = vgetq_lane_u16(result_level1.val[2], 3);
        result_level2.val[2] = vsetq_lane_u32(t0+t1, result_level2.val[2], 1);

        t0 = vgetq_lane_u16(result_level1.val[2], 4);
        t1 = vgetq_lane_u16(result_level1.val[2], 5);
        result_level2.val[2] = vsetq_lane_u32(t0+t1, result_level2.val[2], 2);

        t0 = vgetq_lane_u16(result_level1.val[2], 6);
        t1 = vgetq_lane_u16(result_level1.val[2], 7);
        result_level2.val[2] = vsetq_lane_u32(t0+t1, result_level2.val[2], 3);

        vsum.val[2] = vaddq_u64(vsum.val[2], vmovl_u32(vget_low_u32(result_level2.val[2])));
        vsum.val[2] = vaddq_u64(vsum.val[2], vmovl_u32(vget_high_u32(result_level2.val[2])));

        result_level1.val[2] = vdupq_n_u16(0);

        // .val[3]
        t0 = vgetq_lane_u16(result_level1.val[3], 0);
        t1 = vgetq_lane_u16(result_level1.val[3], 1);
        result_level2.val[3] = vsetq_lane_u32(t0+t1, result_level2.val[3], 0);

        t0 = vgetq_lane_u16(result_level1.val[3], 2);
        t1 = vgetq_lane_u16(result_level1.val[3], 3);
        result_level2.val[3] = vsetq_lane_u32(t0+t1, result_level2.val[3], 1);

        t0 = vgetq_lane_u16(result_level1.val[3], 4);
        t1 = vgetq_lane_u16(result_level1.val[3], 5);
        result_level2.val[3] = vsetq_lane_u32(t0+t1, result_level2.val[3], 2);

        t0 = vgetq_lane_u16(result_level1.val[3], 6);
        t1 = vgetq_lane_u16(result_level1.val[3], 7);
        result_level2.val[3] = vsetq_lane_u32(t0+t1, result_level2.val[3], 3);

        vsum.val[3] = vaddq_u64(vsum.val[3], vmovl_u32(vget_low_u32(result_level2.val[3])));
        vsum.val[3] = vaddq_u64(vsum.val[3], vmovl_u32(vget_high_u32(result_level2.val[3])));

        result_level1.val[3] = vdupq_n_u16(0);

        flag = true;
    }
    vsum.val[0] = vaddq_u64(vsum.val[0], vsum.val[1]);
    vsum.val[0] = vaddq_u64(vsum.val[0], vsum.val[2]);
    vsum.val[0] = vaddq_u64(vsum.val[0], vsum.val[3]);

    uint64_t sum_lst[2];
    vst1q_u64(sum_lst, vsum.val[0]);
    sum = sum_lst[0] + sum_lst[1];

    done = vec_size;
#endif // __ARM_NEON

    for (; done<len; done++) {
        sum += *data;
        data++;
    }

    return sum * 1.0 / len;
}

// 基于asimd4，vld4改为vld4q
float array_mean_u8_asimd6(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON

#if __aarch64__
    size_t step = 16*4;
    size_t vec_size = len - len % step;

    uint8x16x4_t vdata;
    uint16x8_t v2;
    for (size_t i=0; i<vec_size; i+=step) {
        vdata = vld4q_u8(data);
        data += step;
        v2 = vaddq_u16(vmovl_u8(vget_low_u8(vdata.val[0])), vmovl_u8(vget_high_u8(vdata.val[0])));
        v2 = vaddq_u16(v2, vaddq_u16(vmovl_u8(vget_low_u8(vdata.val[1])), vmovl_u8(vget_high_u8(vdata.val[1]))));
        v2 = vaddq_u16(v2, vaddq_u16(vmovl_u8(vget_low_u8(vdata.val[2])), vmovl_u8(vget_high_u8(vdata.val[2]))));
        v2 = vaddq_u16(v2, vaddq_u16(vmovl_u8(vget_low_u8(vdata.val[3])), vmovl_u8(vget_high_u8(vdata.val[3]))));
        sum += vaddvq_u16(v2);
    }
#else // 注意：armv7没有vaddv指令，下面的实现是从asimd1拷贝来的。。慢。
    size_t step = 8;
    size_t vec_size = len - len % step;

    uint8x8_t v1;
    uint16x8_t v2;
    uint32x4_t v30, v31;

    uint64x2_t v40 = vdupq_n_u64(0);
    uint64x2_t v41 = vdupq_n_u64(0);
    uint64x2_t v42 = vdupq_n_u64(0);
    uint64x2_t v43 = vdupq_n_u64(0);

    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1_u8(data);
        data += step;
        v2 = vmovl_u8(v1);
        v30 = vmovl_u16(vget_low_u16(v2));
        v31 = vmovl_u16(vget_high_u16(v2));

        v40 = vaddq_u64(v40, vmovl_u32(vget_low_u32(v30)));
        v41 = vaddq_u64(v41, vmovl_u32(vget_high_u32(v30)));
        v42 = vaddq_u64(v42, vmovl_u32(vget_low_u32(v31)));
        v43 = vaddq_u64(v43, vmovl_u32(vget_high_u32(v31)));
    }
    v40 = vaddq_u64(v40, v41);
    v40 = vaddq_u64(v40, v42);
    v40 = vaddq_u64(v40, v43);

    uint64_t sum_lst[2];
    vst1q_u64(sum_lst, v40);
    sum += sum_lst[0] + sum_lst[1];
#endif // __aarch64__
    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    return sum * 1.0 / len;
}

// asimd3的重构
float array_mean_u8_asimd70(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;

#ifdef __ARM_NEON
    size_t step = 8;
    size_t vec_size = len - len % step;

    uint8x8_t v0;
    uint16x8_t v2 = vdupq_n_u16(0);
    uint8x8_t v_one = vdup_n_u8(1);
    bool flag;

    uint16_t v2_mem[8];
    for(size_t i=0; i<vec_size; i+=step) {
        v0 = vld1_u8(data);
        data += step;
        v2 = vmlal_u8(v2, v0, v_one);
        flag = false;

        if( i % 256 == 0) {
            vst1q_u16(v2_mem, v2);
            sum += v2_mem[0] + v2_mem[1] + v2_mem[2] + v2_mem[3] + v2_mem[4] + v2_mem[5] + v2_mem[6] + v2_mem[7];
            v2 = vdupq_n_u16(0);
            flag = true;
        }
    }
    if(!flag) {
        vst1q_u16(v2_mem, v2);
        sum += v2_mem[0] + v2_mem[1] + v2_mem[2] + v2_mem[3] + v2_mem[4] + v2_mem[5] + v2_mem[6] + v2_mem[7];
        v2 = vdupq_n_u16(0);
        flag = true;
    }

    done = vec_size;
#endif // __ARM_NEON

    for (; done<len; done++) {
        sum += *data;
        data++;
    }

    return sum * 1.0 / len;
}

// 基于array_mean_u8_asimd70，vld1改为vld1q, 1.01ms->0.63ms @ armv8
float array_mean_u8_asimd71(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;

#ifdef __ARM_NEON
    size_t step = 16;
    size_t vec_size = len - len % step;

    uint8x16_t v0;
    uint16x8_t v2 = vdupq_n_u16(0);
    uint8x8_t v_one = vdup_n_u8(1);
    bool flag;

    uint16_t v2_mem[8];
    for(size_t i=0; i<vec_size; i+=step) {
        v0 = vld1q_u8(data);
        data += step;
        v2 = vmlal_u8(v2, vget_low_u8(v0), v_one);
        v2 = vmlal_u8(v2, vget_high_u8(v0), v_one);
        flag = false;

        if( i % 256 == 0) {
            vst1q_u16(v2_mem, v2);
            sum += v2_mem[0] + v2_mem[1] + v2_mem[2] + v2_mem[3] + v2_mem[4] + v2_mem[5] + v2_mem[6] + v2_mem[7];
            v2 = vdupq_n_u16(0);
            flag = true;
        }
    }
    if(!flag) {
        vst1q_u16(v2_mem, v2);
        sum += v2_mem[0] + v2_mem[1] + v2_mem[2] + v2_mem[3] + v2_mem[4] + v2_mem[5] + v2_mem[6] + v2_mem[7];
        v2 = vdupq_n_u16(0);
        flag = true;
    }

    done = vec_size;
#endif // __ARM_NEON

    for (; done<len; done++) {
        sum += *data;
        data++;
    }

    return sum * 1.0 / len;
}

// 有时候比array_mean_u8_asimd71快一点；和OpenCV在armv7速度基本一样，或更快
float array_mean_u8_asimd7(unsigned char* data, size_t len) {
    uint64_t sum = 0;
    size_t done = 0;

#ifdef __ARM_NEON
    size_t step = 16*2;
    size_t vec_size = len - len % step;

    uint8x16x2_t v0;
    uint16x8_t v2 = vdupq_n_u16(0);
    bool flag;

    uint16_t v2_mem[8];
    for(size_t i=0; i<vec_size; i+=step) {
        v0 = vld2q_u8(data);
        data += step;
        flag = false;

        v2 = vaddq_u16(v2, vmovl_u8(vget_low_u8(v0.val[0])));
        v2 = vaddq_u16(v2, vmovl_u8(vget_high_u8(v0.val[0])));
        v2 = vaddq_u16(v2, vmovl_u8(vget_low_u8(v0.val[1])));
        v2 = vaddq_u16(v2, vmovl_u8(vget_high_u8(v0.val[1])));

        if( i % 256 == 0) {
            vst1q_u16(v2_mem, v2);
            sum += v2_mem[0] + v2_mem[1] + v2_mem[2] + v2_mem[3] + v2_mem[4] + v2_mem[5] + v2_mem[6] + v2_mem[7];
            v2 = vdupq_n_u16(0);
            flag = true;
        }
    }
    if(!flag) {
        vst1q_u16(v2_mem, v2);
        sum += v2_mem[0] + v2_mem[1] + v2_mem[2] + v2_mem[3] + v2_mem[4] + v2_mem[5] + v2_mem[6] + v2_mem[7];
        v2 = vdupq_n_u16(0);
        flag = true;
    }

    done = vec_size;
#endif // __ARM_NEON

    for (; done<len; done++) {
        sum += *data;
        data++;
    }

    return sum * 1.0 / len;
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

float array_mean_s8_naive(int8_t* data, size_t len) {
    int64_t sum = 0; // note: if use float type for `sum`, when sum>=16777216 and as integer, sum would be incorrect
    for (size_t i=0; i<len; i++) {
        sum += data[i];
    }
    return sum*1.0 / len;
}

float array_mean_s8_asimd(int8_t* data, size_t len)
{
    int64_t sum = 0;
    size_t done = 0;

#ifdef __ARM_NEON
    size_t step = 8;
    size_t vec_size = len - len % step;

    int16x8_t result_level1 = vdupq_n_s16(0);
    int32x4_t result_level2 = vdupq_n_s32(0);
    int64x2_t vsum = vdupq_n_s64(0);
    
    int8x8_t h_vec;
    int8x8_t x_vec = vdup_n_s8(1);
    int t0, t1;
    bool flag;
    for(size_t i=0; i<vec_size; i++) {
        h_vec = vld1_s8(data);
        data += step;
        result_level1 = vmlal_s8(result_level1, h_vec, x_vec);
        flag = false;

        if( i % 256 == 0) {
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

            vsum = vaddq_s64(vsum, vmovl_s32(vget_low_s32(result_level2)));
            vsum = vaddq_s64(vsum, vmovl_s32(vget_high_s32(result_level2)));

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

        vsum = vaddq_s64(vsum, vmovl_s32(vget_low_s32(result_level2)));
        vsum = vaddq_s64(vsum, vmovl_s32(vget_high_s32(result_level2)));

        result_level1 = vdupq_n_s16(0);
        flag = true;
    }
    int64_t sum_lst[2];
    vst1q_s64(sum_lst, vsum);
    sum += sum_lst[0] + sum_lst[1];

    done = vec_size;
#endif // __ARM_NEON

    for (; done<len; done++) {
        sum += *data;
        data++;
    }

    return sum * 1.0 / len;
}