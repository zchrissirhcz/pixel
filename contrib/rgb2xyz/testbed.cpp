#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "common/pixel_benchmark.h"
#include "matcalc/array_mean.h"

typedef struct XYZ {
    float x, y, z;
} XYZ;

XYZ rgb2xyz_naive(cv::Mat Ir, cv::Mat Ig, cv::Mat Ib)
{
    cv::Mat Ix = Ir * 0.412453 + Ig * 0.357580 + Ib * 0.180423;
    cv::Mat Iy = Ir * 0.212671 + Ig * 0.715160 + Ib * 0.072169;
    cv::Mat Iz = Ir * 0.019334 + Ig * 0.119193 + Ib * 0.950227;

    double Sxyz = cv::sum(mean(Ix).val[0] + mean(Iy).val[0] + mean(Iz).val[0]).val[0];

    XYZ xyz;
    xyz.x = mean(Ix).val[0] / Sxyz;
    xyz.y = mean(Iy).val[0] / Sxyz;
    xyz.z = mean(Iz).val[0] / Sxyz;

    return xyz;
}

XYZ rgb2xyz_fused(cv::Mat Ir, cv::Mat Ig, cv::Mat Ib)
{
    cv::Mat Dr = 1.0 * Ir;
    cv::Mat Dg = 1.0 * Ig;
    cv::Mat Db = 1.0 * Ib;

    double r_mean = cv::mean(Dr).val[0];
    double g_mean = cv::mean(Dg).val[0];
    double b_mean = cv::mean(Db).val[0];

    double x_mean = 0.412453 * r_mean + 0.357580 * g_mean + 0.180423 * b_mean;
    double y_mean = 0.212671 * r_mean + 0.715160 * g_mean + 0.072169 * b_mean;
    double z_mean = 0.019334 * r_mean + 0.119193 * g_mean + 0.950227 * b_mean;

    double Sxyz = x_mean + y_mean + z_mean;
    XYZ xyz;
    xyz.x = x_mean / Sxyz;
    xyz.y = y_mean / Sxyz;
    xyz.z = z_mean / Sxyz;

    return xyz;
}

XYZ rgb2xyz_fused_float(cv::Mat Ir, cv::Mat Ig, cv::Mat Ib)
{
    cv::Mat Fr = 1.0 * Ir;
    cv::Mat Fg = 1.0 * Ig;
    cv::Mat Fb = 1.0 * Ib;

    float r_mean = cv::mean(Fr).val[0];
    float g_mean = cv::mean(Fg).val[0];
    float b_mean = cv::mean(Fb).val[0];

    float x_mean = 0.412453f * r_mean + 0.357580f * g_mean + 0.180423f * b_mean;
    float y_mean = 0.212671f * r_mean + 0.715160f * g_mean + 0.072169f * b_mean;
    float z_mean = 0.019334f * r_mean + 0.119193f * g_mean + 0.950227f * b_mean;

    float Sxyz = x_mean + y_mean + z_mean;
    XYZ xyz;
    xyz.x = x_mean / Sxyz;
    xyz.y = y_mean / Sxyz;
    xyz.z = z_mean / Sxyz;

    return xyz;
}

XYZ rgb2xyz_fused_asimd(cv::Mat Ir, cv::Mat Ig, cv::Mat Ib)
{
    cv::Size size = Ir.size();
    size_t height = size.height;
    size_t width = size.width;
    size_t len = height * width;
    float r_mean = array_mean_asimd4(Ir.data, len);
    float g_mean = array_mean_asimd4(Ig.data, len);
    float b_mean = array_mean_asimd4(Ib.data, len);

    float x_mean = 0.412453f * r_mean + 0.357580f * g_mean + 0.180423f * b_mean;
    float y_mean = 0.212671f * r_mean + 0.715160f * g_mean + 0.072169f * b_mean;
    float z_mean = 0.019334f * r_mean + 0.119193f * g_mean + 0.950227f * b_mean;

    float Sxyz = x_mean + y_mean + z_mean;
    XYZ xyz;
    xyz.x = x_mean / Sxyz;
    xyz.y = y_mean / Sxyz;
    xyz.z = z_mean / Sxyz;

    return xyz;
}

void test_rgb2xyz(const std::string& image_path){

    cv::Mat image = cv::imread(image_path);
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

    cv::Size size = image.size();
    int height = size.height;
    int width = size.width;
    printf("image info: height=%d, width=%d\n", height, width);

    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    double t_start, t_cost;
    XYZ xyz;

    t_start = pixel_get_current_time();
    xyz = rgb2xyz_naive(channels[0], channels[1], channels[2]);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2xyz, naive impl,    time=%.4lf ms, x=%.4lf, y=%.4lf, z=%.4lf\n", t_cost, xyz.x, xyz.y, xyz.z);

    t_start = pixel_get_current_time();
    xyz = rgb2xyz_fused(channels[0], channels[1], channels[2]);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2xyz, fused double,  time=%.4lf ms, x=%.4lf, y=%.4lf, z=%.4lf\n", t_cost, xyz.x, xyz.y, xyz.z);

    t_start = pixel_get_current_time();
    xyz = rgb2xyz_fused_float(channels[0], channels[1], channels[2]);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2xyz, fused float,   time=%.4lf ms, x=%.4lf, y=%.4lf, z=%.4lf\n", t_cost, xyz.x, xyz.y, xyz.z);

    t_start = pixel_get_current_time();
    xyz = rgb2xyz_fused_asimd(channels[0], channels[1], channels[2]);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2xyz, fused asimd,   time=%.4lf ms, x=%.4lf, y=%.4lf, z=%.4lf\n", t_cost, xyz.x, xyz.y, xyz.z);
}


int main() {

    test_rgb2xyz("colorhouse.jpg");
    test_rgb2xyz("sky.jpg");

    return 0;
}
