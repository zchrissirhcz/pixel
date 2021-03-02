#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

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

void rgb2gray_ldp(const unsigned char *rgbImage,
              const int            height,
              const int            width,
              unsigned char       *grayImage) {
    int size = height * width;
    const unsigned char *src = rgbImage;
    
    uint16_t rW = 77;
    uint16_t gW = 151;
    uint16_t bW = 28;

    for (int i = 0; i < size; ++i ) {
        uint16_t gray = (static_cast<uint16_t>(src[0]) * rW) + 
                        (static_cast<uint16_t>(src[1]) * gW) + 
                        (static_cast<uint16_t>(src[2]) * bW);

        // undo the scale by 256 and write to memory
        gray = gray >> 8;
        gray = std::max(std::min(gray, (uint16_t)255), (uint16_t)0);
        grayImage[i] = static_cast<unsigned char>(gray);
        src += 3;
    }
}

void rgb2gray_weighted(const unsigned char* rgb_buf, size_t height, size_t width, unsigned char* gray_buf)
{
    size_t total_len = height * width;

    // uint16_t rW = 77;
    // uint16_t gW = 151;
    // uint16_t bW = 28;
    int16_t gray;
    for(size_t i=0; i<total_len; i++) {
        //*gray_buf = (rW*rgb_buf[0] + gW*rgb_buf[1] + bW*rgb_buf[2]) >> 8;
        *gray_buf = (77*rgb_buf[0] + 151*rgb_buf[1] + 28*rgb_buf[2]) >> 8;
        gray_buf++;
        rgb_buf += 3;
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