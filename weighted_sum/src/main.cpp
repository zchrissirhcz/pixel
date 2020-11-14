#include <iostream>
#include <chrono>
#include <arm_neon.h>

using namespace std;

void arr_weighted_sum(const float* arr1, const float arr1_weight, const float* arr2, const float arr2_weight,
    int len, float* result_arr)
{
    for (int i=0; i<len; i++) {
        result_arr[i] = arr1[i]*arr1_weight + arr2[i]*arr2_weight;
    }
}

void arr_weighted_sum_neon_intrinsic(const float* arr1, const float arr1_weight, const float* arr2, const float arr2_weight,
    int len, float* result_arr)
{
    int neon_len = len >> 2;  // len/4
    //int remain = len - (neon_len<<2); // len%4, i.e. len-(len/4)*4
    //int remain = len - (neon_len/2*2)*4;
    int remain = len - ((neon_len/4)*4)*4;

    // 每次处理4个元素，因此权值要拷贝4份到一个float32x4_t类型的变量中
    float32x4_t arr1_w4 = vdupq_n_f32(arr1_weight);
    float32x4_t arr2_w4 = vdupq_n_f32(arr2_weight);
    // unroll with 4
    asm volatile("prfm pldl1keep, [%0, #256]" : :"r"(arr1) :);
    asm volatile("prfm pldl1keep, [%0, #256]" : :"r"(arr2) :);
    for (int i=0; i+3<neon_len; i+=4) {
        // 1
        float32x4_t v11 = vld1q_f32(arr1);
        float32x4_t v12 = vld1q_f32(arr2);
        v11 = vmulq_f32(v11, arr1_w4);
        v12 = vmulq_f32(v12, arr2_w4);
        float32x4_t res1 = vaddq_f32(v11, v12);
        vst1q_f32(result_arr, res1);

        // 2
        float32x4_t v21 = vld1q_f32(arr1+4);
        float32x4_t v22 = vld1q_f32(arr2+4);
        v21 = vmulq_f32(v21, arr1_w4);
        v22 = vmulq_f32(v22, arr2_w4);
        float32x4_t res2 = vaddq_f32(v21, v22);
        vst1q_f32(result_arr+4, res2);

        // 3
        float32x4_t v31 = vld1q_f32(arr1+8);
        float32x4_t v32 = vld1q_f32(arr2+8);
        v31 = vmulq_f32(v31, arr1_w4);
        v32 = vmulq_f32(v32, arr2_w4);
        float32x4_t res3 = vaddq_f32(v31, v32);
        vst1q_f32(result_arr+8, res3);

        // 4
        float32x4_t v41 = vld1q_f32(arr1+12);
        float32x4_t v42 = vld1q_f32(arr2+12);
        v41 = vmulq_f32(v41, arr1_w4);
        v42 = vmulq_f32(v42, arr2_w4);
        float32x4_t res4 = vaddq_f32(v41, v42);
        vst1q_f32(result_arr+12, res4);

        // --
        arr1 += 16;
        arr2 += 16;
        result_arr += 16;
    }

    // remain parts
    for(; remain>0; remain--) {
        *result_arr = (*arr1)*arr1_weight + (*arr2)*arr2_weight;
        result_arr++;
        arr1++;
        arr2++;
    }
}

int main() {
    //test_vdup();
    //test_vld();
    //test_add();
    const int len = 11484;
    float arr1[len];
    for (int i=0; i<len; i++) {
        arr1[i] = 2&i;
    }

    float arr2[len];
    for (int i=0; i<len; i++) {
        arr2[i] = 5&i;
    }

    float res1[len] = {0};
    float res2[len] = {0};

    double t_start, t_c, t_neon;
    int count = 1000;

    {
        t_start = now_ms();
        for (int i=0; i<count; i++) {
            arr_weighted_sum(arr1, 0.5, arr2, 0.2, len, res1);
        }
        t_c = now_ms() - t_start;
    }

    {
        t_start = now_ms();
        for (int i=0; i<count; i++) {
            arr_weighted_sum_neon_intrinsic(arr1, 0.5, arr2, 0.2, len, res2);
        }
        t_neon = now_ms() - t_start;
    }

    int fail_cnt = 0;
    for (int i=0; i<len; i++) {
        if (res1[i]!=res2[i]) {
            fail_cnt++;
        }
    }
    if (fail_cnt>0) { 
        printf("fail count is %d\n", fail_cnt);
    } else {
        printf("result match\n");
    }
    printf("c impl cost %g ms\n", t_c);
    printf("neon impl cost %g ms\n", t_neon);



    return 0;
}
