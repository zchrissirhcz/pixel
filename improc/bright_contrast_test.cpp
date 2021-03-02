#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>

#include "common/pixel_benchmark.h"
#include "bright_contrast.h"

int main() {

    cv::Mat image = cv::imread("sky.png");
    //cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    cv::Size size = image.size();
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;

    int bright = 20;
    int contrast = 30;
    unsigned char* src_rgb = NULL;
    unsigned char* dst_rgb = NULL;

    cv::Mat result_naive(size, CV_8UC3);
    
    double t_start, t_cost;

    src_rgb = image.data;
    dst_rgb = result_naive.data;
    t_start = pixel_get_current_time();
    bright_contrast(src_rgb, height, width, bright, contrast, dst_rgb);
    t_cost = pixel_get_current_time() - t_start;
    printf("bright-contrast, naive, time cost is %.4lf ms\n", t_cost);

    cv::imwrite("sky_bc20-30.png", result_naive);

    return 0;
}