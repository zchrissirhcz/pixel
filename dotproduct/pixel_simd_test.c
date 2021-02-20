#include "pixel_simd.h"
#include "common/pixel_fast_rng.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_check.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define PIXEL_LOGD(fmt, ...) fprintf(stderr, (fmt "\n"))

//_naive: naive implementation
//_psimd: pixel_simd implementation. will be optimized to simd by compiler.
//_asimd: sse and neon mixed implementation

// dotproduct
static float dotproduct_naive(size_t len, float* va, float* vb);
static float dotproduct_asimd(size_t len, float* va, float* vb);
static float dotproduct_psimd(size_t len, float* va, float* vb);
static void test_dotproduct_f32();

// relu
static void relu_naive(size_t len, float* input, float* output);
static void relu_asimd(size_t len, float* input, float* output);
static void relu_psimd(size_t len, float* input, float* output);
static void test_relu_32();

// relu inplace
static void relu_inplace_naive(size_t len, float* data);
static void relu_inplace_asimd(size_t len, float* data);
static void relu_inplace_psimd(size_t len, float* data);
static void test_relu_inplace_f32();

// weighted sum
// res[i] = arr1[i]*weight1 + arr2[i]*weight2;
static void weighted_sum_naive(size_t len, const float* arr1, const float weight1, const float* arr2, const float weight2, float* res);
static void weighted_sum_asimd(size_t len, const float* arr1, const float weight1, const float* arr2, const float weight2, float* res);
static void weighted_sum_psimd(size_t len, const float* arr1, const float weight1, const float* arr2, const float weight2, float* res);
static void test_weighted_sum();

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
    v_float32x4 v1, v2;
    v_uint32x4 mask;
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

