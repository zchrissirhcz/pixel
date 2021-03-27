#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_gemm.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_check.h"

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

using std::cout;
using std::endl;

typedef struct Matrix {
    size_t rows;
    size_t cols;
    float* data;
} Matrix;

/*
1 2     1  2    7  10
3 4     3  4    15 22
*/

static void print_matrix(float* data, uint32_t rows, uint32_t cols) {
    size_t idx=0;
    for (uint32_t i=0; i<rows; i++){
        for (uint32_t j=0; j<cols; j++) {
            printf("%.2f, ", data[idx]);
            idx++;
        }
        printf("\n");
    }
}

static void matrix_gemm_f32_opencv(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N)
{
    cv::Mat matA = cv::Mat(M, K, CV_32FC1, mA);
    cv::Mat matB = cv::Mat(K, N, CV_32FC1, mB);
    cv::Mat matC = cv::Mat(M, N, CV_32FC1, mC);
    matC = matA * matB;
    // cout << "matA:\n" << matA << endl;
    // cout << "matB:\n" << matB << endl;
    // cout << "matC:\n" << matC << endl;
}

static void matrix_gemm_f32_eigen(float* mA, float* mB, float* mC, const uint32_t M, const uint32_t K, const uint32_t N)
{
    Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eA(mA, M, K);
    Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eB(mB, K, N);
    Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eC(mC, M, N);
    eC = eA * eB;
    // cout << "eA:\n" << eA << endl;
    // cout << "eB:\n" << eB << endl;
    // cout << "eC:\n" << eC << endl;
}

static void gemm_tiny_debug()
{
    uint32_t m = 3;
    uint32_t k = 2;
    uint32_t n = 4;

    Matrix mA;
    mA.rows = m;
    mA.cols = k;
    
    Matrix mB;
    mB.rows = k;
    mB.cols = n;

    mA.data = (float*)malloc(mA.rows*mA.cols * sizeof(float));
    mB.data = (float*)malloc(mB.rows*mB.cols * sizeof(float));

    mA.data[0] = 2;
    mA.data[1] = -6;
    mA.data[2] = 3;
    mA.data[3] = 5;
    mA.data[4] = 1;
    mA.data[5] = -1;

    mB.data[0] = 4;
    mB.data[1] = -2;
    mB.data[2] = -4;
    mB.data[3] = -5;
    mB.data[4] = -7;
    mB.data[5] = -3;
    mB.data[6] = 6;
    mB.data[7] = 7;

    Matrix mC;
    mC.rows = m;
    mC.cols = n;
    mC.data = (float*)malloc(mC.rows*mC.cols * sizeof(float));

    matrix_gemm_f32_order_opt(mA.data, mB.data, mC.data, m, k, n);

    printf("--- matrix A:\n");
    print_matrix(mA.data, mA.rows, mA.cols);

    printf("--- matrix B:\n");
    print_matrix(mB.data, mB.rows, mB.cols);

    printf("--- matrix C:\n");
    print_matrix(mC.data, mC.rows, mC.cols);

}

static void prepare_big(uint32_t& M, uint32_t& K, uint32_t& N, cv::Mat& matA, cv::Mat& matB)
{
    cv::Mat image = cv::imread("colorhouse.png");

    cv::Size size = image.size();
    uint32_t height = size.height;
    uint32_t width = size.width;
    printf("image info: height=%u, width=%u\n", height, width);

    cv::Size transposed_size;
    transposed_size.height = size.width;
    transposed_size.width = size.height;

    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    cv::Mat b_channels = channels[0];
    cv::Mat g_channels = channels[1];
    cv::Mat r_channels = channels[2];
    
    // matA's dim: height * width
    b_channels.convertTo(matA, CV_32FC1);

    matB = b_channels.t(); // matB's dim: width * height
    matB.convertTo(matB, CV_32FC1);

    M = height;
    K = width;
    N = height;
}

static void prepare_small(uint32_t& M, uint32_t& K, uint32_t& N, cv::Mat& matA, cv::Mat& matB)
{
    M = 2;
    K = 3;
    N = 2;

    /*
    0  1  2
    1  2  3
    */
    float* data = NULL;
    matA = cv::Mat(M, K, CV_32FC1);
    data = (float*)matA.data;
    data[0] = 1.2;
    data[1] = 2.3;
    data[2] = 3.4;
    data[3] = 4.5;
    data[4] = 5.6;
    data[5] = 6.7;

    cout << "matA:" << endl << matA << endl;

    /*
    1 2
    3 4
    5 6
    */
    matB = cv::Mat(K, N, CV_32FC1);
    data = (float*)matB.data;
    data[0] = 1.1;
    data[1] = 2.2;
    data[2] = 3.3;
    data[3] = 4.5;
    data[4] = 5.5;
    data[5] = 6.6;

    cout << "matB:" << endl << matB << endl;
}

