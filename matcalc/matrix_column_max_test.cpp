#include "matrix_column_max.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/opencv.hpp>

int main() {
    cv::Mat image = cv::imread("sky.png");
    cv::Size size = image.size();
    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat result_naive(size, CV_8UC1);
    cv::Mat result_opencv(size, CV_8UC1);

    double t_start, t_cost;
    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;

    // naive
    src_buf = gray.data;
    dst_buf = result_naive.data;
    t_start = pixel_get_current_time();
    matrix_column_max_u8_naive(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max u8, naive, time cost %4lf ms\n", t_cost);

    // opencv
    src_buf = gray.data;
    dst_buf = result_naive.data;
    t_start = pixel_get_current_time();
    (src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max u8, opencv, time cost %4lf ms\n", t_cost);


    // TODO: opencv里怎么对二维数组逐列求最小/最大？ 似乎是minMaxLoc和minMaxIdx，但是没看出区别来
    // https://blog.csdn.net/zhangqinghao0811/article/details/81538173
    // https://stackoverflow.com/questions/46024744/how-find-index-of-max-in-each-row-of-opencv-mat-boject
    //
    //
    cv::Mat foo1(3,3,CV_8UC1);

    foo1.setTo(1);
    foo1.at<uchar>(1,1) = 2;

    double a,b;
    cv::Point p1,p2;

    cv::minMaxLoc(foo1,&a,&b,&p1,&p2);

    // 1 2 [0, 0] [1, 1]
    cout << a << " " << b << " " << p1 << " " << p2 << endl;


    int arr[3] = {4,3,2};
    Mat foo2(3, arr, CV_8UC1);

    foo2.setTo(0);
    foo2.at<uchar>(2,1,0) = 2;


    int minInd[3];
    int maxInd[3];
    cv::minMaxIdx(foo2, &a, &b, minInd, maxInd, Mat());

    // 0 2 [0 0 0] [2 1 0]
    cout << a << " " << b << " [" << minInd[0] << " " << minInd[1] << " " << minInd[2] << "] [" << maxInd[0] << " " << maxInd[1] << " " << maxInd[2] << "]" << endl;


    return 0;
}