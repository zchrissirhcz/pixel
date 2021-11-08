#include <opencv2/opencv.hpp>
#include "common/cv_puttextzh.h"

int main() {
    cv::Mat image = cv::imread("colorhouse.png");

    cv::Size size = image.size();
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    cv::Mat dst;

    cv::imshow("原图", image);

    cv::Sobel(image, grad_x, CV_8U, 1, 0, 3, 1, 1, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::imshow("X方向Sobel", abs_grad_x);

    cv::Sobel(image, grad_y, CV_8U, 0, 1, 3, 1, 1, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::imshow("Y方向Sobel", abs_grad_y);

    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dst);
    cv::imshow("整体方向Sobel", dst);

    cv::waitKey(0);

    return 0;
}