static void gemm_f32_test() {
    //--------------------------------
    cv::Mat matA;
    cv::Mat matB;
    uint32_t M, K, N;
    prepare_big(M, K, N, matA, matB);
    //prepare_small(M, K, N, matA, matB);

    printf("-- after prepare_small:\n");
    // cout << "matA:" << endl << matA << endl;
    // cout << "matB:" << endl << matB << endl;

    float* mA = (float*)matA.data;
    float* mB = (float*)matB.data;

    size_t buf_size = M*N * sizeof(float);
    float* mC_naive = (float*)malloc(buf_size);
    float* mC_eigen = (float*)malloc(buf_size);
    float* mC_opencv = (float*)malloc(buf_size);
    float* mC_order_opt = (float*)malloc(buf_size);
    float* mC_order_opt2 = (float*)malloc(buf_size);
    float* mC_asimd = (float*)malloc(buf_size);
    for(uint32_t i=0; i<M*N; i++) {
        mC_naive[i] = 0;
        mC_eigen[i] = 0;
        mC_opencv[i] = 0;
        mC_order_opt[i] = 0;
        mC_order_opt2[i] = 0;
        mC_asimd[i] = 0;
    }

    double t_start, t_cost;

    // eigen
    t_start = pixel_get_current_time();
    matrix_gemm_f32_eigen(mA, mB, mC_eigen, M, K, N);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix gemm, eigen,     time cost %.2lf ms\n", t_cost);

    // opencv
    t_start = pixel_get_current_time();
    matrix_gemm_f32_opencv(mA, mB, mC_opencv, M, K, N);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix gemm, opencv,    time cost %.2lf ms\n", t_cost);

    // naive
    t_start = pixel_get_current_time();
    matrix_gemm_f32_naive(mA, mB, mC_naive, M, K, N);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix gemm, naive,     time cost %.2lf ms\n", t_cost);

    // order opt 调整维度顺序
    t_start = pixel_get_current_time();
    matrix_gemm_f32_order_opt(mA, mB, mC_order_opt, M, K, N);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix gemm, order opt, time cost %.2lf ms\n", t_cost);

    // order opt2 调整维度顺序2
    t_start = pixel_get_current_time();
    matrix_gemm_f32_order_opt2(mA, mB, mC_order_opt2, M, K, N);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix gemm, order opt2,time cost %.2lf ms\n", t_cost);

    // asimd
    t_start = pixel_get_current_time();
    matrix_gemm_f32_asimd(mA, mB, mC_asimd, M, K, N);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix gemm, asimd,     time cost %.2lf ms\n", t_cost);

    // validate if different implementations result match
    int mis_eigen = 0;
    int mis_opencv = 0;
    int mis_order_opt = 0;
    int mis_order_opt2 = 0;
    int mis_asimd = 0;
    uint32_t len = M * N;

    //float* mC_gt = mC_order_opt;
    float* mC_gt = mC_naive;
    float epsilon = 1e-3;
    for (uint32_t i=0; i<len; i++) {
        if (!nearly_equal_absolutely(mC_gt[i], mC_eigen[i], epsilon)) {
            mis_eigen++;
        }
        if (!nearly_equal_absolutely(mC_gt[i], mC_opencv[i], epsilon)) {
            mis_opencv++;
        }
        if (!nearly_equal_absolutely(mC_gt[i], mC_order_opt[i], epsilon)) {
            mis_order_opt++;
        }
        if (!nearly_equal_absolutely(mC_gt[i], mC_order_opt2[i], epsilon)) {
            mis_order_opt2++;
        }
        if (!nearly_equal_absolutely(mC_gt[i], mC_asimd[i], epsilon)) {
            mis_asimd++;
        }
    }

    printf("mis_eigen=%d, mis_opencv=%d, mis_order_opt=%d, mis_order_opt2=%d, mis_asimd=%d\n",
        mis_eigen, mis_opencv, mis_order_opt, mis_order_opt2, mis_asimd);

    FILE* fout = NULL;

    fout = fopen("opencv.txt", "w");
    for (uint32_t i=0; i<len; i++) {
        fprintf(fout, "%f\n", mC_opencv[i]);
    }
    fclose(fout);

    fout = fopen("eigen.txt", "w");
    for (uint32_t i=0; i<len; i++) {
        fprintf(fout, "%f\n", mC_eigen[i]);
    }
    fclose(fout);

    fout = fopen("order_opt.txt", "w");
    for (uint32_t i=0; i<len; i++) {
        fprintf(fout, "%f\n", mC_order_opt[i]);
    }
    fclose(fout);

    fout = fopen("naive.txt", "w");
    for (uint32_t i=0; i<len; i++) {
        fprintf(fout, "%f\n", mC_naive[i]);
    }
    fclose(fout);
}


int main() {
    //gemm_tiny_debug();
    gemm_f32_test();

    return 0;
}