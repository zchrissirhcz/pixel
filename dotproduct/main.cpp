#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/pixel_benchmark.h"
#include "common/pixel_fast_rng.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/simd_intrinsics.hpp>


float dotproduct1(size_t len, float* va, float* vb)
{
    float sum = 0;
    for (size_t i=0; i<len; i++) {
        sum += va[i] * vb[i];
    }
    return sum;
}

float dotproduct2(size_t len, float* va, float* vb)
{
    size_t step = sizeof(cv::v_float32)/sizeof(float);
    printf("step is %zu\n", step);
    cv::v_float32 v_sum = cv::vx_setzero_f32();
    for (size_t i=0; i<len; i+=step)
    {
        cv::v_float32 v1 = cv::vx_load(va+i);
        cv::v_float32 v2 = cv::vx_load(vb+i);
        v_sum += v1 * v2;
    }

    float sum = cv::v_reduce_sum(v_sum);

    size_t remain_begin = len -1 - (len % step);
    for (size_t i=remain_begin; i<len; i++) {
        sum += va[i] * vb[i];
    }

    return sum;
}

float dotproduct3(size_t len, float* va, float* vb)
{
    int segments = len / 4;
    int remain = len - (segments/4*4)*4;

    float32x4_t partSum = vdupq_n_f32(0);
    float32x4_t sum1 = vdupq_n_f32(0);
    float32x4_t sum2 = vdupq_n_f32(0);
    float32x4_t sum3 = vdupq_n_f32(0);
    float32x4_t sum4 = vdupq_n_f32(0);
    for (int i=0; i+3<segments; i+=4)
    {
        float32x4_t v11 = vld1q_f32(va);
        float32x4_t v12 = vld1q_f32(vb);

        float32x4_t v21 = vld1q_f32(va+4);
        float32x4_t v22 = vld1q_f32(vb+4);

        float32x4_t v31 = vld1q_f32(va+8);
        float32x4_t v32 = vld1q_f32(vb+8);

        float32x4_t v41 = vld1q_f32(va+12);
        float32x4_t v42 = vld1q_f32(vb+12);

        sum1 = vmlaq_f32(sum1, v11, v12);
        sum2 = vmlaq_f32(sum2, v21, v22);
        sum3 = vmlaq_f32(sum3, v31, v32);
        sum4 = vmlaq_f32(sum4, v41, v42);

        va += 16;
        vb += 16;
    }
    partSum = sum1 + sum2 + sum3 + sum4;

    for (int i=0; i<remain; i++) {
        float32x4_t vector1Neon = vld1q_f32(va);
        float32x4_t vector2Neon = vld1q_f32(vb);
        partSum = vmlaq_f32(partSum, vector1Neon, vector2Neon);

        va += 4;
        vb += 4;
    }

    float sum = partSum[0] + partSum[1] + partSum[2] + partSum[3];

    return sum;
}

static void simd_test()
{
    using namespace cv;
#ifdef CV_SIMD
    printf("CV_SIMD is : " CVAUX_STR(CV_SIMD) "\n");
    printf("CV_SIMD_WIDTH is : " CVAUX_STR(CV_SIMD_WIDTH) "\n");
    printf("CV_SIMD128 is : " CVAUX_STR(CV_SIMD128) "\n");
    printf("CV_SIMD256 is : " CVAUX_STR(CV_SIMD256) "\n");
    printf("CV_SIMD512 is : " CVAUX_STR(CV_SIMD512) "\n");
#else
    printf("CV_SIMD is NOT defined\n");
#endif

#ifdef CV_SIMD
    printf("sizeof(v_uint8) = %d\n", (int)sizeof(v_uint8));
    printf("sizeof(v_int32) = %d\n", (int)sizeof(v_int32));
    printf("sizeof(v_float32) = %d\n", (int)sizeof(v_float32));
#endif
}

int main() {
    simd_test();

    size_t len = 200000000; //200M
    float* va = (float*)malloc(sizeof(float)*len);
    float* vb = (float*)malloc(sizeof(float)*len);

    g_state.a = 7767517;
    double r_start = pixel_get_current_time();
    for (size_t i=0; i<len; i++) {
        va[i] = pixel_fast_random_float(-1.2, 1.2);
        vb[i] = pixel_fast_random_float(-1.2, 1.2);
    }
    double r_cost = pixel_get_current_time() - r_start;
    printf("time cost for random assign is %.6lf ms\n", r_cost);

    double t_start, t_cost;
    float res;

    t_start = pixel_get_current_time();
    res = dotproduct1(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl1, result is %.6f, time cost is %lf ms\n", res, t_cost);

    t_start = pixel_get_current_time();
    res = dotproduct2(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl2, result is %.6f, time cost is %lf ms\n", res, t_cost);

    t_start = pixel_get_current_time();
    res = dotproduct3(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl3, result is %.6f, time cost is %lf ms\n", res, t_cost);

    free(va);
    free(vb);

    return 0;
}
