#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common/pixel_benchmark.h"
#include "array_mean.h"

#include <opencv2/opencv.hpp>

void test_array_mean() {
    //cv::Mat image = cv::imread("sky.jpg");
    cv::Mat image = cv::imread("river_bank2.png");
    cv::Size size = image.size();
    int height = size.height;
    int width = size.width;

    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    size_t len = height * width;

    float sum = 0;
    double t_start, t_cost;

    unsigned char* data = gray.data;

    t_start = pixel_get_current_time();
    sum = array_mean_u8_naive(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, naive impl, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_u8_asimd(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_u8_asimd2(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl2, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_u8_asimd3(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl3, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_u8_asimd4(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl4, result=%.4f, time cost %.4lf\n", sum, t_cost);
}

int main() {
    test_array_mean();

    return 0;
}