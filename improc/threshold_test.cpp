#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/opencv.hpp>
#include "threshold.h"

int main() {

    cv::Mat image = cv::imread("sky.png");
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    cv::Size size = image.size();
    size_t height = (size_t)size.height;
    size_t width = (size_t)size.width;

    unsigned char* input_gray = gray.data;
    cv::Mat result(size, CV_8UC1);
    unsigned char* output_gray = result.data;
    threshold_gray(gray.data, height, width, output_gray, 60);

    cv::imwrite("sky_thresh60.bmp", result);

    return 0;
}