#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/opencv.hpp>

int main() {

    cv::Mat image = cv::imread("sky.png");
    cv::Size size = image.size();
    printf("image: h=%d, w=%d\n", size.height, size.width);

    cv::Mat result;
    cv::Size kernel_size(3, 3);
    boxFilter(image, result, 8, kernel_size);
    size = result.size();
    printf("result: h=%d, w=%d\n", size.height, size.width);

    cv::imwrite("sky_boxfilter.png", result);

    return 0;
}