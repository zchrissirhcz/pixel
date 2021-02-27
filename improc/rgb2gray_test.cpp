#include "rgb2gray.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"
#include "common/pixel_cpu_affinity.h"

#include <opencv2/opencv.hpp>


int main() {
    size_t mask = 0;
    for (int i = 0; i < 8; ++i) {
      if (i >= 5) {
        mask |= (1 << i);
      }
    }
    int ret = set_sched_affinity(mask);

    cv::Mat image = cv::imread("sky.jpg");
    cv::Mat image_rgb;
    cv::cvtColor(image, image_rgb, cv::COLOR_BGR2RGB);

    unsigned char* rgb_buf = image_rgb.data;
    cv::Size size = image_rgb.size();
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;
    size_t rgb_linebytes = width * 3;

    unsigned char* gray_buf = NULL;
    size_t gray_linebytes = width;

    cv::Mat gray_naive(size, CV_8UC1);
    cv::Mat gray_fixed(size, CV_8UC1);
    cv::Mat gray_fixed_asimd(size, CV_8UC1);
    cv::Mat gray_opencv(size, CV_8UC1);
    cv::Mat gray_fixed_asm0(size, CV_8UC1);
    cv::Mat gray_fixed_asm(size, CV_8UC1);

    double t_start, t_cost;

    gray_buf = gray_naive.data;
    t_start = pixel_get_current_time();
    pixel_rgb2gray_naive(rgb_buf, height, width, gray_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2gray naive, size h=%zu, w=%zu, time cost %.6lf ms\n", height, width, t_cost);
    cv::imwrite("sky_gray_naive.bmp", gray_naive);

    gray_buf = gray_fixed.data;
    t_start = pixel_get_current_time();
    pixel_rgb2gray_fixed(rgb_buf, height, width, gray_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2gray fixed, size h=%zu, w=%zu, time cost %.6lf ms\n", height, width, t_cost);
    cv::imwrite("sky_gray_fixed.bmp", gray_fixed);
    
    gray_buf = gray_fixed_asimd.data;
    t_start = pixel_get_current_time();
    pixel_rgb2gray_fixed_asimd(rgb_buf, height, width, gray_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2gray fixed asimd, size h=%zu, w=%zu, time cost %.6lf ms\n", height, width, t_cost);
    cv::imwrite("sky_gray_asimd.bmp", gray_fixed_asimd);

    t_start = pixel_get_current_time();
    cv::cvtColor(image_rgb, gray_opencv, cv::COLOR_RGB2GRAY);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2gray, opencv, size h=%zu, w=%zu, time cost %.6lf ms\n", height, width, t_cost);
    cv::imwrite("sky_gray_opencv.bmp", gray_opencv);

    gray_buf = gray_fixed_asm.data;
    t_start = pixel_get_current_time();
    pixel_rgb2gray_fixed_asm(rgb_buf, height, width, gray_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("rgb2gray, fixed asm, size h=%zu, w=%zu, time cost %.6lf ms\n", height, width, t_cost);
    cv::imwrite("sky_gray_fixed_asm.bmp", gray_fixed_asm);

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