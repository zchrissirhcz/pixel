#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "common/pixel_benchmark.h"

#ifdef __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

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

static float array_mean_naive(unsigned char* data, size_t len) {
    float sum = 0;
    for (size_t i=0; i<len; i++) {
        sum += data[i];
    }
    sum /= len;
    return sum;
}

// u8转u16再转u32再转f32，然后算sum
static float array_mean_asimd(unsigned char* data, size_t len) {
    float sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON
    size_t step = 8;
    size_t vec_size = len - len % step;
    float sum_lst[4] = {0.f, 0.f, 0.f, 0.f};
    uint8x8_t v1;
    uint16x8_t v2;
    uint32x4_t v3_low, v3_high;
    float32x4_t v4_low, v4_high;
    float32x4_t v5;
    float32x4_t vsum = vdupq_n_f32(0.f);
    for (size_t i=0; i<vec_size; i+=step) {
        v1 = vld1_u8(data);
        data += step;
        v2 = vmovl_u8(v1);
        v3_low = vmovl_u16(vget_low_u16(v2));
        v3_high = vmovl_u16(vget_high_u16(v2));
        v4_low = vcvtq_f32_u32(v3_low);
        v4_high = vcvtq_f32_u32(v3_high);
        v5 = vaddq_f32(v4_low, v4_high);
        vsum = vaddq_f32(v5, vsum);
    }
    vst1q_f32(sum_lst, vsum);
    sum = sum_lst[0] + sum_lst[1] + sum_lst[2] + sum_lst[3];
    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    sum /= len;
    return sum;
}

static float array_mean_asimd2(unsigned char* data, size_t len) {
    float sum = 0;
    size_t done = 0;
#ifdef __ARM_NEON
    size_t step = 32;
    size_t vec_size = len - len % step;
    float sum_lst[4] = {0.f, 0.f, 0.f, 0.f};
    uint8x8x4_t vdata;
    uint16x8_t v21, v22, v23, v24;
    uint32x4_t v31_low, v32_low, v33_low, v34_low;
    uint32x4_t v31_high, v32_high, v33_high, v34_high;
    float32x4_t v41_low, v42_low, v43_low, v44_low;
    float32x4_t v41_high, v42_high, v43_high, v44_high;
    float32x4_t v51, v52, v53, v54;
    float32x4_t vsum1 = vdupq_n_f32(0.f);
    float32x4_t vsum2 = vdupq_n_f32(0.f);
    float32x4_t vsum3 = vdupq_n_f32(0.f);
    float32x4_t vsum4 = vdupq_n_f32(0.f);
    for (size_t i=0; i<vec_size; i+=step) {
        vdata = vld4_u8(data);
        data += step;

        //vdata[0]
        v21 = vmovl_u8(vdata.val[0]);
        v31_low = vmovl_u16(vget_low_u16(v21));
        v31_high = vmovl_u16(vget_high_u16(v21));
        v41_low = vcvtq_f32_u32(v31_low);
        v41_high = vcvtq_f32_u32(v31_high);
        v51 = vaddq_f32(v41_low, v41_high);
        vsum1 = vaddq_f32(v51, vsum1);

        //vdata[1]
        v22 = vmovl_u8(vdata.val[1]);
        v32_low = vmovl_u16(vget_low_u16(v22));
        v32_high = vmovl_u16(vget_high_u16(v22));
        v42_low = vcvtq_f32_u32(v32_low);
        v42_high = vcvtq_f32_u32(v32_high);
        v52 = vaddq_f32(v42_low, v42_high);
        vsum2 = vaddq_f32(v52, vsum2);

        //vdata[2]
        v23 = vmovl_u8(vdata.val[2]);
        v33_low = vmovl_u16(vget_low_u16(v23));
        v33_high = vmovl_u16(vget_high_u16(v23));
        v43_low = vcvtq_f32_u32(v33_low);
        v43_high = vcvtq_f32_u32(v33_high);
        v53 = vaddq_f32(v43_low, v43_high);
        vsum3 = vaddq_f32(v53, vsum3);

        //vdata[3]
        v24 = vmovl_u8(vdata.val[3]);
        v34_low = vmovl_u16(vget_low_u16(v24));
        v34_high = vmovl_u16(vget_high_u16(v24));
        v44_low = vcvtq_f32_u32(v34_low);
        v44_high = vcvtq_f32_u32(v34_high);
        v54 = vaddq_f32(v44_low, v44_high);
        vsum4 = vaddq_f32(v54, vsum4);
    }
    vsum1 = vaddq_f32(vsum1, vsum2);
    vsum1 = vaddq_f32(vsum1, vsum3);
    vsum1 = vaddq_f32(vsum1, vsum4);
    vst1q_f32(sum_lst, vsum1);
    sum = sum_lst[0] + sum_lst[1] + sum_lst[2] + sum_lst[3];
    done = vec_size;
#endif // __ARM_NEON
    for (; done<len; done++) {
        sum += *data;
        data++;
    }
    sum /= len;
    return sum;
}

