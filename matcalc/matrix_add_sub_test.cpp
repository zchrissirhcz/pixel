#include "matrix_add_sub.h"
#include "matrix.h"
#include "common/pixel_benchmark.h"

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>

static void matrix_add_f32_opencv(float* mA, float* mB, float* mC, size_t M, size_t N)
{
    cv::Size size;
    size.height = M;
    size.width = N;
    cv::Mat matA = cv::Mat(size, CV_32FC1, mA);
    cv::Mat matB = cv::Mat(size, CV_32FC1, mB);
    cv::Mat matC = cv::Mat(size, CV_32FC1, mC);
    cv::add(matA, matB, matC);
}

static void matrix_add_f32_eigen(float* mA, float* mB, float* mC, size_t M, size_t N)
{
    size_t len = M * N;
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> eA(mA, len);
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> eB(mB, len);
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> eC(mA, len);
    eC = eA + eB;
}

static void matrix_add_float_test()
{
    cv::Mat image = cv::imread("river_bank2.png");
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    cv::Mat b_channels = channels[0];
    cv::Mat g_channels = channels[1];
    cv::Mat r_channels = channels[2];

    b_channels.convertTo(b_channels, CV_32FC1);
    g_channels.convertTo(g_channels, CV_32FC1);
    r_channels.convertTo(r_channels, CV_32FC1);

    cv::Size size = image.size();
    size_t height = size.height; // M
    size_t width = size.width; // N
    printf("image info: height=%zu, width=%zu\n", height, width);

    float* mA = (float*)b_channels.data;
    float* mB = (float*)g_channels.data;

    size_t buf_size = sizeof(float)*height*width;
    float* mC_opencv = (float*)malloc(buf_size);
    float* mC_eigen = (float*)malloc(buf_size);
    float* mC_naive = (float*)malloc(buf_size);
    float* mC_asimd = (float*)malloc(buf_size);
    double t_start, t_cost;

    t_start = pixel_get_current_time();
    matrix_add_f32_opencv(mA, mB, mC_opencv, height, width);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix_add_f32, opencv, time cost %.2lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    matrix_add_f32_eigen(mA, mB, mC_eigen, height, width);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix_add_f32, eigen, time cost %.2lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    matrix_add_f32_naive(mA, mB, mC_naive, height, width);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix_add_f32, naive,  time cost %.2lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    matrix_add_f32_asimd(mA, mB, mC_asimd, height, width);
    t_cost = pixel_get_current_time() - t_start;
    printf("matrix_add_f32, asimd,  time cost %.2lf ms\n", t_cost);

    // validate result, check if they are match
    int mis1 = 0;
    int mis2 = 0;
    int mis3 = 0;
    size_t len = height * width;
    for (size_t i=0; i<len; i++) {
        if (mC_naive[i]!=mC_opencv[i]) {
            mis1 ++;
        }

        if (mC_naive[i]!=mC_eigen[i]) {
            mis2 ++;
        }

        if (mC_naive[i]!=mC_asimd[i]) {
            mis3 ++;
        }
    }
    printf("mis1=%d, mis2=%d, mis3=%d\n", mis1, mis2, mis3);

}

int main() {

    matrix_add_float_test();

    return 0;
}