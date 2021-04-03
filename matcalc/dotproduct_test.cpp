#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dotproduct.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_fast_rng.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/simd_intrinsics.hpp>

#include <Eigen/Dense>

#include <iostream>
using namespace std;

static void dotproduct_f32_test();
static float dotproduct_f32_opencv1(float* a, float* b, uint32_t len);
static float dotproduct_f32_opencv2(float* a, float* b, uint32_t len);
static float dotproduct_f32_eigen(float* a, float* b, uint32_t len);

static void dotproduct_u8_test();
static uint64_t dotproduct_u8_opencv1(uint8_t* a, uint8_t* b, uint32_t len);
static uint64_t dotproduct_u8_opencv2(uint8_t* a, uint8_t* b, uint32_t len);
static uint64_t dotproduct_u8_eigen(uint8_t* a, uint8_t* b, uint32_t len);
static uint64_t dotproduct_u8_eigen2(uint8_t* a, uint8_t* b, uint32_t len);

static void arm_marco_determine();

//----------------------------------------------------------------------

float dotproduct_f32_opencv1(float* a, float* b, uint32_t len)
{
    cv::Mat matA(1, len, CV_32FC1, a);
    cv::Mat matB(1, len, CV_32FC1, b);
    float res = matA.dot(matB);
    return res;
}

float dotproduct_f32_opencv2(float* a, float* b, uint32_t len)
{
    size_t step = sizeof(cv::v_float32)/sizeof(float);
    cv::v_float32 v_sum = cv::vx_setzero_f32();
    size_t vec_size = len - len % step;
    for (size_t i=0; i< vec_size; i+=step)
    {
        cv::v_float32 v1 = cv::vx_load(a+i);
        cv::v_float32 v2 = cv::vx_load(b+i);
        v_sum += v1 * v2;
    }

    float sum = cv::v_reduce_sum(v_sum);

    for (size_t i= vec_size; i<len; i++) {
        sum += a[i] * b[i];
    }

    return sum;
}

float dotproduct_f32_eigen(float* a, float* b, uint32_t len) {
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> va(a, len);
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> vb(b, len);
    float res = va.dot(vb);
    return res;
}



uint64_t dotproduct_u8_opencv1(uint8_t* a, uint8_t* b, uint32_t len)
{
    cv::Mat matA(1, len, CV_8UC1, a);
    cv::Mat matB(1, len, CV_8UC1, b);
    uint64_t res = matA.dot(matB);
    return res;
}

uint64_t dotproduct_u8_opencv2(uint8_t* a, uint8_t* b, uint32_t len)
{
    // 实现起来可能略繁琐，并且和neon实现类似，暂时不写
    return 0;
}

uint64_t dotproduct_u8_eigen(uint8_t* a, uint8_t* b, uint32_t len)
{
    // 这里的实现有问题：如果是u8的Eigen::Matrix做dotproduct，结果不对
    // 如果转为u64或float的Eigen::Matrix，结果对，但是比naive实现慢好多倍
    Eigen::Map<Eigen::Matrix<uint8_t, 1, Eigen::Dynamic, Eigen::RowMajor>> va(a, len);
    Eigen::Map<Eigen::Matrix<uint8_t, 1, Eigen::Dynamic, Eigen::RowMajor>> vb(b, len);
    
    Eigen::Matrix<uint64_t, 1, Eigen::Dynamic, Eigen::RowMajor> fa = va.cast<uint64_t>();
    Eigen::Matrix<uint64_t, 1, Eigen::Dynamic, Eigen::RowMajor> fb = vb.cast<uint64_t>();

    return fa.dot(fb);
}

uint64_t dotproduct_u8_eigen2(uint8_t* a, uint8_t* b, uint32_t len)
{
    // 这里的实现有问题：如果是u8的Eigen::Matrix做dotproduct，结果不对
    // 如果转为u64或float的Eigen::Matrix，结果对，但是比naive实现慢好多倍
    Eigen::Map<Eigen::Matrix<uint8_t, 1, Eigen::Dynamic, Eigen::RowMajor>> va(a, len);
    Eigen::Map<Eigen::Matrix<uint8_t, 1, Eigen::Dynamic, Eigen::RowMajor>> vb(b, len);
    uint64_t res = va.cast<uint64_t>().dot(vb.cast<uint64_t>());
    return res;
}

