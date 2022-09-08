#include "histogram.h"
#include "px_timer.h"

#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat image = cv::imread("sky.jpg");
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

    cv::Size size = image.size();
    size_t height = (size_t)size.height;
    size_t width  = (size_t)size.width;

    unsigned int hist[256];
    memset(hist, 0, sizeof(hist));

    unsigned char* gray_buf = NULL;
    double t_start, t_cost;

    gray_buf = gray.data;
    t_start = px_get_current_time();
    histogram_gray_naive(gray_buf, height, width, hist);
    t_cost = px_get_current_time() - t_start;
    printf("histogram_gray, naive, time cost %.4lf ms\n", t_cost);

    memset(hist, 0, sizeof(hist));
    t_start = px_get_current_time();
    histogram_gray_asimd(gray_buf, height, width, hist);
    t_cost = px_get_current_time() - t_start;
    printf("histogram_gray, asimd, time cost %.4lf ms\n", t_cost);

    // memset(hist, 0, sizeof(hist));
    // unsigned char* rgb_buf = image.data;
    // t_start = px_get_current_time();
    // px_histogram_rgb(rgb_buf, height, width, hist, 1);
    // t_cost = px_get_current_time() - t_start;
    // printf("histogram_rgb, naive, time cost %.4lf ms\n", t_cost);

    return 0;
}