// nearly no optimization effect, sometimes inverse-optimization.
void relu_asimd_fast(size_t len, float* input, float* output) {
    size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
    float* sd_input = input;
    float* sd_output = output;
    //const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    const size_t step = 16;
    vec_size = len - len % step;
    float32x4_t zero = vdupq_n_f32(0);
    uint32x4_t mask1, mask2, mask3, mask4;
    float32x4_t v11, v12;
    float32x4_t v21, v22;
    float32x4_t v31, v32;
    float32x4_t v41, v42;
    for (size_t i=0; i<vec_size; i+=step) {
#if 0        
        v11 = vld1q_f32(sd_input);
        mask1 = vcltq_f32(v11, zero);
        v12 = vbslq_f32(mask1, zero, v11);
        vst1q_f32(sd_output, v12);

        v21 = vld1q_f32(sd_input+4);
        mask2 = vcltq_f32(v21, zero);
        v22 = vbslq_f32(mask2, zero, v21);
        vst1q_f32(sd_output+4, v22);

        v31 = vld1q_f32(sd_input+8);
        mask3 = vcltq_f32(v31, zero);
        v32 = vbslq_f32(mask3, zero, v31);
        vst1q_f32(sd_output+8, v32);

        v41 = vld1q_f32(sd_input+12);
        mask4 = vcltq_f32(v41, zero);
        v42 = vbslq_f32(mask4, zero, v41);
        vst1q_f32(sd_output+12, v42);

        sd_input += 16;
        sd_output += 16;
#else // still no optimization effect
        float32x4x4_t vin = vld4q_f32(sd_input);
        float32x4x4_t vout;
        uint32x4x4_t mask;
        mask.val[0] = vcltq_f32(vin.val[0], zero);
        mask.val[1] = vcltq_f32(vin.val[1], zero);
        mask.val[2] = vcltq_f32(vin.val[2], zero);
        mask.val[3] = vcltq_f32(vin.val[3], zero);
        vout.val[0] = vbslq_f32(mask.val[0], zero, vin.val[0]);
        vout.val[1] = vbslq_f32(mask.val[1], zero, vin.val[1]);
        vout.val[2] = vbslq_f32(mask.val[2], zero, vin.val[2]);
        vout.val[3] = vbslq_f32(mask.val[3], zero, vin.val[3]);
        vst4q_f32(sd_output, vout);

        sd_input += 16;
        sd_output += 16;
#endif
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
    v_float32x4 value;
    v_uint32x4 mask;
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

// res[i] = arr1[i] * weight1 + arr2[i] * weight2
void weighted_sum_naive(size_t len, const float* arr1, const float weight1, const float* arr2, const float weight2, float* res)
{
    for (size_t i=0; i<len; i++) {
        res[i] = arr1[i]*weight1 + arr2[i]*weight2;
    }
}

void weighted_sum_asimd(size_t len, const float* arr1, const float weight1, const float* arr2, const float weight2, float* res)
{
    size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    vec_size = len - len % step;
    float32x4_t v_weight1 = vdupq_n_f32(weight1);
    float32x4_t v_weight2 = vdupq_n_f32(weight2);
    float32x4_t v1, v2, vres;
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1q_f32(arr1+i);
        v2 = vld1q_f32(arr2+i);
        v1 = vmulq_f32(v1, v_weight1);
        v2 = vmulq_f32(v2, v_weight2);
        vres = vaddq_f32(v1, v2);
        vst1q_f32(res+i, vres);
    }
#elif defined(PIXEL_SIMD_SSE)
#endif
    for (size_t i=vec_size; i<len; i++) {
        res[i] = arr1[i]*weight1 + arr2[i]*weight2;
    }
}

// nearly no effect, even inverse-optimization.
void weighted_sum_asimd_fast(size_t len, const float* arr1, const float weight1, const float* arr2, const float weight2, float* res)
{
    size_t vec_size = 0;
#ifdef PIXEL_SIMD_NEON
    const float* ptr1 = arr1;
    const float* ptr2 = arr2;
    float* pres = res;
    //const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    const int step = 16;
    vec_size = len - len % step;

    // float32x4_t v11, v12, v1res;
    // float32x4_t v21, v22, v2res;
    // float32x4_t v31, v32, v3res;
    // float32x4_t v41, v42, v4res;
    for (size_t i=0; i<vec_size; i+=step) {
#if 0
        v11 = vld1q_f32(ptr1);
        v12 = vld1q_f32(ptr2);
        v11 = vmulq_f32(v11, v_weight1);
        v12 = vmulq_f32(v12, v_weight2);

        v1res = vaddq_f32(v11, v12);
        vst1q_f32(pres, v1res);

        ///

        v21 = vld1q_f32(ptr1+4);
        v22 = vld1q_f32(ptr2+4);
        v21 = vmulq_f32(v21, v_weight1);
        v22 = vmulq_f32(v22, v_weight2);

        v2res = vaddq_f32(v21, v22);
        vst1q_f32(pres+4, v2res);

        ///

        v31 = vld1q_f32(ptr1+8);
        v32 = vld1q_f32(ptr2+8);
        v31 = vmulq_f32(v31, v_weight1);
        v32 = vmulq_f32(v32, v_weight2);

        v3res = vaddq_f32(v31, v32);
        vst1q_f32(pres+8, v3res);

        ///

        v41 = vld1q_f32(ptr1+12);
        v42 = vld1q_f32(ptr2+12);
        v41 = vmulq_f32(v41, v_weight1);
        v42 = vmulq_f32(v42, v_weight2);

        v4res = vaddq_f32(v41, v42);
        vst1q_f32(pres+12, v4res);

        ptr1 += 16;
        ptr2 += 16;
        pres += 16;
#else
        float32x4x4_t v1 = vld4q_f32(ptr1); //第一个数组的数据
        float32x4x4_t v2 = vld4q_f32(ptr2); //第二个数组的数据
        float32x4_t v_weight1 = vdupq_n_f32(weight1); // 权值向量化
        float32x4_t v_weight2 = vdupq_n_f32(weight2); // 第二个权值向量化
        float32x4x4_t vout; // 存结果
        
    #if 0 //如下4句，结果正确
        vout.val[0] = vaddq_f32(vmulq_f32(v1.val[0], v_weight1), vmulq_f32(v2.val[0], v_weight2));
        vout.val[1] = vaddq_f32(vmulq_f32(v1.val[1], v_weight1), vmulq_f32(v2.val[1], v_weight2));
        vout.val[2] = vaddq_f32(vmulq_f32(v1.val[2], v_weight1), vmulq_f32(v2.val[2], v_weight2));
        vout.val[3] = vaddq_f32(vmulq_f32(v1.val[3], v_weight1), vmulq_f32(v2.val[3], v_weight2));
    #else //这4句，结果不对了。  fma 这样用似乎不对？
        // vout.val[0] = vmlaq_f32(vmulq_f32(v1.val[0], v_weight1), v2.val[0], v_weight2);
        // vout.val[1] = vmlaq_f32(vmulq_f32(v1.val[1], v_weight1), v2.val[1], v_weight2);
        // vout.val[2] = vmlaq_f32(vmulq_f32(v1.val[2], v_weight1), v2.val[2], v_weight2);
        // vout.val[3] = vmlaq_f32(vmulq_f32(v1.val[3], v_weight1), v2.val[3], v_weight2);

        vout.val[0] = vfmaq_f32(vmulq_f32(v1.val[0], v_weight1), v2.val[0], v_weight2);
        vout.val[1] = vfmaq_f32(vmulq_f32(v1.val[1], v_weight1), v2.val[1], v_weight2);
        vout.val[2] = vfmaq_f32(vmulq_f32(v1.val[2], v_weight1), v2.val[2], v_weight2);
        vout.val[3] = vfmaq_f32(vmulq_f32(v1.val[3], v_weight1), v2.val[3], v_weight2);

        // float32x4_t t0 = vmulq_f32(v1.val[0], v_weight1);
        // vout.val[0] = vfmaq_f32(t0, v2.val[0], v_weight2);
        // float32x4_t t1 = vmulq_f32(v1.val[1], v_weight1);
        // vout.val[1] = vfmaq_f32(t1, v2.val[1], v_weight2);
        // float32x4_t t2 = vmulq_f32(v1.val[2], v_weight1);
        // vout.val[2] = vfmaq_f32(t2, v2.val[2], v_weight2);
        // float32x4_t t3 = vmulq_f32(v1.val[3], v_weight1);
        // vout.val[3] = vfmaq_f32(t3, v2.val[3], v_weight2);

        vst4q_f32(pres, vout);
    #endif
        ptr1 += 16;  ptr2 += 16; pres += 16;
#endif
    }
#elif defined(PIXEL_SIMD_SSE)
#endif
    for (size_t i=vec_size; i<len; i++) {
        res[i] = arr1[i]*weight1 + arr2[i]*weight2;
    }
}

void weighted_sum_psimd(size_t len, const float* arr1, const float weight1, const float* arr2, const float weight2, float* res)
{
    const size_t step = PIXEL_VQ_BYTES / sizeof(float);
    size_t vec_size = len - len % step;
    
    v_float32x4 v_weight1 = vq_setvalue_f32(weight1);
    v_float32x4 v_weight2 = vq_setvalue_f32(weight2);
    v_float32x4 v1, v2, vres;
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vq_load_f32(arr1+i);
        v2 = vq_load_f32(arr2+i);
        v1 = vq_mul_f32(v1, v_weight1);
        v2 = vq_mul_f32(v2, v_weight2);
        vres = vq_add_f32(v1, v2);
        vq_store_f32(res+i, vres);
    }
    
    for (size_t i=vec_size; i<len; i++) {
        res[i] = arr1[i]*weight1 + arr2[i]*weight2;
    }
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
    //relu_asimd(len, input, output_asimd);
    relu_asimd_fast(len, input, output_asimd);
    t_cost = pixel_get_current_time() - t_start;
    not_equal_count = check_array_equal(len, output_asimd, output_naive);
    printf("relu_asimd, not_equal_count=%d, time cost is %.6lf ms\n", not_equal_count, t_cost);

    t_start = pixel_get_current_time();
    relu_psimd(len, input, output_psimd);
    t_cost = pixel_get_current_time() - t_start;
    not_equal_count = check_array_equal(len, output_psimd, output_naive);
    printf("relu_psimd, not_equal_count=%d, time cost is %.6lf ms\n", not_equal_count, t_cost);
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

    free(data);
    free(data_naive);
    free(data_psimd);
    free(data_asimd);
}

