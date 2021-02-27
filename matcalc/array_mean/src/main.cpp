#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <time.h>

#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"

#ifdef __ARM_NEON //will this macro be defined in T3's device?
#include<arm_neon.h>
#endif

float get_random_float(float s, float t){
    float v = (float)(rand()) / RAND_MAX;
    v = v * (t - s) + s;
    return v;
}

float mean_naive(int8_t* data, int len) {
    float res = 0;
    for(int i=0; i<len; i++) {
        res += data[i];
    }
    res /= len;
    return res;
}

// //ref: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0472j/chr1359124214828.html
// float mean_neon_s16(int16_t* data, int len) {
//     // initialize the accumulator vector to zero
//     int16x4_t acc = vdup_n_s16(0);
//     int32x2_t acc1;
//     int64x1_t acc2;

//     for(int i=0; i<len; i+=4) {
//         int16x4_t vec;
//         // load 4 values in parallel from the array
//         vec = vld1_s16(data);
//         // increment the array pointer to the next element
//         data += 4;
//         // add the vector to the accumulator vector
//         acc = vadd_s16(acc, vec);
//     }
//     // calculate the total
//     acc1 = vpaddl_s16(acc);
//     acc2 = vpaddl_s32(acc1);
//     int sum = (int)vget_lane_s64(acc2, 0);
//     return sum*1.0 / len;
// }

// //if len is very large, result may out of max_int16
// float mean_neon_(int8_t* data, int len) {
//     int16x8_t result_vec = vdupq_n_s16(0);
    
//     int8x8_t h_vec;

//     //int8x8_t x_vec = vdup_n_s32(1);
//     int8_t ones[] = {1, 1, 1, 1, 1, 1, 1, 1};
//     int8x8_t x_vec = vdup_n_s8(1);

//     for(int i=0; i<len/8; i++) {
//         //printf("=== i=%d\n", i);
//         h_vec = vld1_s8(&data[i*8]);
//         result_vec = vmlal_s8(result_vec, h_vec, x_vec);
//     }

//     float sum = 0;
//     sum += vgetq_lane_s16(result_vec, 0);
//     sum += vgetq_lane_s16(result_vec, 1);
//     sum += vgetq_lane_s16(result_vec, 2);
//     sum += vgetq_lane_s16(result_vec, 3);
//     sum += vgetq_lane_s16(result_vec, 4);
//     sum += vgetq_lane_s16(result_vec, 5);
//     sum += vgetq_lane_s16(result_vec, 6);
//     sum += vgetq_lane_s16(result_vec, 7);

//     if(len%8!=0) {
//         for(int k=len-(len%8); k<len; k++) {
//             sum += data[k];
//         }
//     }

//     float avg = sum / len;
//     return avg;
// }

float mean_neon(int8_t* data, int len) {
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
}

// //https://zhuanlan.zhihu.com/p/128640600
// void add_float_c(float* dst, float* src1, float* src2, int count)
//  {
//      int i;
//      for (i = 0; i < count; i++) {
//          dst[i] = src1[i] + src2[i];
//      }
//  }

//  void add_float_neon1(float* dst, float* src1, float* src2, int count)
//  {
//      int i;
//      for (i = 0; i < count; i += 4)
//      {
//         float32x4_t in1, in2, out;
//         in1 = vld1q_f32(src1);
//         src1 += 4;
//         in2 = vld1q_f32(src2);
//         src2 += 4;
//         out = vaddq_f32(in1, in2);
//         vst1q_f32(dst, out);
//         dst += 4;
        
//         // 下列代码只是描述如何使用AArch64专有代码的一个例子，不具有实际含义。
// #if defined (__aarch64__)
//          float32_t tmp = vaddvq_f32(in1);
// #endif

//     }
// }

// static void test1(){
//     //VHADD
//     //VRHADD
//     //VQADD
// }

int main() {
    const int N = 1920*1080; // multiple of 4
    int8_t data[N];
    srand(time(0)) ;
    for(int i=0; i<N; i++) {
        data[i] = (int8_t)get_random_float(-20.0f, 20.0f);
        //printf("data[%d]=%d\n", i, data[i]);
    }

    double t_start;
    const int COUNT = 100;
    float res1, res2;

    //----------------------------------------
    // naive
    //----------------------------------------
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<COUNT; i++) {
            res1 = mean_naive(data, N);
        }
        PIXEL_LOGD("mean_naive cost %lf ms", pixel_get_current_time() - t_start);
    }

    //----------------------------------------
    // naive
    //----------------------------------------
    {
        t_start = pixel_get_current_time();
        for (int i=0; i<COUNT; i++) {
            res2 = mean_neon(data, N);
        }
        PIXEL_LOGD("mean_neon cost %lf ms", pixel_get_current_time() - t_start);
    }

    PIXEL_LOGD("mean_naive returns %lf", res1);
    PIXEL_LOGD("mean_neon  returns %lf", res2);

    return 0;
}
