// original code https://zhuanlan.zhihu.com/p/61216321

#include <iostream>
#include <opencv2/opencv.hpp>

#include "pixel_benchmark.h"
#include "pixel_log.h"

#include <Eigen/Dense>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    int dim = 1000;

    cv::Mat m1 = (cv::Mat_<float>(dim, dim) << 1., 0., 3., 0., 5., 6., 7., 8., 0.);
    cv::Mat m2 = (cv::Mat_<float>(dim, dim) << 0., 2., 1., 4., 5., 6., 7., 1., 0.);
    cv::Mat m3 = cv::Mat::zeros(dim, dim, CV_32F);
    const int times = 10;

    // opencv multiple
    double t_start = pixel_get_current_time();
    for (int i = 0; i < times; i++)
    {
        m3 += m1 * m2;
    }
    double t_cost = pixel_get_current_time() - t_start;
    printf("Opencv Multiple %d cost %lf ms.\n", times, t_cost);
    //cout<<"Result: "<<endl<<m3<<endl;

    // opencv add
    m3 = cv::Mat::zeros(dim, dim, CV_32F);
    t_start = pixel_get_current_time();
    for (int i = 0; i < times; i++)
    {
        m3 += m1 + m2;
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("Opencv Add %d cost %lf ms.\n", times, t_cost);
    //cout<<"Result: "<<endl<<m3<<endl;

    Eigen::Matrix3f eM1, eM2, eM3;
    eM1 << 1., 0., 3., 0., 5., 6., 7., 8., 0.;
    eM2 << 0., 2., 1., 4., 5., 6., 7., 1., 0.;
    eM3 = Eigen::Matrix3f::Zero();

    // eigen multiple
    t_start = pixel_get_current_time();
    for (int i = 0; i < times; i++)
    {
        eM3 += eM1 * eM2;
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("Eigen Multiple %d cost %lf ms.\n", times, t_cost);
    //cout<<"Result: "<<endl<<eM3<<endl;

    // eigen add
    eM3 = Eigen::Matrix3f::Zero();
    t_start = pixel_get_current_time();
    for (int i = 0; i < times; i++)
    {
        eM3 += eM1 + eM2;
    }
    t_cost = pixel_get_current_time() - t_start;
    printf("Eigen Add %d cost %lf ms.\n", times, t_cost);
    //cout<<"Result: "<<endl<<eM3<<endl;

    return 0;
}