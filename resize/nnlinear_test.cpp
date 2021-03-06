#include "nnlinear.h"
#include "common/pixel_benchmark.h"

#include <opencv2/opencv.hpp>

int main() {

    cv::Mat image = cv::imread("sky.png");
    cv::Size src_size = image.size();

    size_t src_h = (size_t)src_size.height;
    size_t src_w = (size_t)src_size.width;

    size_t dst_h = src_h / 2;
    size_t dst_w = src_w / 2;
    cv::Size dst_size;
    dst_size.height = dst_h;
    dst_size.width = dst_w;

    cv::Mat result_naive(dst_size, CV_8UC3);
    cv::Mat result_opencv(dst_size, CV_8UC3);

    double t_start, t_cost;
    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL; 


    src_buf = image.data;
    dst_buf = result_naive.data;
    t_start = pixel_get_current_time();
    nnlinear_rgb(src_buf, src_h, src_w, dst_buf, dst_h, dst_w);
    t_cost = pixel_get_current_time() - t_start;
    printf("nnlinear, naive, time cost %.4lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    cv::resize(image, result_opencv, dst_size, 0, 0, cv::INTER_NEAREST);
    t_cost = pixel_get_current_time() - t_start;
    printf("nnlinear, opencv, time cost %.4lf ms\n", t_cost);


    cv::imwrite("sky_half_naive.png", result_naive);
    cv::imwrite("sky_half_opencv.png", result_opencv);


    return 0;
}