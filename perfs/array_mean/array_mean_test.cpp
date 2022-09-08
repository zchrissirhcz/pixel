#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "px_timer.h"
#include "array_mean.h"

#include <opencv2/opencv.hpp>

void test_array_mean()
{
    //cv::Mat image = cv::imread("sky.jpg");
    cv::Mat image = cv::imread("river_bank2.jpg");
    cv::Size size = image.size();
    int height = size.height;
    int width = size.width;

    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    size_t len = height * width;

    float mean = 0;
    double t_start, t_cost;
    unsigned char* data = gray.data;

    t_start = px_get_current_time();
    mean = cv::mean(gray).val[0];
    t_cost = px_get_current_time() - t_start;
    printf("array mean, opencv, result=%.4f, time cost %.4lf\n", mean, t_cost);

    t_start = px_get_current_time();
    mean = array_mean_u8_naive(data, len);
    t_cost = px_get_current_time() - t_start;
    printf("array mean, naive,  result=%.4f, time cost %.4lf\n", mean, t_cost);

    t_start = px_get_current_time();
    mean = array_mean_u8_asimd1(data, len);
    t_cost = px_get_current_time() - t_start;
    printf("array mean, asimd1, result=%.4f, time cost %.4lf\n", mean, t_cost);

    t_start = px_get_current_time();
    mean = array_mean_u8_asimd2(data, len);
    t_cost = px_get_current_time() - t_start;
    printf("array mean, asimd2, result=%.4f, time cost %.4lf\n", mean, t_cost);

    t_start = px_get_current_time();
    mean = array_mean_u8_asimd3(data, len);
    t_cost = px_get_current_time() - t_start;
    printf("array mean, asimd3, result=%.4f, time cost %.4lf\n", mean, t_cost);

    t_start = px_get_current_time();
    mean = array_mean_u8_asimd4(data, len);
    t_cost = px_get_current_time() - t_start;
    printf("array mean, asimd4, result=%.4f, time cost %.4lf\n", mean, t_cost);

    t_start = px_get_current_time();
    mean = array_mean_u8_asimd5(data, len);
    t_cost = px_get_current_time() - t_start;
    printf("array mean, asimd5, result=%.4f, time cost %.4lf\n", mean, t_cost);

    t_start = px_get_current_time();
    mean = array_mean_u8_asimd6(data, len);
    t_cost = px_get_current_time() - t_start;
    printf("array mean, asimd6, result=%.4f, time cost %.4lf\n", mean, t_cost);

    t_start = px_get_current_time();
    mean = array_mean_u8_asimd7(data, len);
    t_cost = px_get_current_time() - t_start;
    printf("array mean, asimd7, result=%.4f, time cost %.4lf\n", mean, t_cost);
}

int main() {
    test_array_mean();

    return 0;
}