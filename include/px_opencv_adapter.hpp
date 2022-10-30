#pragma once

#include "px_image.h"
#include <opencv2/opencv.hpp>

static px_image_t* px_image_from_opencv(cv::Mat& image)
{
    const int height = image.rows;
    const int width = image.cols;
    const int channel = image.channels();
    px_image_t* px_image = px_create_image_header(height, width, channel);
    px_image->data = image.data;
    return px_image;
}