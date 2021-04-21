#include "copy_cut_border.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "copy_cut_border.h"
#include "common/pixel_benchmark.h"

namespace cv {
    void copyCutBorder(cv::Mat src, cv::Mat& dst, int top, int bottom, int left, int right)
    {
        cv::Size src_size = src.size();
        cv::Size dst_size;
        dst_size.height = src_size.height - top - bottom;
        dst_size.width = src_size.width - left - right;

        cv::Rect rect;
        rect.x = left;
        rect.y = top;
        rect.width = dst_size.width;
        rect.height = dst_size.height;

        dst = cv::Mat(src, rect).clone();
    }
}

int main() {

    cv::Mat src_image = cv::imread("IU.bmp");
    cv::Size src_size = src_image.size();

    printf("src_image info: height=%d, width=%d\n", src_size.height, src_size.width);

    // int border = 2;
    // int top = border;
    // int bottom = border;
    // int left = border;
    // int right = border;

    int top = 10;
    int bottom = 2;
    int left = 2;
    int right = 2;

    cv::Size dst_size;
    dst_size.height = src_size.height - top - bottom;
    dst_size.width = src_size.width - left - right;

    double t_start, t_cost;

    cv::Mat dst_opencv;

    t_start = pixel_get_current_time();
    cv::copyCutBorder(src_image, dst_opencv, top, bottom, left, right);
    t_cost = pixel_get_current_time() - t_start;
    printf("cv::copyCutBorder time cost %.4lf ms\n", t_cost);

    cv::imwrite("IU_cutborder_opencv.png", dst_opencv);

    //-------------------------------------

    t_start = pixel_get_current_time();
    cv::Mat dst_naive(dst_size, src_image.type());
    unsigned char* src = src_image.data;
    unsigned char* dst = dst_naive.data;
    int channels = src_image.channels();

    
    copy_cut_border(src, src_size.height, src_size.width, channels, dst, top, bottom, left, right);
    t_cost = pixel_get_current_time() - t_start;
    printf("copy_cut_border time cost %.4lf ms\n", t_cost);

    cv::imwrite("IU_cutborder_naive.png", dst_naive);

    return 0;
}