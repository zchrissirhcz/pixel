#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/opencv.hpp>

#include "common/pixel_benchmark.h"
#include "mean_stddev.h"

static void test_mean_stddev_u8()
{
    //std::string image_path = "IU.bmp";
    std::string image_path = "river_bank2.png";
    cv::Mat image = cv::imread(image_path);
    cv::Size size = image.size();
    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    size_t height = size.height;
    size_t width = size.width;
    printf("image info: name=%s, height=%zu, width=%zu\n",
        image_path.c_str(), height, width);
    cv::Mat mean_mat, stddev_mat;

    double t_start, t_cost;
    unsigned char* src_buf = gray.data;

    t_start = pixel_get_current_time();
    cv::meanStdDev(gray, mean_mat, stddev_mat);
    t_cost = pixel_get_current_time() - t_start;
    double mean=0;
    double std_dev = 0;
    printf("mean_mat.size: height=%d, width=%d\n", mean_mat.rows, mean_mat.cols);
    printf("stddev_mat.size: height=%d, width=%d\n", stddev_mat.rows, stddev_mat.cols);
    mean = mean_mat.at<double>(0, 0);
    std_dev = stddev_mat.at<double>(0, 0);
    printf("meanStdDev for u8 input, opencv, mean: %.4lf, stdDev: %.4lf, time cost %.4lf ms\n",
        mean, std_dev, t_cost);

    float mean_naive = 0;
    float stddev_naive = 0;
    size_t len = height * width;
    t_start = pixel_get_current_time();
    mean_stddev_u8_naive(src_buf, len, &mean_naive, &stddev_naive);
    t_cost = pixel_get_current_time() - t_start;
    printf("meanStdDev for u8 input, naive, mean: %.4f, stdDev: %.4f, time cost %.4lf ms\n",
        mean_naive, stddev_naive, t_cost);
}

int main(){
    test_mean_stddev_u8();

    return 0;
}