/*
 * 访问图像像素信息方式的优化
 *
 * 原文地址：https://www.cnblogs.com/Imageshop/archive/2011/11/12/2246362.html
 *
 * 图像像素遍历，不同实现方式会有不同的效率。
 * 以图像反色算法为例，对比多种像素访问的实现方式，展示基本的优化技巧。

 * 这里采用C++重新实现，并在Ubuntu 20.04下用clang++ 10.0进行编译和测试。
 * 用到了OpenCV做图像读写，用到了C++11的时间戳实现跨平台的时间开销统计。
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>

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

void reverse_color1(RGBImage* im)
{
    int bpp = 3; // byte per pixel (channels)
    int stride = im->w * bpp;
    for(int i=0; i<im->h; i++) {
        for(int j=0; j<im->w; j++) {
            im->data[i*stride+j*bpp] = 255 - im->data[i*stride+j*bpp];
            im->data[i*stride+j*bpp+1] = 255 - im->data[i*stride+j*bpp+1];
            im->data[i*stride+j*bpp+2] = 255 - im->data[i*stride+j*bpp+2];
        }
    }
}

void reverse_color2(RGBImage* im)
{
    int bpp = 3;
    int stride = im->w * bpp;
    for(int i=0; i<im->h; i++) {
        for(int j=0; j<im->w; j++) {
            int speed = i*stride+j*bpp;
            im->data[speed] = 255 - im->data[speed];
            im->data[speed+1] = 255 - im->data[speed+1];
            im->data[speed+2] = 255 - im->data[speed+2];
        }
    }
}

void reverse_color3_bad(RGBImage* im)
{
    int bpp = 3;
    int stride = im->w * bpp;
    for(int i=0; i<im->h; i++) {
        int speed = i*stride;
        for(int j=0; j<im->w; j++) {
            im->data[speed] = 255 - im->data[speed];
            im->data[speed+1] = 255 - im->data[speed+1];
            im->data[speed+2] = 255 - im->data[speed+2];
            speed += bpp;
        }
    }
}

void reverse_color3(RGBImage* im)
{
    int bpp = 3;
    int linebytes = im->w * bpp; //有时候可能是这个数字再向上取4/8/16倍数
    int line_pad = linebytes - im->w * bpp;
    int speed = 0; //!! imageshop 忘了这个
    for(int i=0; i<im->h; i++) {
        for(int j=0; j<im->w; j++) {
            im->data[speed] = 255 - im->data[speed];
            im->data[speed+1] = 255 - im->data[speed+1];
            im->data[speed+2] = 255 - im->data[speed+2];
            speed += bpp;
        }
        speed += line_pad;
    }
}

int main()
{
    std::string im_pth = "bg.jpg";
    cv::Mat mat = cv::imread(im_pth);

    RGBImage im;
    im.h = mat.rows;
    im.w = mat.cols;
    im.data = mat.data;

    RGBImage im1 = create_image(im.h, im.w);
    copy_image_buffer(&im, &im1);

    RGBImage im2 = create_image(im.h, im.w);
    copy_image_buffer(&im, &im2);

    RGBImage im3 = create_image(im.h, im.w);
    copy_image_buffer(&im, &im3);

    const auto t0 = std::chrono::steady_clock::now();
    reverse_color1(&im1);
    const auto t1 = std::chrono::steady_clock::now();

    reverse_color2(&im2);
    const auto t2 = std::chrono::steady_clock::now();

    reverse_color3(&im3);
    const auto t3 = std::chrono::steady_clock::now();

    std::cout << "reverse_color: " << toString(t1-t0) << std::endl;
    std::cout << "reverse_color: " << toString(t2-t1) << std::endl;
    std::cout << "reverse_color: " << toString(t3-t2) << std::endl;

    cv::Mat res1(im.h, im.w, CV_8UC3, im1.data);
    cv::imwrite("res1.jpg", res1);

    cv::Mat res2(im.h, im.w, CV_8UC3, im2.data);
    cv::imwrite("res2.jpg", res2);

    cv::Mat res3(im.h, im.w, CV_8UC3, im3.data);
    cv::imwrite("res3.jpg", res3);

    free(im1.data);
    free(im2.data);
    free(im3.data);

    return 0;
}
