#include "rgb2gray.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"

#include <opencv2/opencv.hpp>


int main() {
    cv::Mat image = cv::imread("sky.jpg");
    cv::Mat image_rgb;
    cv::cvtColor(image, image_rgb, cv::COLOR_BGR2RGB);

    unsigned char* rgb_buf = image.data;
    cv::Size size = image_rgb.size();
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;
    size_t rgb_linebytes = width * 3;

    cv::Mat gray(size, CV_8UC1);
    unsigned char* gray_buf = gray.data;
    size_t gray_linebytes = width;

    double t_start = pixel_get_current_time();
    //rgb_to_gray_naive(rgb_buf, height, width, rgb_linebytes, gray_buf, gray_linebytes);
    rgb_to_gray_fast(rgb_buf, height, width, rgb_linebytes, gray_buf, gray_linebytes);
    double t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2gray, size h=%zu, w=%zu, time cost %.6lf ms\n", height, width, t_cost);
    cv::imwrite("sky_gray.bmp", gray);

#if 0
    cv::Mat gray_mean(size, CV_8UC1);
    gray_buf = gray_mean.data;
    f_rgb_to_gray(rgb_buf, height, width, rgb_linebytes, gray_buf, gray_linebytes, 0);
    cv::imwrite("mingren_gray_mean.bmp", gray);

    cv::Mat gray_classical(size, CV_8UC1);
    gray_buf = gray_classical.data;
    f_rgb_to_gray(rgb_buf, height, width, rgb_linebytes, gray_buf, gray_linebytes, 0);
    cv::imwrite("mingren_gray_classical.bmp", gray);

    cv::Mat gray_photoshop(size, CV_8UC1);
    gray_buf = gray_photoshop.data;
    f_rgb_to_gray(rgb_buf, height, width, rgb_linebytes, gray_buf, gray_linebytes, 0);
    cv::imwrite("mingren_gray_photoshop.bmp", gray);
#endif

    return 0;
}