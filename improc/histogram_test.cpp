#include "histogram.h"
#include "common/pixel_benchmark.h"

#include <opencv2/opencv.hpp>

int main() {
    cv::Mat image = cv::imread("sky.png");
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

    cv::Size size = image.size();
    size_t height = (size_t)size.height;
    size_t width  = (size_t)size.width;

    unsigned char hist[256];
    memset(hist, 0, sizeof(hist));

    unsigned char* gray_buf = NULL;
    double t_start, t_cost;

    gray_buf = gray.data;
    t_start = pixel_get_current_time();
    histogram_gray_naive(gray_buf, height, width, hist);
    t_cost = pixel_get_current_time() - t_start;
    printf("histogram_gray, naive, time cost %.4lf ms\n", t_cost);

    memset(hist, 0, sizeof(hist));
    unsigned char* rgb_buf = image.data;
    t_start = pixel_get_current_time();
    histogram_rgb_naive(rgb_buf, height, width, hist, 1);
    t_cost = pixel_get_current_time() - t_start;
    printf("histogram_gray, naive, time cost %.4lf ms\n", t_cost);

    return 0;
}