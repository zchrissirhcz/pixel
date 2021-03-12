#include "flip.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"

#include <opencv2/opencv.hpp>

//cv::flip(src, dst, mode);
//mode=0, 上下颠倒  vertically
//mode=1，左右颠倒  horizontally
//mode=-1, against center point

void flip_rgb_test() {
    
    //cv::Mat image = cv::imread("colorhouse.jpg");
    cv::Mat image = cv::imread("sky.jpg");
    cv::Size size = image.size();
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;
    
    cv::Mat result_opencv(size, CV_8UC3);
    cv::Mat result_naive(size, CV_8UC3);
    cv::Mat result_idxopt(size, CV_8UC3);
    cv::Mat result_asimd(size, CV_8UC3);

    double t_start, t_cost;
    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;
    
    PIXEL_LOGD("image info: height=%zu, width=%zu\n", height, width);

    // opencv
    t_start = pixel_get_current_time();
    cv::flip(image, result_opencv, 1);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("flip rgb horiz, opencv, time cost %.4lf ms\n", t_cost);

    // naive
    src_buf = image.data;
    dst_buf = result_naive.data;
    t_start = pixel_get_current_time();
    flip_rgb_horiz_naive(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("flip rgb horiz, naive,  time cost %.4lf ms\n", t_cost);

    // idxopt
    src_buf = image.data;
    dst_buf = result_idxopt.data;
    t_start = pixel_get_current_time();
    flip_rgb_horiz_idxopt(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("flip rgb horiz, idxopt, time cost %.4lf ms\n", t_cost);

    // asimd
    src_buf = image.data;
    dst_buf = result_asimd.data;
    t_start = pixel_get_current_time();
    flip_rgb_horiz_asimd(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("flip rgb horiz, asimd, time cost %.4lf ms\n", t_cost);

    cv::imwrite("sky_flip_opencv.png", result_opencv);
    cv::imwrite("sky_flip_naive.png", result_naive);
    cv::imwrite("sky_flip_idxopt.png", result_idxopt);
    cv::imwrite("sky_flip_asimd.png", result_asimd);
}

void flip_gray_test()
{
    cv::Mat image = cv::imread("sky.png");
    cv::Size size = image.size();
    size_t height = size.height;
    size_t width = size.width;
    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat result_opencv(size, CV_8UC1);
    cv::Mat result_naive(size, CV_8UC1);
    cv::Mat result_asimd(size, CV_8UC1);

    double t_start, t_cost;
    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;

    // opencv
    t_start = pixel_get_current_time();
    cv::flip(gray, result_opencv, 1);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("flip gray horiz, opencv, time cost %.4lf ms\n", t_cost);
    
    // naive
    src_buf = gray.data;
    dst_buf = result_naive.data;
    t_start = pixel_get_current_time();
    flip_gray_horiz_naive(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("flip gray horiz, naive,  time cost %.4lf ms\n", t_cost);

    // asimd
    src_buf = gray.data;
    dst_buf = result_naive.data;
    t_start = pixel_get_current_time();
    flip_gray_horiz_asimd(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("flip gray horiz, asimd,  time cost %.4lf ms\n", t_cost);

    cv::imwrite("sky_gray_flip_opencv.png", result_opencv);
    cv::imwrite("sky_gray_flip_naive.png", result_naive);
    cv::imwrite("sky_gray_flip_asimd.png", result_asimd);
}

int main() {

    //flip_rgb_test();
    flip_gray_test();

    return 0;
}