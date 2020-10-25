/*
 * 图像处理界双线性插值算法的优化
 *
 * 原文地址：https://www.cnblogs.com/Imageshop/archive/2011/11/12/2246808.html
 * 
 * 这里采用C++重新实现，并在Ubuntu 20.04下用clang++ 10.0进行编译和测试。
 * 用到了OpenCV做图像读写，用到了C++11的时间戳实现跨平台的时间开销统计。
 * 
 * 优化手段：
 * 1  找了一个参考实现，以及自行按照公式进行实现。
 * 2. 索引计算尽量提前，从内层循环放到外层循环
 * 3. 避免重复计算：算出A的索引后，B、C、D索引用加法得到，而不是分别用乘法和加法算出
 *         A    B
 *         C    D
 * 4. 用定点化的方法替代浮点运算
 * 5. 用移位操作替代除法操作
 * 
 * 这里有一个相关的优化问题：对于放大倍数特别多的（比如12倍），直接用bilinear resize效果并不好，迭代的使用则效果好，但速度也会变慢。能否进行优化？
 * 
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

//https://github.com/17343149/bilinearInterpolation
RGBImage bilinear_resize1(RGBImage* src, int dst_width, int dst_height)
{
    int src_height = src->h;
    int src_width = src->w;
    RGBImage dst = create_image(dst_width, dst_height);
    int channels = 3;
    int src_stride = src_width * channels;
    int dst_stride = dst_width * channels;
    for(int i=0; i<dst_height; i++) {
        for(int j=0; j<dst_width; j++) {
            double src_j = (j+0.5) * ((double)src_width/dst_width) - 0.5;
            double src_i = (i+0.5) * ((double)src_height/dst_height) - 0.5;
            int arr_j[2] = {(int)src_j, (int)(src_j+1)}; // {}
            int arr_i[2] = {(int)src_i, (int)(src_i+1)};
            
            for(int k=0; k<channels; k++) {
                // 对x方向进行线性插值
                int src_idx = src_i*src_stride + src_j*channels + k;
                double val_up = {
                    (src_j - arr_j[0]) * src->data[arr_i[0]*src_stride + arr_j[0]*channels + k] +
                        (arr_j[1] - src_j) * src->data[arr_i[0]*src_stride +  arr_j[1]*channels + k]
                };
                double val_down = {
                    (src_j - arr_j[0]) * src->data[arr_i[1]*src_stride + arr_j[0]*channels + k] +
                        (arr_j[1] - src_j) * src->data[arr_i[1]*src_stride + arr_j[1]*channels + k]
                };

                // 对y方向进行线性插值
                int val = nearbyint((src_i-arr_i[0]) * val_up + (arr_i[1] - src_i) * val_down);

                int dst_idx = i*dst_stride + j*channels + k;
                //printf("--- dst_idx = %d\n", dst_idx);
                dst.data[i*dst_stride + j*channels + k] = val;
            }
        }
    }
    return dst;
}

// 根据公式实现的。
RGBImage bilinear_resize2(RGBImage* src, int dst_width, int dst_height)
{
    int src_height = src->h;
    int src_width = src->w;
    RGBImage dst = create_image(dst_width, dst_height);
    int channels = 3;
    int src_stride = src_width * channels;
    int dst_stride = dst_width * channels;

    for (int i=0; i<dst_height; i++) {
        for (int j=0; j<dst_width; j++) {
            float src_i = (i+0.5) * ((float)src_height/dst_height) - 0.5;
            float src_j = (j+0.5) * ((float)src_width/dst_width) - 0.5;
            
            float y = src_i - (int)src_i;
            float x = src_j - (int)src_j; 

            int Ai = (int)src_i;
            int Aj = (int)src_j;

            int Bi = (int)src_i;
            int Bj = (int)(src_j+0.5);

            int Ci = (int)(src_i+0.5);
            int Cj = (int)src_j;

            int Di = (int)(src_i+0.5);
            int Dj = (int)(src_j+0.5);

            for (int k=0; k<channels; k++) {
                float A = src->data[Ai*src_stride+Aj*channels+k];
                float B = src->data[Bi*src_stride+Bj*channels+k];
                float C = src->data[Ci*src_stride+Cj*channels+k];
                float D = src->data[Di*src_stride+Dj*channels+k];

                float M = (1-x)*(1-y)*A + x*(1-y)*B + y*(1-x)*C + x*y*D;
                //float M = (1-y)*((1-x)*A + x*B) + y*((1-x)*C + x*D);
                int dst_idx = i*dst_stride+j*channels+k;
                dst.data[dst_idx] = (unsigned char)M;
            }
        }
    }
    return dst;
}


// 根据公式实现的。
RGBImage bilinear_resize3(RGBImage* src, int dst_width, int dst_height)
{
    int src_height = src->h;
    int src_width = src->w;
    RGBImage dst = create_image(dst_width, dst_height);
    int channels = 3;
    int src_stride = src_width * channels;
    int dst_stride = dst_width * channels;

    float scale_h = (float)src_height / dst_height;
    float scale_w = (float)src_width / dst_width;

    float src_i, src_j, y, x, invx, invy, x_invy, y_invx, x_y, invx_invy;
    int Ai, Aj;

    int dst_index = 0;
    int dst_line_pad = dst_stride - dst_width*channels;
    for (int i=0; i<dst_height; i++) {
        src_i = (i+0.5) * scale_h - 0.5;
        y = src_i - (int)src_i;
        invy = 1 - y;
        Ai = (int)src_i;
        for (int j=0; j<dst_width; j++) {
            src_j = (j+0.5) * scale_w - 0.5;
            x = src_j - (int)src_j; 
            invx = 1 - x;

            x_invy = x * invy;
            y_invx = y * invx;
            x_y = x * y;
            invx_invy = invx * invy;
            
            Aj = (int)src_j;
            int index_A_base = Ai*src_stride+Aj*channels;
            int index_B_base = index_A_base + channels;
            int index_C_base = index_A_base + src_stride;
            int index_D_base = index_C_base + channels;

            for (int k=0; k<channels; k++) {
                float A = src->data[index_A_base+k];
                float B = src->data[index_B_base+k];
                float C = src->data[index_C_base+k];
                float D = src->data[index_D_base+k];

                //float M = (1-x)*(1-y)*A + x*(1-y)*B + y*(1-x)*C + x*y*D;
                float M = invx_invy*A + x_invy*B + y*invx*C + x_y*D;
                //float M = (1-y)*((1-x)*A + x*B) + y*((1-x)*C + x*D);
                //int dst_idx = i*dst_stride+j*channels+k;
                dst.data[dst_index+k] = (unsigned char)M;
            }
            dst_index += channels;
        }
        dst_index += dst_line_pad;
    }
    return dst;
}

// 根据公式实现的。按imageshop思路重新写一下
RGBImage bilinear_resize4(RGBImage* src, int dst_width, int dst_height)
{
    int src_height = src->h;
    int src_width = src->w;
    RGBImage dst = create_image(dst_width, dst_height);
    int channels = 3;
    int src_stride = src_width * channels;
    int dst_stride = dst_width * channels;

    float scale_h = (float)src_height/dst_height;
    float scale_w = (float)src_width/dst_width;

    int fix = 1024; // 2^11
    //int fix_inv = 4194304; //2^22
    int fix_inv = fix * fix;

    int Speed = 0;
    int line_pad = dst_stride - dst_width*3;
    for (int i=0; i<dst_height; i++) {
        for (int j=0; j<dst_width; j++) {
            float src_i = (i+0.5) * scale_h - 0.5;
            float src_j = (j+0.5) * scale_w - 0.5;

            int Ai = (int)src_i;
            int Aj = (int)src_j;

            //int NewX = (int)src_j;
            //int NewY = (int)src_i;
            int PartX = (src_j - Aj) * fix;
            int PartY = (src_i - Ai) * fix;
            int InvX = fix - PartX;
            int InvY = fix - PartY;

            int index_A = src_stride * Ai + Aj * 3;
            int index_C = index_A + src_stride;
            int index_B = index_A + 3;
            int index_D = index_C + 3;

            dst.data[Speed + 2] = (
                    (src->data[index_A + 2] * InvX + src->data[index_B + 2] * PartX) * InvY + 
                    (src->data[index_C + 2] * InvX + src->data[index_D + 2] * PartX) * PartY
                ) / fix_inv;       //处理红色分量
            dst.data[Speed + 1] = (
                    (src->data[index_A + 1] * InvX + src->data[index_B + 1] * PartX) * InvY + 
                    (src->data[index_C + 1] * InvX + src->data[index_D + 1] * PartX) * PartY
                ) / fix_inv;       //处理绿色分量
            dst.data[Speed] = (
                    (src->data[index_A] * InvX + src->data[index_B] * PartX) * InvY + 
                    (src->data[index_C] * InvX + src->data[index_D] * PartX) * PartY
                ) / fix_inv;           //处理蓝色分量


            Speed += 3;
        }
        Speed += line_pad;
    }
    return dst;
}

// 根据公式实现的。按imageshop思路重新写一下。定点化操作用移位操作替代，内层for循环索引提前到外层for循环
RGBImage bilinear_resize5(RGBImage* src, int dst_width, int dst_height)
{
    int src_height = src->h;
    int src_width = src->w;
    RGBImage dst = create_image(dst_width, dst_height);
    int channels = 3;
    int src_stride = src_width * channels;
    int dst_stride = dst_width * channels;

    float scale_h = (float)src_height/dst_height;
    float scale_w = (float)src_width/dst_width;

    int shift = 10;
    int fix = (1<<shift); // 2^11
    //int fix_inv = 4194304; //2^22
    int fix_inv = 2*shift;

    int Speed = 0;
    int line_pad = dst_stride - dst_width*3;
    for (int i=0; i<dst_height; i++) {
        float src_i = (i+0.5f) * scale_h - 0.5f;
        int Ai = (int)src_i;
        int PartY = (src_i - Ai) * fix;
        int InvY = fix - PartY;
        int index_A_base = src_stride * Ai;
        for (int j=0; j<dst_width; j++) {
            float src_j = (j+0.5f) * scale_w - 0.5f;
            int Aj = (int)src_j;
            int PartX = (src_j - Aj) * fix;
            int InvX = fix - PartX;

            //int index_A = src_stride * Ai + Aj * 3;
            int index_A = index_A_base + Aj*3;
            int index_C = index_A + src_stride;
            int index_B = index_A + 3;
            int index_D = index_C + 3;

            dst.data[Speed + 2] = (
                    (src->data[index_A + 2] * InvX + src->data[index_B + 2] * PartX) * InvY + 
                    (src->data[index_C + 2] * InvX + src->data[index_D + 2] * PartX) * PartY
                ) >> fix_inv;       //处理红色分量
            dst.data[Speed + 1] = (
                    (src->data[index_A + 1] * InvX + src->data[index_B + 1] * PartX) * InvY + 
                    (src->data[index_C + 1] * InvX + src->data[index_D + 1] * PartX) * PartY
                ) >> fix_inv;       //处理绿色分量
            dst.data[Speed] = (
                    (src->data[index_A] * InvX + src->data[index_B] * PartX) * InvY + 
                    (src->data[index_C] * InvX + src->data[index_D] * PartX) * PartY
                ) >> fix_inv;       //处理蓝色分量

            Speed += 3;
        }
        Speed += line_pad;
    }
    return dst;
}

int main()
{
    std::string im_pth = "bg.jpg";
    //std::string im_pth = "mingren.jpg";
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

    RGBImage im4 = create_image(im.h, im.w);
    copy_image_buffer(&im, &im4);

    RGBImage im5 = create_image(im.h, im.w);
    copy_image_buffer(&im, &im5);

    float w_mult = 0.5;
    float h_mult = 0.5;
    int dst_width = (int)(im.w * w_mult);
    int dst_height = (int)(im.h * h_mult);

    const auto t0 = std::chrono::steady_clock::now();
    RGBImage im1_res = bilinear_resize1(&im1, dst_width, dst_height);
    const auto t1 = std::chrono::steady_clock::now();

    RGBImage im2_res = bilinear_resize2(&im2, dst_width, dst_height);
    const auto t2 = std::chrono::steady_clock::now();

    RGBImage im3_res = bilinear_resize3(&im3, dst_width, dst_height);
    const auto t3 = std::chrono::steady_clock::now();

    RGBImage im4_res = bilinear_resize4(&im4, dst_width, dst_height);
    const auto t4 = std::chrono::steady_clock::now();

    RGBImage im5_res = bilinear_resize5(&im5, dst_width, dst_height);
    const auto t5 = std::chrono::steady_clock::now();

    std::cout << "bilinear_resize1: " << toString(t1-t0) << std::endl;
    std::cout << "bilinear_resize2: " << toString(t2-t1) << std::endl;
    std::cout << "bilinear_resize3: " << toString(t3-t2) << std::endl;
    std::cout << "bilinear_resize4: " << toString(t4-t3) << std::endl;
    std::cout << "bilinear_resize5: " << toString(t5-t4) << std::endl;

    cv::Mat res1(dst_height, dst_width, CV_8UC3, im1_res.data);
    cv::imwrite("res1.jpg", res1);


    cv::Mat res2(dst_height, dst_width, CV_8UC3, im2_res.data);
    cv::imwrite("res2.jpg", res2);

    cv::Mat res3(dst_height, dst_width, CV_8UC3, im3_res.data);
    cv::imwrite("res3.jpg", res3);

    cv::Mat res4(dst_height, dst_width, CV_8UC3, im4_res.data);
    cv::imwrite("res4.jpg", res4);

    cv::Mat res5(dst_height, dst_width, CV_8UC3, im5_res.data);
    cv::imwrite("res5.jpg", res5);

    free(im1.data);
    free(im2.data);
    free(im3.data);
    free(im4.data);
    free(im5.data);

    return 0;
}
