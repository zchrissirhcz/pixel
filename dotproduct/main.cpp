#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/pixel_benchmark.h"
#include "common/pixel_fast_rng.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/simd_intrinsics.hpp>

#ifdef USE_MIPP
#include "mipp.h"
#endif

#include <Eigen/Dense>
#include "xsimd/xsimd.hpp"

#ifdef _MSC_VER
#define USE_SSE 1
#endif

#ifdef USE_SSE
#include <xmmintrin.h> // SSE
#endif

float dotproduct1(size_t len, float* va, float* vb)
{
    float sum = 0;
    for (size_t i=0; i<len; i++) {
        sum += va[i] * vb[i];
    }
    return sum;
}

// OpenCV universal intrinsics based implementation
float dotproduct2(size_t len, float* va, float* vb)
{
    size_t step = sizeof(cv::v_float32)/sizeof(float);
    cv::v_float32 v_sum = cv::vx_setzero_f32();
    size_t vec_size = len - len % step;
    for (size_t i=0; i< vec_size; i+=step)
    {
        cv::v_float32 v1 = cv::vx_load(va+i);
        cv::v_float32 v2 = cv::vx_load(vb+i);
        v_sum += v1 * v2;
    }

    float sum = cv::v_reduce_sum(v_sum);

    for (size_t i= vec_size; i<len; i++) {
        sum += va[i] * vb[i];
    }

    return sum;
}

#ifdef __ARM_NEON
float dotproduct3(size_t len, float* va, float* vb)
{
    const size_t step = 4; // 128 / sizeof(float)
    size_t vec_size = len - len % step;
    float32x4_t vres = vdupq_n_f32(0);
    float32x4_t v1;
    float32x4_t v2;
    float32x4_t vtmp;
    for (size_t i = 0; i < vec_size; i+=step) {
        v1 = vld1q_f32(va + i);
        v2 = vld1q_f32(vb + i);
        vtmp = vmulq_f32(v1, v2);
        vres = vaddq_f32(vtmp, vres);
    }

    float sum_lst[4];
    vst1q_f32(sum_lst, vres);
    
    float sum = 0;
    for (size_t i = 0; i < step; i++) {
        sum += sum_lst[i];
    }
    for (size_t i = vec_size; i < len; i++) {
        sum += va[i] * vb[i];
    }
    return sum;
}

float dotproduct3_cache(size_t len, float* va, float* vb)
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
#endif

#ifdef USE_MIPP
// MIPP based implementation
float dotproduct4(size_t len, float* va, float* vb)
{
    constexpr int step = mipp::N<float>();
    //printf("--- mipp::N<float>() is %d\n", mipp::N<float>());
    size_t vec_size = len - len % step;
    mipp::Reg<float> rc;
    mipp::Reg<float> ra, rb;
    rc.set0();
    for (size_t i=0; i< vec_size; i+=step) {
        ra.load(va + i);
        rb.load(vb + i);
        rc += ra * rb;
    }

    float sum = rc.sum();

    for (size_t i= vec_size; i<len; i++) {
        sum += va[i] * vb[i];
    }

    return sum;
}
#endif

// Eigen based implementation
float dotproduct5(size_t len, float* va, float* vb)
{
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> vva(va, len);
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> vvb(vb, len);
    float res = vva.dot(vvb);
    return res;
}

// xsimd based implementation
float dotproduct6(size_t len, float* va, float* vb)
{
    constexpr size_t simd_size = xsimd::simd_type<float>::size;
    size_t vec_size = len - len % simd_size;

    //xsimd::batch<float, simd_size> vres;
    //for (size_t i = 0; i < simd_size; i++) {
    //    vres[i] = 0;
    //}
    xsimd::batch<float, simd_size> vres(0.f);
    for (size_t i=0; i<vec_size; i+=simd_size)
    {
        auto vva = xsimd::load_unaligned(va + i);
        auto vvb = xsimd::load_unaligned(vb + i);
        vres += vva * vvb;
    }
    float sum = 0;
    for (size_t i = 0; i < simd_size; i++) {
        sum += vres[i];
    }
    for (size_t i = vec_size; i < len; i++) {
        sum += va[i] * vb[i];
    }
    return sum;
}

#ifdef USE_SSE
// SSE based implementation
float dotproduct7(size_t len, float* va, float* vb)
{
    const size_t step = 4; // 128 / sizeof(float)
    size_t vec_size = len - len % step;
    __m128 vacc = _mm_setzero_ps();
    __m128 v1;
    __m128 v2;
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = _mm_loadu_ps(va + i);
        v2 = _mm_loadu_ps(vb + i);
        vacc = _mm_add_ps(_mm_mul_ps(v1, v2), vacc);
    }

    float sum_lst[step];
    _mm_store_ps(sum_lst, vacc);

    float sum = 0;
    for (size_t i = 0; i < step; i++) {
        sum += sum_lst[i];
    }

    for (size_t i = vec_size; i < len; i++) {
        sum += va[i] * vb[i];
    }
    return sum;
}
#endif
 
static void opencv_simd_test()
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
    //opencv_simd_test();

    size_t len = 200000000; //200M
    //size_t len = 9;
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
    printf("impl1(Naive),\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);

    t_start = pixel_get_current_time();
    res = dotproduct2(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl2(OpenCV),\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);

#ifdef __ARM_NEON
    t_start = pixel_get_current_time();
    res = dotproduct3(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl3(neon),\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);
#endif

#ifdef USE_MIPP
    t_start = pixel_get_current_time();
    res = dotproduct4(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl4(MIPP),\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);
#endif

    t_start = pixel_get_current_time();
    res = dotproduct5(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl5(Eigen),\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);

    t_start = pixel_get_current_time();
    res = dotproduct6(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl6(xsimd),\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);

#ifdef USE_SSE
    t_start = pixel_get_current_time();
    res = dotproduct7(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("impl7(sse),\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);
#endif

    free(va);
    free(vb);

    return 0;
}
