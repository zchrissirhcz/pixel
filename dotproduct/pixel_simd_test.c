#include "pixel_simd.h"
#include "common/pixel_fast_rng.h"
#include "common/pixel_benchmark.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIXEL_LOGD(fmt, ...) fprintf(stderr, (fmt "\n"))

static void test_dotproduct_f32();

//_naive: naive implementation
//_psimd: pixel_simd implementation. will be optimized to simd by compiler.
//_asimd: sse and neon mixed implementation
static float dotproduct_naive(size_t len, float* va, float* vb);
static float dotproduct_psimd(size_t len, float* va, float* vb);
static float dotproduct_asimd(size_t len, float* va, float* vb);

float dotproduct_naive(size_t len, float* va, float* vb) {
    float sum = 0;
    for (size_t i=0; i<len; i++) {
        sum += va[i] * vb[i];
    }
    return sum;
}

float dotproduct_psimd(size_t len, float* va, float* vb) {
    float sum = 0;

    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    size_t vec_size = len - len % step;
    v_float32x4 v1, v2;
    v_float32x4 vres = vq_setzero_f32();
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vq_load_f32(va + i);
        v2 = vq_load_f32(vb + i);
        vres = vq_fmadd_f32(v1, v2, vres);
        //vres = vq_add_f32(vq_mul_f32(v1, v2), vres);
    }
    
    float sum_lst[PIXEL_VQ_BYTES];
    vq_store_f32(sum_lst, vres);
    for (size_t i=0; i<step; i++) {
        sum += sum_lst[i];
    }
    
    for (size_t i=vec_size; i<len; i++) {
        sum += va[i] * vb[i];
    }
    return sum;
}

float dotproduct_asimd(size_t len, float* va, float* vb) {
    float sum = 0;
    size_t vec_size = 0;
#ifdef PIXEL_NEON
    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    vec_size = len - len % step;
    float32x4_t v1, v2;
    float32x4_t vres = vdupq_n_f32(0);
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1q_f32(va + i);
        v2 = vld1q_f32(vb + i);
        vres = vmlaq_f32(vres, v1, v2);
        //vres = vaddq_f32(vmulq_f32(v1, v2), vres);
    }
    float sum_lst[step];
    vst1q_f32(sum_lst, vres);
    for (size_t i=0; i<step; i++) {
        sum += sum_lst[i];
    }
#elif defined(PIXEL_SSE)
    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    vec_size = len - len % step;
    __m128 v1, v2;
    __m128 vres = _mm_setzero_ps();
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = _mm_loadu_ps(va + i);
        v2 = _mm_loadu_ps(vb + i);
        vres = _mm_add_ps(_mm_mul_ps(v1, v2), vres);
    }
    float sum_lst[PIXEL_VQ_BYTES];
    _mm_store_ps(sum_lst, vres);
    for (size_t i=0; i<step; i++) {
        sum += sum_lst[i];
    }
#endif
    for (size_t i=vec_size; i<len; i++) {
        sum += va[i] * vb[i];
    }

    return sum;
}

void test_dotproduct_f32()
{
    size_t len = 200000000; //200M
    //size_t len = 20000;
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
    res = dotproduct_naive(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_naive,\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);

    t_start = pixel_get_current_time();
    res = dotproduct_psimd(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_psimd,\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);

    t_start = pixel_get_current_time();
    res = dotproduct_asimd(len, va, vb);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_asimd,\tresult is %.6f, time cost is %.6lf ms\n", res, t_cost);

    free(va);
    free(vb);
}

int main() {
    test_dotproduct_f32();
    //test_relu_f32();

    return 0;
}