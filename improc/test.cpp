#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common/pixel_benchmark.h"

#include <opencv2/opencv.hpp>

void rgb2gray_mean(const unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf);
void rgb2gray_weighted(const unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf);

void rgb2gray_mean(const unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    size_t total_len = height * width;
    for (size_t i=0; i<total_len; i++) {
        *gray_buf = (rgb_buf[0] + rgb_buf[1] + rgb_buf[2]);
        rgb_buf += 3;
        gray_buf ++;
    }
}

void rgb2gray_weighted(const unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    size_t gray_len = height * width;
    size_t rgb_len = gray_len * 3;

    size_t i=0; // rgb_idx
    size_t j=0; // gray_idx
    for( ; j<gray_len; j++, i+=3) {
        float r = rgb_buf[i];
        float g = rgb_buf[i+1];
        float b = rgb_buf[i+2];
        float gray = (0.299*r + 0.587*g + 0.114*b);
        gray_buf[j] = gray;
    }
}

int main() {
    cv::Mat image = cv::imread("sky.png");
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    cv::Size size = image.size();
    cv::Mat gray_mean(size, CV_8UC1);
    cv::Mat gray_weighted(size, CV_8UC1);
    cv::Mat gray_opencv(size, CV_8UC1);

    unsigned char* rgb_buf = image.data;
    unsigned char* gray_buf = NULL;
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;

    double t_start, t_cost;

    gray_buf = gray_mean.data;
    t_start = pixel_get_current_time();
    rgb2gray_mean(rgb_buf, height, width, gray_buf);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2gray, mean, time cost is %.4lf ms\n", t_cost);

    gray_buf = gray_weighted.data;
    t_start = pixel_get_current_time();
    rgb2gray_weighted(rgb_buf, height, width, gray_buf);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2gray, weighted, time cost is %.4lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    cv::cvtColor(image, gray_opencv, cv::COLOR_BGR2GRAY);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2gray, opencv, time cost is %.4lf ms\n", t_cost);

    return 0;
}