void dotproduct_f32_test()
{
    uint32_t len = 200000000; //200M
    float* a = (float*)malloc(sizeof(float)*len);
    float* b = (float*)malloc(sizeof(float)*len);

    float res_naive;
    float res_opencv1;
    float res_opencv2;
    float res_eigen;
    float res_asimd;
    float res_asimd2;
    float res_asimd3;

    double t_start, t_cost;

    g_state.a = 7767517;
    t_start = pixel_get_current_time();
    for(uint32_t i=0; i<len; i++) {
        a[i] = pixel_fast_random_float(-1.2, 1.2);
        b[i] = pixel_fast_random_float(-1.2, 1.2);
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("generate random numbers, time cost %.4lf ms\n", t_cost);

    // naive
    t_start = pixel_get_current_time();
    res_naive = dotproduct_f32_naive(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_f32, naive, time cost %.4lf ms\n", t_cost);

    // opencv, method1
    t_start = pixel_get_current_time();
    res_opencv1 = dotproduct_f32_opencv1(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_f32, opencv method1, time cost %.4lf ms\n", t_cost);

    // opencv, method2
    t_start = pixel_get_current_time();
    res_opencv2 = dotproduct_f32_opencv2(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_f32, opencv method2, time cost %.4lf ms\n", t_cost);

    // eigen
    t_start = pixel_get_current_time();
    res_eigen = dotproduct_f32_eigen(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_f32, eigen, time cost %.4lf ms\n", t_cost);

    // asimd
    t_start = pixel_get_current_time();
    res_asimd = dotproduct_f32_asimd(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_f32, asimd, time cost %.4lf ms\n", t_cost);

    // asimd2
    t_start = pixel_get_current_time();
    res_asimd2 = dotproduct_f32_asimd2(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_f32, asimd2, time cost %.4lf ms\n", t_cost);

    // asimd3
    t_start = pixel_get_current_time();
    res_asimd3 = dotproduct_f32_asimd3(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_f32, asimd3, time cost %.4lf ms\n", t_cost);

    // validate
    int mis_opencv1=0;
    int mis_opencv2=0;
    int mis_eigen=0;
    int mis_asimd=0;
    int mis_asimd2=0;
    if (res_naive!=res_opencv1) {
        mis_opencv1++;
    }
    if (res_naive!=res_opencv2) {
        mis_opencv2++;
    }
    if (res_naive!=res_eigen) {
        mis_eigen++;
    }
    if (res_naive!=res_asimd) {
        mis_asimd++;
    }
    if (res_naive!=res_asimd2) {
        mis_asimd2++;
    }
    printf("mis_opencv1=%d, mis_opencv2=%d, mis_eigen=%d, mis_asimd=%d, mis_asimd2=%d\n", 
        mis_opencv1, mis_opencv2, mis_eigen, mis_asimd, mis_asimd2);
    printf("res_naive=%f, res_opencv1=%f, res_opencv2=%f, res_eigen=%f, res_asimd=%f, res_asimd2=%f, res_asimd3=%f\n",
        res_naive, res_opencv1, res_opencv2, res_eigen, res_asimd, res_asimd2, res_asimd3
    );
}

void dotproduct_u8_test()
{
    uint32_t len = 200000000; //200M
    uint8_t* a = (uint8_t*)malloc(sizeof(uint8_t)*len);
    uint8_t* b = (uint8_t*)malloc(sizeof(uint8_t)*len);

    uint64_t res_naive;
    uint64_t res_opencv1;
    uint64_t res_opencv2;
    uint64_t res_eigen;
    uint64_t res_asimd;
    uint64_t res_asimd2;
    uint64_t res_asimd3;
    uint64_t res_asimd4;

    double t_start, t_cost;

    g_state.a = 7767517;
    t_start = pixel_get_current_time();
    for(uint32_t i=0; i<len; i++) {
        a[i] = (uint8_t)(pixel_fast_random_float(0, 255));
        b[i] = (uint8_t)(pixel_fast_random_float(0, 255));
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("generate random numbers, time cost %.4lf ms\n", t_cost);

    // naive
    t_start = pixel_get_current_time();
    res_naive = dotproduct_u8_naive(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, naive, time cost %.4lf ms\n", t_cost);

    // opencv, method1
    t_start = pixel_get_current_time();
    res_opencv1 = dotproduct_u8_opencv1(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, opencv method1, time cost %.4lf ms\n", t_cost);

    // opencv, method2
    t_start = pixel_get_current_time();
    res_opencv2 = dotproduct_u8_opencv2(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, opencv method2, time cost %.4lf ms\n", t_cost);

    // eigen
    t_start = pixel_get_current_time();
    res_eigen = dotproduct_u8_eigen(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, eigen, time cost %.4lf ms\n", t_cost);

    // eigen2
    t_start = pixel_get_current_time();
    res_eigen = dotproduct_u8_eigen2(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, eigen2, time cost %.4lf ms\n", t_cost);

    // asimd
    t_start = pixel_get_current_time();
    res_asimd = dotproduct_u8_asimd(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, asimd, time cost %.4lf ms\n", t_cost);

    // asimd2
    t_start = pixel_get_current_time();
    res_asimd2 = dotproduct_u8_asimd2(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, asimd2, time cost %.4lf ms\n", t_cost);

    // asimd3
    t_start = pixel_get_current_time();
    res_asimd3 = dotproduct_u8_asimd3(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, asimd3, time cost %.4lf ms\n", t_cost);

    // asimd4
    t_start = pixel_get_current_time();
    res_asimd4 = dotproduct_u8_asimd4(a, b, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("dotproduct_u8, asimd4, time cost %.4lf ms\n", t_cost);

    // validate
    int mis_opencv1=0;
    int mis_opencv2=0;
    int mis_eigen=0;
    int mis_asimd=0;
    int mis_asimd2=0;
    int mis_asimd3=0;
    int mis_asimd4=0;
    if (res_naive!=res_opencv1) {
        mis_opencv1++;
    }
    if (res_naive!=res_opencv2) {
        mis_opencv2++;
    }
    if (res_naive!=res_eigen) {
        mis_eigen++;
    }
    if (res_naive!=res_asimd) {
        mis_asimd++;
    }
    if (res_naive!=res_asimd2) {
        mis_asimd2++;
    }
    if (res_naive!=res_asimd3) {
        mis_asimd3++;
    }
    if (res_naive!=res_asimd4) {
        mis_asimd4++;
    }
    printf("mis_opencv1=%d, mis_opencv2=%d, mis_eigen=%d, mis_asimd=%d, mis_asimd2=%d, mis_asimd3=%d, mis_asimd4=%d\n", 
        mis_opencv1, mis_opencv2, mis_eigen, mis_asimd, mis_asimd2, mis_asimd3, mis_asimd4);
    // printf("res_naive=%ul, res_opencv1=%ul, res_opencv2=%ul, res_eigen=%ul, res_asimd=%ul, res_asimd2=%ul, res_asimd3=%ul\n",
    //     res_naive, res_opencv1, res_opencv2, res_eigen, res_asimd, res_asimd2, res_asimd3
    // );
    cout << "res_naive=" << res_naive << endl;
    cout << "res_opencv1=" << res_opencv1 << endl;
    cout << "res_eigen=" << res_eigen << endl;
    cout << "res_asimd=" << res_asimd << endl;
    cout << "res_asimd2=" << res_asimd2 << endl;
    cout << "res_asimd3=" << res_asimd3 << endl;
    cout << "res_asimd4=" << res_asimd4 << endl;
}

void arm_marco_determine()
{
#if __ARM_NEON
    printf("__ARM_NEON\n");
    #if __aarch64__
        printf("__aarch64__\n");
        #if __ARM_FEATURE_FP16_VECTOR_ARITHMETIC
            printf("arm8.2 fp16\n");
        #else
            printf("not arm8.2 fp16\n");
        #endif

        #if __ARM_FEATURE_DOTPROD
            printf("arm8.2 dotproduct\n");
        #else
            printf("not arm8.2 dotproduct\n");
        #endif
    #else
        printf("not __aarch64__\n");
    #endif // __aarch64__
#else
    printf("not __ARM_NEON\n");
#endif // __ARM_NEON
}


static void sdot_example() {
#if __ARM_FEATURE_DOTPROD
    //uint32x2_t vdot_u32 (uint32x2_t r, uint8x8_t a, uint8x8_t b)
    uint8_t a[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8_t b[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    uint8x8_t va = vld1_u8(a);
    uint8x8_t vb = vld1_u8(b);
    uint32x2_t vr = vdup_n_u32(0);
    vr = vdot_u32(vr, va, vb);

    uint32_t res[2];
    vst1_u32(res, vr);
    printf("res[0]=%d, res[1]=%d\n", res[0], res[1]);
#endif
}


int main() {
    arm_marco_determine();
    //dotproduct_f32_test();
    dotproduct_u8_test();

    return 0;
}