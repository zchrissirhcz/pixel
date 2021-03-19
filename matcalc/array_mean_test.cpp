#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common/pixel_benchmark.h"

void test_array_mean() {
    cv::Mat image = cv::imread("sky.jpg");
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    cv::Mat Ir = channels[0];
    cv::Mat Ig = channels[1];
    cv::Mat Ib = channels[2];

    cv::Size size = image.size();
    int height = size.height;
    int width = size.width;
    size_t len = height * width;

    float sum = 0;
    double t_start, t_cost;


    unsigned char* data = Ir.data;

    t_start = pixel_get_current_time();
    sum = array_mean_naive(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, naive impl, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_asimd(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_asimd2(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl2, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_asimd3(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl3, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_asimd4(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl4, result=%.4f, time cost %.4lf\n", sum, t_cost);
}

int main() {
    test_array_mean();

    return 0;
}