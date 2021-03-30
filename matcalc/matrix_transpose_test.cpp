#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_transpose.h"

#include <opencv2/opencv.hpp>
#include "common/pixel_benchmark.h"
#include <Eigen/Dense>

using std::cout;
using std::endl;

static void matrix_transpose_u8_opencv(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst)
{
    cv::Mat src_mat(height, width, CV_8UC1, src);
    cv::Mat dst_mat(width, height, CV_8UC1, dst);
    dst_mat = src_mat.t();
}

static void matrix_transpose_u8_eigen(unsigned char* src, uint32_t height, uint32_t width, unsigned char* dst)
{
    Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eSrc(src, height, width);
    Eigen::Map<Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eDst(dst, width, height);
    eDst = eSrc.transpose();

    // cout << "eSrc:" << endl << eSrc << endl;
    // cout << "eDst:" << endl << eDst << endl;
}

static void prepare_big(uint32_t& height, uint32_t& width, cv::Mat& gray)
{
    cv::Mat image = cv::imread("river_bank2.png");

    // 为了检查边界是否处理正确，resize到不是4的倍数的尺寸
    // cv::Size dsize;
    // dsize.height = image.rows - 1;
    // dsize.width = image.cols - 1;
    // cv::resize(image, image, dsize);

    cv::Size size = image.size();
    printf("image info: height=%d, width=%d\n", size.height, size.width);

    
    gray = cv::Mat(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    height = size.height;
    width = size.width;
}

static void prepare_small(uint32_t& height, uint32_t& width, cv::Mat& gray)
{
    height = 3;
    width = 5;
    gray = cv::Mat(height, width, CV_8UC1);
    
    unsigned char* data = gray.data;
    for (uint32_t i=0; i<height*width; i++) {
        data[i] = i%256;
    }
}

static void matrix_transpose_u8_test()
{
    uint32_t height;
    uint32_t width;
    cv::Mat gray;
    prepare_big(height, width, gray);
    //prepare_small(height, width, gray);

    unsigned char* src = gray.data;
    double t_start, t_cost;
    uint32_t len = height * width;
    unsigned char* dst_naive = (unsigned char*)malloc(sizeof(unsigned char)*len);
    unsigned char* dst_order_opt = (unsigned char*)malloc(sizeof(unsigned char)*len);
    unsigned char* dst_opencv = (unsigned char*)malloc(sizeof(unsigned char)*len);
    unsigned char* dst_eigen = (unsigned char*)malloc(sizeof(unsigned char)*len);
    unsigned char* dst_partition4x4 = (unsigned char*)malloc(sizeof(unsigned char)*len);
    unsigned char* dst_asimd = (unsigned char*)malloc(sizeof(unsigned char)*len);
    
    // naive
    t_start = pixel_get_current_time();
    matrix_transpose_u8_naive(src, height, width, dst_naive);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix transpose u8, naive,         time cost %.2lf ms\n", t_cost);

    // order_opt
    t_start = pixel_get_current_time();
    matrix_transpose_u8_order_opt(src, height, width, dst_order_opt);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix transpose u8, order_opt,     time cost %.2lf ms\n", t_cost);

    // opencv
    t_start = pixel_get_current_time();
    matrix_transpose_u8_opencv(src, height, width, dst_opencv);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix transpose u8, opencv,        time cost %.2lf ms\n", t_cost);

    // eigen
    t_start = pixel_get_current_time();
    matrix_transpose_u8_eigen(src, height, width, dst_eigen);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix transpose u8, eigen,         time cost %.2lf ms\n", t_cost);

    // partition8x8
    t_start = pixel_get_current_time();
    matrix_transpose_u8_partition8x8(src, height, width, dst_partition4x4);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix transpose u8, partition8x8,  time cost %.2lf ms\n", t_cost);

    // partition8x8, asimd
    t_start = pixel_get_current_time();
    matrix_transpose_u8_partition8x8_asimd(src, height, width, dst_asimd);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix transpose u8, partition8x8 asimd, time cost %.2lf ms\n", t_cost);

    int mis_order_opt = 0;
    int mis_opencv = 0;
    int mis_eigen = 0;
    int mis_partition4x4 = 0;
    int mis_asimd = 0;

    for (uint32_t i=0; i<len; i++) {
        if (dst_naive[i]!=dst_order_opt[i]) {
            mis_order_opt++;
        }
        if (dst_naive[i]!=dst_opencv[i]) {
            mis_opencv++;
        }
        if (dst_naive[i]!=dst_eigen[i]) {
            mis_eigen++;
        }
        if (dst_naive[i]!=dst_partition4x4[i]) {
            mis_partition4x4++;
        }
        if (dst_naive[i]!=dst_asimd[i]) {
            mis_asimd++;
        }
    }

    printf("mis_order_opt=%d, mis_opencv=%d, mis_eigen=%d, mis_partition4x4=%d, mis_asimd=%d\n",
        mis_order_opt, mis_opencv, mis_eigen, mis_partition4x4, mis_asimd);
}

static void eigen_test()
{
    using namespace Eigen;

    // Matrix3d Mat1;
    // Mat1 << 1, 2, 3,
    //     4, 6, 8,
    //     7, 9, 9;

    Eigen::Matrix <double, 3, 5, RowMajor> Mat1;
    Mat1 << 1, 2, 3, 4, 5,
            11, 12, 13, 14, 15,
            21, 22, 23, 24, 25;

    cout << "Mat1=\n" << Mat1 << endl;
    cout << "Mat1转置矩阵：\n" << Mat1.transpose() << endl;
    // cout << "Mat1伴随矩阵：\n" << Mat1.adjoint() << endl;
    // cout << "Mat1逆矩阵：\n" << Mat1.inverse() << endl;
    // cout << "Mat1行列式：\n" << Mat1.determinant() << endl;
    // SelfAdjointEigenSolver<Matrix3d>eigensover(Mat1);
    // if (eigensover.info() != Success) abort();
    // cout << "特征值：\n" << eigensover.eigenvalues() << endl;
    // cout << "特征向量：\n" << eigensover.eigenvectors() << endl;
}

static void vtrn_f32_test()
{
    float data0[4] = {0, 1, 2, 3};
    float data1[4] = {32, 33, 34, 35};
    float data2[4] = {64, 65, 66, 67};
    float data3[4] = {96, 97, 98, 99};

    printf("before transpose32x4x4:\n");
    printf("data0: %f %f %f %f\n", data0[0], data0[1], data0[2], data0[3]);
    printf("data1: %f %f %f %f\n", data1[0], data1[1], data1[2], data1[3]);
    printf("data2: %f %f %f %f\n", data2[0], data2[1], data2[2], data2[3]);
    printf("data3: %f %f %f %f\n", data3[0], data3[1], data3[2], data3[3]);

    transpose_f32_4x4(data0, data1, data2, data3);


    printf("--------------------------------------------------\n");

    printf("after transpose32x4x4:\n");
    printf("data0: %f %f %f %f\n", data0[0], data0[1], data0[2], data0[3]);
    printf("data1: %f %f %f %f\n", data1[0], data1[1], data1[2], data1[3]);
    printf("data2: %f %f %f %f\n", data2[0], data2[1], data2[2], data2[3]);
    printf("data3: %f %f %f %f\n", data3[0], data3[1], data3[2], data3[3]);
}

static void print_u8_array(unsigned char* data, uint32_t len) {
    for (size_t i=0; i<len; i++) {
        printf(" %2d", data[i]);
    }
    printf("\n");
}

static void vtrn_u8_test()
{
    unsigned char data0[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    unsigned char data1[8] = {10, 11, 12, 13, 14, 15, 16, 17};
    unsigned char data2[8] = {20, 21, 22, 23, 24, 25, 26, 27};
    unsigned char data3[8] = {30, 31, 32, 33, 34, 35, 36, 37};
    unsigned char data4[8] = {40, 41, 42, 43, 44, 45, 46, 47};
    unsigned char data5[8] = {50, 51, 52, 53, 54, 55, 56, 57};
    unsigned char data6[8] = {60, 61, 62, 63, 64, 65, 66, 67};
    unsigned char data7[8] = {70, 71, 72, 73, 74, 75, 76, 77};

    printf("before transpose_u8_8x8:\n");
    printf("data0:"); print_u8_array(data0, 8);
    printf("data1:"); print_u8_array(data1, 8);
    printf("data2:"); print_u8_array(data2, 8);
    printf("data3:"); print_u8_array(data3, 8);
    printf("data4:"); print_u8_array(data4, 8);
    printf("data5:"); print_u8_array(data5, 8);
    printf("data6:"); print_u8_array(data6, 8);
    printf("data7:"); print_u8_array(data7, 8);
    
    transpose_u8_8x8(data0, data1, data2, data3, data4, data5, data6, data7);

    printf("--------------------------------------------------\n");

    printf("after transpose_u8_8x8:\n");
    printf("data0:"); print_u8_array(data0, 8);
    printf("data1:"); print_u8_array(data1, 8);
    printf("data2:"); print_u8_array(data2, 8);
    printf("data3:"); print_u8_array(data3, 8);
    printf("data4:"); print_u8_array(data4, 8);
    printf("data5:"); print_u8_array(data5, 8);
    printf("data6:"); print_u8_array(data6, 8);
    printf("data7:"); print_u8_array(data7, 8);
}


int main() {

    matrix_transpose_u8_test();
    //eigen_test();
    //vtrn_f32_test();
    //vtrn_u8_test();

    return 0;
}