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

// dotproduct
static float dotproduct_naive(size_t len, float* va, float* vb);
static float dotproduct_asimd(size_t len, float* va, float* vb);
static float dotproduct_psimd(size_t len, float* va, float* vb);

// utils
static int check_array_equal(size_t len, float* data, float* gt);

// relu
static void relu_naive(size_t len, float* input, float* output);
static void relu_aaive(size_t len, float* input, float* output);
static void relu_psimd(size_t len, float* input, float* output);

// relu inplace
static void relu_inplace_naive(size_t len, float* data);
static void relu_inplace_asimd(size_t len, float* data);
static void relu_inplace_psimd(size_t len, float* data);

//----------------------------------------------------------------------
// function implementations
//----------------------------------------------------------------------
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
#ifdef PIXEL_SIMD_NEON
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
#elif defined(PIXEL_SIMD_SSE)
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

void relu_naive(size_t len, float* input, float* output) {
    for (size_t i = 0; i < len; i++) {
        output[i] = (input[i] > 0) ? input[i] : 0;
    }
}

void relu_psimd(size_t len, float* input, float* output) {
    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    size_t vec_size = len - len % step;
    v_float32x4 zero = vq_setzero_f32();
    v_float32x4 v1, v2, mask;
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vq_load_f32(input + i);
        mask = vq_cmplt_f32(v1, zero);
        v2 = vq_bitselect_f32(mask, zero, v1);
        vq_store_f32(output+i, v2);
    }
    
    for (size_t i=vec_size; i<len; i++) {
        output[i] = (input[i] > 0) ? input[i] : 0;
    }
}

void relu_asimd(size_t len, float* input, float* output) {
    size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    vec_size = len - len % step;
    float32x4_t zero = vdupq_n_f32(0);
    float32x4_t mask;
    float32x4_t v1;
    float32x4_t v2;
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1q_f32(input + i);
        mask = vcltq_f32(v1, zero);
        v2 = vbslq_f32(mask, zero, v1);
        vst1q_f32(output+i, v2);
    }
#elif defined(PIXEL_SIMD_SSE)
#endif
    for (size_t i=vec_size; i<len; i++) {
        output[i] = (input[i] > 0) ? input[i] : 0;
    }
}

void relu_inplace_naive(size_t len, float* data) {
    for (size_t i=0; i<len; i++) {
        data[i] = (data[i] > 0) ? data[i] : 0;
    }
}

void relu_inplace_asimd(size_t len, float* data) {
    size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    vec_size = len - len % step;
    float32x4_t value, mask;
    float32x4_t zero = vdupq_n_f32(0);
    for (size_t i=0; i<vec_size; i+=step) {
        value = vld1q_f32(data + i);
        mask = vcltq_f32(value, zero);
        value = vbslq_f32(mask, zero, value);
        vst1q_f32(data + i, value);
    }
#elif defined(PIXEL_SIMD_SSE)
#endif
    for (size_t i=vec_size; i<len; i++) {
        data[i] = (data[i] > 0) ? data[i] : 0;
    }
}

void relu_inplace_psimd(size_t len, float* data) {
    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    size_t vec_size = len - len % step;
    v_float32x4 value, mask;
    v_float32x4 zero = vq_setzero_f32();
    for (size_t i=0; i<vec_size; i+=step) {
        value = vq_load_f32(data+i);
        mask = vq_cmplt_f32(value, zero);
        value = vq_bitselect_f32(mask, zero, value);
        vq_store_f32(data+i, value);
    }
    
    for (size_t i=vec_size; i<len; i++) {
        data[i] = (data[i] > 0) ? data[i] : 0;
    }
}

int check_array_equal(size_t len, float* data, float* gt)
{
    int cnt = 0;
    for (size_t i=0; i<len; i++) {
        if (data[i]!=gt[i]) {
            cnt++;
        }
    }
    return cnt;
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

void test_relu_f32()
{
    //size_t len = 2000000;
    size_t len = 224*224*32;
    float* input = (float*)malloc(sizeof(float)*len);
    float* output_naive = (float*)malloc(sizeof(float)*len);
    float* output_psimd = (float*)malloc(sizeof(float)*len);
    float* output_asimd = (float*)malloc(sizeof(float)*len);

    g_state.a = 7767517;
    double r_start = pixel_get_current_time();
    for (size_t i=0; i<len; i++) {
        input[i] = pixel_fast_random_float(-233.f, 233.f);
    }
    double r_cost = pixel_get_current_time() - r_start;
    printf("time cost for random assign is %.6lf ms\n", r_cost);

    double t_start, t_cost;
    float res;
    int not_equal_count;
    
    t_start = pixel_get_current_time();
    relu_naive(len, input, output_naive);
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_naive, time cost is %.6lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    relu_psimd(len, input, output_psimd);
    t_cost = pixel_get_current_time() - t_start;
    not_equal_count = check_array_equal(len, output_psimd, output_naive);
    printf("relu_psimd, not_equal_count=%d, time cost is %.6lf ms\n", not_equal_count, t_cost);

    t_start = pixel_get_current_time();
    relu_asimd(len, input, output_asimd);
    t_cost = pixel_get_current_time() - t_start;
    not_equal_count = check_array_equal(len, output_asimd, output_naive);
    printf("relu_asimd, not_equal_count=%d, time cost is %.6lf ms\n", not_equal_count, t_cost);
}

void test_relu_inplace_f32()
{
    size_t len = 224*224*32;
    size_t buf_len = sizeof(float)*len;
    float* data = (float*)malloc(buf_len);;
    float* data_naive = (float*)malloc(buf_len);
    float* data_psimd = (float*)malloc(buf_len);
    float* data_asimd = (float*)malloc(buf_len);

    g_state.a = 7767517;
    double r_start = pixel_get_current_time();
    for (size_t i=0; i<len; i++) {
        data[i] = pixel_fast_random_float(-233.f, 233.f);
    }
    double r_cost = pixel_get_current_time() - r_start;
    printf("time cost for random assign is %.6lf ms\n", r_cost);


    memcpy(data_naive, data, buf_len);
    memcpy(data_psimd, data, buf_len);
    memcpy(data_asimd, data, buf_len);

    int not_equal_count;
    double t_start, t_cost;

    t_start = pixel_get_current_time();
    relu_inplace_naive(len, data_naive);
    t_cost = pixel_get_current_time() - t_start;
    printf("relu_inplace_naive, time cost is %.6lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    relu_inplace_asimd(len, data_asimd);
    t_cost = pixel_get_current_time() - t_start;
    not_equal_count = check_array_equal(len, data_asimd, data_naive);
    printf("relu_inplace_asimd, not_equal_count=%d, time cost is %.6lf ms\n", not_equal_count, t_cost);

    t_start = pixel_get_current_time();
    relu_inplace_psimd(len, data_psimd);
    t_cost = pixel_get_current_time() - t_start;
    not_equal_count = check_array_equal(len, data_psimd, data_naive);
    printf("relu_inplace_psimd, not_equal_count=%d, time cost is %.6lf ms\n", not_equal_count, t_cost);

}

int main() {
    //test_dotproduct_f32();
    //test_relu_f32();
    test_relu_inplace_f32();

    return 0;
}