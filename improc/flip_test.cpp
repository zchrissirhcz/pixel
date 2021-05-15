#include "flip.h"
#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"

#include <opencv2/opencv.hpp>

#include "common/autotimer.h"

//cv::flip(src, dst, mode);
//mode=0, 上下颠倒  vertically
//mode=1，左右颠倒  horizontally
//mode=-1, against center point

void flip_horiz_rgb_test() {

    //cv::Mat image = cv::imread("colorhouse.jpg");
    cv::Mat image = cv::imread("sky.jpg");
    cv::Size size = image.size();
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;

    cv::Mat result_opencv(size, CV_8UC3);
    cv::Mat result_naive(size, CV_8UC3);
    cv::Mat result_idxopt(size, CV_8UC3);
    cv::Mat result_asimd(size, CV_8UC3);

    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;

    PIXEL_LOGD("image info: height=%zu, width=%zu\n", height, width);

    // opencv
    {
        AutoTimer timer("flip rgb horiz, opencv");
        cv::flip(image, result_opencv, 1);
    }

    // naive
    src_buf = image.data;
    dst_buf = result_naive.data;
    {
        AutoTimer timer("flip rgb horiz, naive");
        flip_horiz_rgb_naive(src_buf, height, width, dst_buf);
    }

    // idxopt
    src_buf = image.data;
    dst_buf = result_idxopt.data;
    {
        AutoTimer("flip rgb horiz, idxopt");
        flip_horiz_rgb_idxopt(src_buf, height, width, dst_buf);
    }

    // asimd
    src_buf = image.data;
    dst_buf = result_asimd.data;
    {
        AutoTimer timer("flip rgb horiz, asimd");
        flip_horiz_rgb_asimd(src_buf, height, width, dst_buf);
    }

    cv::imwrite("sky_flip_opencv.png", result_opencv);
    cv::imwrite("sky_flip_naive.png", result_naive);
    cv::imwrite("sky_flip_idxopt.png", result_idxopt);
    cv::imwrite("sky_flip_asimd.png", result_asimd);
}

void flip_horiz_gray_test()
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

    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;

    // opencv
    {
        AutoTimer timer("flip gray horiz, opencv");
        cv::flip(gray, result_opencv, 1);
    }

    // naive
    src_buf = gray.data;
    dst_buf = result_naive.data;
    {
        AutoTimer timer("flip gray horiz, naive");
        flip_horiz_gray_naive(src_buf, height, width, dst_buf);
    }

    // asimd
    src_buf = gray.data;
    dst_buf = result_naive.data;
    {
        AutoTimer timer("flip gray horiz, asimd");
        flip_horiz_gray_asimd(src_buf, height, width, dst_buf);
    }

    cv::imwrite("sky_gray_flip_opencv.png", result_opencv);
    cv::imwrite("sky_gray_flip_naive.png", result_naive);
    cv::imwrite("sky_gray_flip_asimd.png", result_asimd);
}

static void flip_vert_rgb_test()
{
    cv::Mat image = cv::imread("sky.png");
    cv::Size size = image.size();
    size_t height = size.height;
    size_t width = size.width;

    cv::Mat result_opencv(size, CV_8UC3);
    cv::Mat result_naive(size, CV_8UC3);
    cv::Mat result_bylines(size, CV_8UC3);

    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;

    // opencv
    {
        AutoTimer timer("flip rgb vertically, opencv");
        cv::flip(image, result_opencv, 0);
    }

    // opencv inplace
    cv::Mat result_opencv_inplace = image.clone();
    {
        AutoTimer timer("flip rgb vertically, opencv inplace");
        cv::flip(result_opencv_inplace, result_opencv_inplace, 0);
    }

    // opencv inplace trick
    cv::Mat shadow1 = image.clone();
    cv::Mat result_opencv_inplace_trick = shadow1;
    {
        AutoTimer timer("flip rgb vertically, opencv inplace trick");
        cv::flip(shadow1, result_opencv_inplace_trick, 0);
    }

    // naive
    src_buf = image.data;
    dst_buf = result_naive.data;
    {
        AutoTimer timer("flip rgb vertically, naive");
        flip_vert_rgb_naive(src_buf, height, width, dst_buf);
    }

    //flip_vert_rgb_bylines
    src_buf = image.data;
    dst_buf = result_bylines.data;
    {
        AutoTimer timer("flip rgb vertically");
        flip_vert_rgb_bylines(src_buf, height, width, dst_buf);
    }

    cv::imwrite("sky_flip_vert_rgb_opencv.png", result_opencv);
    cv::imwrite("sky_flip_vert_rgb_naive.png", result_naive);
    cv::imwrite("sky_flip_vert_rgb_bylines.png", result_bylines);
}

static void flip_vert_gray_test()
{
    cv::Mat image = cv::imread("sky.png");
    cv::Size size = image.size();
    size_t height = size.height;
    size_t width = size.width;
    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Mat result_opencv(size, CV_8UC1);
    cv::Mat result_naive(size, CV_8UC1);
    cv::Mat result_bylines(size, CV_8UC1);

    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;

    // opencv
    src_buf = gray.data;
    dst_buf = result_opencv.data;
    {
        AutoTimer timer("flip gray vertically, opencv");
        cv::flip(gray, result_opencv, 0);
    }

    // naive
    src_buf = gray.data;
    dst_buf = result_naive.data;
    {
        AutoTimer timer("flip gray vertically, naive");
        flip_vert_gray_naive(src_buf, height, width, dst_buf);
    }

    //flip_vert_gray_bylines
    src_buf = gray.data;
    dst_buf = result_bylines.data;
    {
        AutoTimer timer("flip gray vertically, by lines");
        flip_vert_gray_bylines(src_buf, height, width, dst_buf);
    }

    cv::imwrite("sky_flip_vert_gray_opencv.png", result_opencv);
    cv::imwrite("sky_flip_vert_gray_naive.png", result_naive);
    cv::imwrite("sky_flip_vert_gray_bylines.png", result_bylines);
}

int main() {

    flip_horiz_rgb_test();
    //flip_horiz_gray_test();

    //flip_vert_rgb_test();
    //flip_vert_gray_test();

    return 0;
}