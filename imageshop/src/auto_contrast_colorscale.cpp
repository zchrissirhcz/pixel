/*
 * 调整图像- 自动对比度、自动色阶算法
 *
 * 原文地址：https://www.cnblogs.com/Imageshop/archive/2011/11/12/2246808.html
 * 
 * 这里采用C++重新实现，并在Ubuntu 20.04下用clang++ 10.0进行编译和测试。
 * 用到了OpenCV做图像读写，用到了C++11的时间戳实现跨平台的时间开销统计。
 * 
 * 目前参照imageshop的文章实现，简单测试后结果不对。
 * 需要搭配 autolevel.m （网友基于imageshop代码写的matlab实现）对比调试。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>
#include <chrono>

#include <opencv2/opencv.hpp>

struct RGBImage {
    int h, w;
    unsigned char* data;
};

template<typename T, typename P>
std::string toString(std::chrono::duration<T,P> dt)
{
    std::ostringstream str;
    using namespace std::chrono;
    str << duration_cast<microseconds>(dt).count()*1e-3 << " ms";
    return str.str();
}

RGBImage create_image(int h, int w)
{
    RGBImage im;
    im.h = h;
    im.w = w;
    size_t sz = h * w * 3;
    im.data = (unsigned char*)malloc(sz);

    return im;
}

void copy_image_buffer(RGBImage* src, RGBImage* dst)
{
    if( (src==NULL || dst==NULL)
        || (src->h!=dst->h)
        || (src->w!=dst->w)
        || (src->data==NULL)
        || (dst->data==NULL)
    )
    {
        fprintf(stderr, "invalid inputs\n");
        return;
    }

    size_t sz = src->h * src->w * 3;
    memcpy(dst->data, src->data, sz);
}

void get_min_max(int* hist, int pixel_amount, float low_cut, float high_cut, 
        int* _min_val, int* _max_val) {
    int sum = 0;
    int min_val = 0;
    for (int i=0; i<=255; i++) {
        sum += hist[i];
        if (sum>=pixel_amount*low_cut) {
            min_val = i;
            break;
        }
    }

    sum = 0;
    int max_val = 0;
    for (int i=255; i>=0; i--) {
        sum += hist[i];
        if (sum >= pixel_amount * high_cut) {
            max_val = i;
            break;
        }
    }

    *_min_val = min_val;
    *_max_val = max_val;
}

// 获取一个通道的颜色映射表，用于自动色阶
void get_channel_color_map(int* color_map, int min_val, int max_val)
{
    for (int i=0; i<255; i++) {
        if (i<=min_val) {
            color_map[i] = 0;
        }
        else if (i>=max_val) {
            color_map[i] = 255;
        }
        else {
            color_map[i] = (i-min_val) / (max_val - min_val) * 255;
        }
    }
}

int main() {
    std::string im_pth = "fog.png";
    cv::Mat mat = cv::imread(im_pth);
    
    RGBImage im;
    im.h = mat.rows;
    im.w = mat.cols;
    im.data = mat.data;
    int channels = 3;

    float low_cut = 0.005;
    float high_cut = 0.005;

    // step1: 分别统计各通道（RGB）的直方图
    int HistRed[256];
    int HistGreen[256];
    int HistBlue[256];
    int stride = im.w * channels;
    int line_pad = stride - im.w * channels;
    int speed;
    for (int i=0; i<im.h; i++) {
        speed = i * stride;
        for(int j=0; j<im.w; j++) {
            unsigned char red = im.data[speed+2];
            unsigned char green = im.data[speed+1];
            unsigned char blue = im.data[speed];
            HistRed[red]++;
            HistGreen[green]++;
            HistBlue[blue]++;
            speed += 3;
        }
        speed += line_pad;
    }

    // step2: 分别计算各通道按照给定的参数所确定的上下限值
    int pixel_amount = im.h * im.w;
    int min_blue=0, max_blue=0;
    int min_green=0, max_green=0;
    int min_red=0, max_red=0;
    get_min_max(HistBlue, pixel_amount, low_cut, high_cut, &min_blue, &max_blue);
    get_min_max(HistGreen, pixel_amount, low_cut, high_cut, &min_green, &max_green);
    get_min_max(HistRed, pixel_amount, low_cut, high_cut, &min_red, &max_red);


    // --------------------------------
    // 自动色阶
    // --------------------------------
    // 获取颜色表
    int blue_map[256];
    int green_map[256];
    int red_map[256];
    get_channel_color_map(blue_map, min_blue, max_blue);
    get_channel_color_map(green_map, min_green, max_red);
    get_channel_color_map(red_map, min_green, max_red);
    // 做颜色映射
    for (int i=0; i<im.h; i++) {
        speed = i * stride;
        for (int j=0; j<im.w; j++) {
            im.data[speed] = blue_map[im.data[speed]];
            im.data[speed+1] = green_map[im.data[speed+1]];
            im.data[speed+2] = red_map[im.data[speed+2]];
            speed += 3;
        }
        speed += line_pad;
    }

    cv::imwrite("auto_colorscale.jpg", mat);

    return 0;
}