// <del>u8转u16，算sum，再转float</del>
static float array_mean_asimd3(unsigned char* data, size_t len) {
    uint16x8_t result_level1 = vdupq_n_u16(0);
    uint32x4_t result_level2 = vdupq_n_u32(0);
    uint32x4_t result_vec = vdupq_n_u32(0);
    
    uint8x8_t h_vec;

    uint8x8_t x_vec = vdup_n_u8(1);

    uint32_t t0, t1;

    bool flag;
    size_t vec_size = len / 8;
    for(size_t i=0; i<vec_size; i++) {
        h_vec = vld1_u8(&data[i*8]);
        result_level1 = vmlal_u8(result_level1, h_vec, x_vec);
        flag = false;

        if( (i*8) % 256 == 0) {
            //16x8 => 32x4
            t0 = vgetq_lane_u16(result_level1, 0);
            t1 = vgetq_lane_u16(result_level1, 1);
            result_level2 = vsetq_lane_u32(t0+t1, result_level2, 0);

            t0 = vgetq_lane_u16(result_level1, 2);
            t1 = vgetq_lane_u16(result_level1, 3);
            result_level2 = vsetq_lane_u32(t0+t1, result_level2, 1);

            t0 = vgetq_lane_u16(result_level1, 4);
            t1 = vgetq_lane_u16(result_level1, 5);
            result_level2 = vsetq_lane_u32(t0+t1, result_level2, 2);

            t0 = vgetq_lane_u16(result_level1, 6);
            t1 = vgetq_lane_u16(result_level1, 7);
            result_level2 = vsetq_lane_u32(t0+t1, result_level2, 3);

            result_vec = vaddq_u32(result_level2, result_vec);

            result_level1 = vdupq_n_u16(0);
            flag = true;
        }
    }
    if(!flag) {
        //16x8 => 32x4
        t0 = vgetq_lane_u16(result_level1, 0);
        t1 = vgetq_lane_u16(result_level1, 1);
        result_level2 = vsetq_lane_u32(t0+t1, result_level2, 0);

        t0 = vgetq_lane_u16(result_level1, 2);
        t1 = vgetq_lane_u16(result_level1, 3);
        result_level2 = vsetq_lane_u32(t0+t1, result_level2, 1);

        t0 = vgetq_lane_u16(result_level1, 4);
        t1 = vgetq_lane_u16(result_level1, 5);
        result_level2 = vsetq_lane_u32(t0+t1, result_level2, 2);

        t0 = vgetq_lane_u16(result_level1, 6);
        t1 = vgetq_lane_u16(result_level1, 7);
        result_level2 = vsetq_lane_u32(t0+t1, result_level2, 3);

        result_vec = vaddq_u32(result_level2, result_vec);

        result_level1 = vdupq_n_u16(0);
        flag = true;
    }

    float sum = 0;
    sum += vgetq_lane_u32(result_vec, 0);
    sum += vgetq_lane_u32(result_vec, 1);
    sum += vgetq_lane_u32(result_vec, 2);
    sum += vgetq_lane_u32(result_vec, 3);

    if(len%8!=0) {
        for(int k=len-(len%8); k<len; k++) {
            sum += data[k];
        }
    }

    float avg = sum / len;
    return avg;
}

XYZ rgb2xyz_fused_asimd(cv::Mat Ir, cv::Mat Ig, cv::Mat Ib)
{
    cv::Size size = Ir.size();
    size_t height = size.height;
    size_t width = size.width;
    size_t len = height * width;
    float r_mean = array_mean_asimd(Ir.data, len);
    float g_mean = array_mean_asimd(Ig.data, len);
    float b_mean = array_mean_asimd(Ib.data, len);

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

XYZ rgb2xyz_fused_asimd2(cv::Mat Ir, cv::Mat Ig, cv::Mat Ib)
{
    cv::Size size = Ir.size();
    size_t height = size.height;
    size_t width = size.width;
    size_t len = height * width;
    float r_mean = array_mean_asimd2(Ir.data, len);
    float g_mean = array_mean_asimd2(Ig.data, len);
    float b_mean = array_mean_asimd2(Ib.data, len);

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


XYZ rgb2xyz_fused_asimd3(cv::Mat Ir, cv::Mat Ig, cv::Mat Ib)
{
    cv::Size size = Ir.size();
    size_t height = size.height;
    size_t width = size.width;
    size_t len = height * width;
    float r_mean = array_mean_asimd3(Ir.data, len);
    float g_mean = array_mean_asimd3(Ig.data, len);
    float b_mean = array_mean_asimd3(Ib.data, len);

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

void test_rgb2xyz(std::string image_path){

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

    t_start = pixel_get_current_time();
    xyz = rgb2xyz_fused_asimd2(channels[0], channels[1], channels[2]);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2xyz, fused asimd2,  time=%.4lf ms, x=%.4lf, y=%.4lf, z=%.4lf\n", t_cost, xyz.x, xyz.y, xyz.z);

    t_start = pixel_get_current_time();
    xyz = rgb2xyz_fused_asimd3(channels[0], channels[1], channels[2]);
    t_cost = pixel_get_current_time() - t_start;
    printf("rgb2xyz, fused asimd3,  time=%.4lf ms, x=%.4lf, y=%.4lf, z=%.4lf\n", t_cost, xyz.x, xyz.y, xyz.z);
}

void test_array_mean() {
    cv::Mat image = cv::imread("sky.jpg");
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    cv::Mat Ir = channels[0];
    cv::Mat Ig = channels[1];
    cv::Mat Ib = channels[2];

    cv::Size size = image.size();
    int height = size.height;
    int width = size.width;
    size_t len = height * width;

    float sum = 0;
    double t_start, t_cost;


    unsigned char* data = Ir.data;

    t_start = pixel_get_current_time();
    sum = array_mean_naive(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, naive impl, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_asimd(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl, result=%.4f, time cost %.4lf\n", sum, t_cost);

    t_start = pixel_get_current_time();
    sum = array_mean_asimd2(data, len);
    t_cost = pixel_get_current_time() - t_start;
    printf("array mean, asimd impl2, result=%.4f, time cost %.4lf\n", sum, t_cost);

}

int main() {

    test_rgb2xyz("colorhouse.jpg");
    test_rgb2xyz("sky.jpg");
    
    //test_array_mean();

    return 0;
}