void test_weighted_sum()
{
    // prepare data
    const size_t len = 11484;
    //const size_t len = 9;
    const size_t buf_size = sizeof(float)*len;
    float* arr1 = (float*)malloc(buf_size);
    float* arr2 = (float*)malloc(buf_size);
    float* res_naive = (float*)malloc(buf_size);
    float* res_asimd = (float*)malloc(buf_size);
    float* res_psimd = (float*)malloc(buf_size);

    float weight1 = 0.5;
    float weight2 = 0.2;

    g_state.a = 7767517;
    double r_start = pixel_get_current_time();
    for (size_t i=0; i<len; i++) {
        arr1[i] = pixel_fast_random_float(-20.f, 12.f);
        arr2[i] = pixel_fast_random_float(-12.f, 12.f);
    }
    double r_cost = pixel_get_current_time() - r_start;
    printf("time cost for random assign is %.6lf ms\n", r_cost);

    // benchmark
    double t_start, t_cost;
    int not_equal_count;

    t_start = pixel_get_current_time();
    weighted_sum_naive(len, arr1, weight1, arr2, weight2, res_naive);
    t_cost = pixel_get_current_time() - t_start;
    printf("weighted_sum_naive, time cost is %.6lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    //weighted_sum_asimd(len, arr1, weight1, arr2, weight2, res_asimd);
    weighted_sum_asimd_fast(len, arr1, weight1, arr2, weight2, res_asimd);
    t_cost = pixel_get_current_time() - t_start;
    not_equal_count = check_array_equal(len, res_asimd, res_naive);
    printf("weighted_sum_asimd, not_equal_count=%d, time cost is %.6lf ms\n", not_equal_count, t_cost);

    t_start = pixel_get_current_time();
    weighted_sum_psimd(len, arr1, weight1, arr2, weight2, res_psimd);
    t_cost = pixel_get_current_time() - t_start;
    not_equal_count = check_array_equal(len, res_psimd, res_naive);
    printf("weighted_sum_psimd, not_equal_count=%d, time cost is %.6lf ms\n", not_equal_count, t_cost);

    free(arr1);
    free(arr2);
    free(res_naive);
    free(res_asimd);
    free(res_psimd);
}


int main() {
    //test_dotproduct_f32();
    //test_relu_f32();
    //test_relu_inplace_f32();
    //test_weighted_sum();

    return 0;
}