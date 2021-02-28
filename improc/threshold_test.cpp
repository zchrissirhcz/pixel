#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/opencv.hpp>
#include "threshold.h"

#include "common/pixel_benchmark.h"

int main() {

    cv::Mat image = cv::imread("sky.png");
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Size size = image.size();
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;

    unsigned char* input_gray = gray.data;
    unsigned char* output_gray = NULL;

    cv::Mat result_naive(size, CV_8UC1);
    cv::Mat result_opencv(size, CV_8UC1);
    cv::Mat result_asimd(size, CV_8UC1);
    cv::Mat result_asm(size, CV_8UC1);

    double t_start, t_cost;
    unsigned char thresh = 60;

    output_gray = result_naive.data;
    t_start = pixel_get_current_time();
    threshold_gray_naive(gray.data, height, width, output_gray, thresh, 0, 255);
    t_cost = pixel_get_current_time() - t_start;
    printf("threshold, naive, time cost is %.4lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    cv::threshold(gray, result_opencv, thresh, 255, 0);
    t_cost = pixel_get_current_time() - t_start;
    printf("threshold, opencv, time cost is %.4lf ms\n", t_cost);

    output_gray = result_asimd.data;
    t_start = pixel_get_current_time();
    threshold_gray_asimd(gray.data, height, width, output_gray, thresh, 0, 255);
    t_cost = pixel_get_current_time() - t_start;
    printf("threshold, asimd, time cost is %.4lf ms\n", t_cost);

    output_gray = result_asm.data;
    t_start = pixel_get_current_time();
    threshold_gray_asm(gray.data, height, width, output_gray, thresh, 0, 255);
    t_cost = pixel_get_current_time() - t_start;
    printf("threshold, asm, time cost is %.4lf ms\n", t_cost);

    cv::imwrite("sky_thresh60_naive.png", result_naive);
    cv::imwrite("sky_thresh60_opencv.png", result_opencv);
    cv::imwrite("sky_thresh60_asimd.png", result_asimd);
    cv::imwrite("sky_thresh60_asm.png", result_asm);

    return 0;
}