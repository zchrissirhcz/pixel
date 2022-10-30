#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <opencv2/opencv.hpp>
#include "boxfilter.h"

#include "common/pixel_benchmark.h"

#include "matcalc/matrix_transpose.h"

void blur_1d(float* src, float* dst, int src_size, int kernel_len)
{
    assert(kernel_len>0);
    assert(src!=dst);

    int offset = kernel_len / 2;
    int norm = kernel_len * kernel_len;
    for (int i=0; i<src_size; i++) {
        float sum = 0;
        for (int j=0; j<kernel_len; j++) {
            int ti = (i-offset) + j;
            if (ti<0) {
                ti = -ti;
            }
            else if (ti> src_size -1) {
                ti = 2* src_size - 2 - ti; // size-1 - (ti-(size-1))  =>  size - 1 - (ti - size + 1) => size - 1 - ti + size - 1 => 2*size - 2 - ti
            }
            sum += src[ti];
        }
        dst[i] = sum / norm;
    }
}

#include <iostream>
#include <algorithm>

void BoxFilter(const cv::Mat& src, cv::Mat& dst, int ksize) {
    CV_Assert(src.channels() == 1 || src.channels() == 3); //只处理单通道或者三通道图像
    //生成一维的
    double* matrix = new double[ksize];
    double norm = 0;
    int origin = ksize / 2;
    for (int i = 0; i < ksize; i++) {
        double g = 1.0;
        norm += g;
        matrix[i] = g;
    }
    for (int i = 0; i < ksize; i++) matrix[i] /= norm;
    int border = ksize / 2;

    // step1 拷贝src到dst并且补边
    copyMakeBorder(src, dst, border, border, border, border, cv::BORDER_CONSTANT);
    int channels = dst.channels();
    int rows = dst.rows - border;
    int cols = dst.cols - border;

    // step2 水平方向boxfilter
    for (int i = border; i < rows; i++) {
        for (int j = border; j < cols; j++) {
            double sum[3] = { 0 };
            for (int k = -border; k <= border; k++) {
                if (channels == 1) {
                    sum[0] += matrix[border + k] * dst.at<uchar>(i, j + k);
                }
                else if (channels == 3) {
                    cv::Vec3b rgb = dst.at<cv::Vec3b>(i, j + k);
                    sum[0] += matrix[border + k] * rgb[0];
                    sum[1] += matrix[border + k] * rgb[1];
                    sum[2] += matrix[border + k] * rgb[2];
                }
            }
            for (int k = 0; k < channels; k++) {
                if (sum[k] < 0) sum[k] = 0;
                else if (sum[k] > 255) sum[k] = 255;
            }
            if (channels == 1)
                dst.at<cv::Vec3b>(i, j) = static_cast<uchar>(sum[0]);
            else if (channels == 3) {
                cv::Vec3b rgb = { static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2]) };
                dst.at<cv::Vec3b>(i, j) = rgb;
            }
        }
    }

    // step3 竖直方向boxfilter
    for (int i = border; i < rows; i++) {
        for (int j = border; j < cols; j++) {
            double sum[3] = { 0 };
            for (int k = -border; k <= border; k++) {
                if (channels == 1) {
                    sum[0] += matrix[border + k] * dst.at<uchar>(i + k, j);
                }
                else if (channels == 3) {
                    cv::Vec3b rgb = dst.at<cv::Vec3b>(i + k, j);
                    sum[0] += matrix[border + k] * rgb[0];
                    sum[1] += matrix[border + k] * rgb[1];
                    sum[2] += matrix[border + k] * rgb[2];
                }
            }
            for (int k = 0; k < channels; k++) {
                if (sum[k] < 0) sum[k] = 0;
                else if (sum[k] > 255) sum[k] = 255;
            }
            if (channels == 1)
                dst.at<cv::Vec3b>(i, j) = static_cast<uchar>(sum[0]);
            else if (channels == 3) {
                cv::Vec3b rgb = { static_cast<uchar>(sum[0]), static_cast<uchar>(sum[1]), static_cast<uchar>(sum[2]) };
                dst.at<cv::Vec3b>(i, j) = rgb;
            }
        }
    }
    delete[] matrix;
}

void boxfilter_xydir(unsigned char* src, unsigned char* dst, int height, int width, int channels, int kernel_h, int kernel_w, int anchor_y, int anchor_x, bool norm, BorderType border_type)
{
    int* pixel = (int*)malloc(sizeof(int)*channels);
    int linebytes = channels * width * sizeof(unsigned char);
    int line_elem = channels * width;

    int* bufmat = (int*)malloc(line_elem*height*sizeof(float));

    // 按行做boxfilter;
    // 仍然有计算冗余，相邻窗口位置重复计算了。。
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            memset(pixel, 0, sizeof(int)*channels);
            for (int kj=0; kj<kernel_w; kj++) {
                int tj = (j + kj - anchor_x);
                tj = border_clip(border_type, tj, width);
                for (int kc=0; kc<channels; kc++) {
                    pixel[kc] += src[i*line_elem + tj*channels + kc];
                }
            }
            for (int kc=0; kc<channels; kc++) {
                bufmat[i*line_elem+j*channels+kc] = pixel[kc];
            }
        }
    }

    // TODO: 这里有一个优化的思路，是把bufmat转置得到bufmat_t，然后从butmat_t算出dst
    // 但是，bufmat类型是int32，而int32类型的转置，还没有写过NEON优化
    // (bufmat用int32类型的原因：pixel存储的值可能超过255,防止溢出；最根本原因，还是希望能准确计算，确保和OpenCV结果相同)

    // 另一个优化思路：不用转置倒腾；利用缓存行来累加（存储）按列做boxfilter的结果
    // 单行缓存行做不到；缓存行的行数应该等于 kernel_h

    // 再另一个优化思路：用栈存储（其实是ring buffer）

    // 目前还是 naive 实现
    int kernel_size = kernel_h * kernel_w;
    int radius = kernel_size / 2;
    int denominator = kernel_size;
    for (int j=0; j<line_elem; j++) {
        for (int i=0; i<height; i++) {
            int sum = 0;
            for (int ki=0; ki<kernel_h; ki++) {
                int ti = i + ki - anchor_y;
                ti = border_clip(border_type, ti, height);
                sum += bufmat[ti*line_elem + j];
            }
            sum = (sum + radius) / denominator;
            dst[i*line_elem + j] = sum;
        }
    }

    free(bufmat);
}

/*
time cost
     naive     xydir      opencv
Mac  2700ms    1553ms     12 ms

*/

int main()
{
    double t_start, t_cost;

    bool print_mat = false;

    bool norm = true;

#if 0
    print_mat = true;

    int height = 20;
    int width = 10;

    srand(7767517);

    unsigned char* src_shadow = (unsigned char*)malloc(sizeof(unsigned char) * height * width);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int t = rand() % 30;
            //int t = 1;
            src_shadow[i*width+j] = t;
        }
    }
    cv::Size size;
    size.height = height;
    size.width = width;
    cv::Mat input(size, CV_8UC1, src_shadow);
#else
    cv::Mat input = cv::imread("sky.png");
    //cv::cvtColor(input, input, cv::COLOR_BGR2GRAY);
    cv::Size size = input.size();
    int height = size.height;
    int width = size.width;
#endif

    cv::Mat dst_opencv = input.clone();
    cv::Size kernel_size(5, 10);
    cv::Point anchor(2, 0);

    t_start = pixel_get_current_time();
    cv::boxFilter(input, dst_opencv, 8, kernel_size, anchor, norm, cv::BORDER_DEFAULT);
    t_cost = pixel_get_current_time() - t_start;
    printf("cv::boxFilter time cost %.4lf ms\n", t_cost);

    if (print_mat) {
        std::cout << "--- input is " << std::endl;
        std::cout << input << std::endl;

        std::cout << "--- dst_opencv is " << std::endl;
        std::cout << dst_opencv << std::endl;
    }

    //---------------------------------------

    unsigned char* src = input.data;
    cv::Mat dst_zz = input.clone();
    unsigned char* dst = dst_zz.data;

    int channels = input.channels();

    t_start = pixel_get_current_time();

    //boxfilter_naive(src, dst, height, width, channels, kernel_size.height, kernel_size.width, anchor.y, anchor.x, norm, PX_BORDER_DEFAULT);
    //t_cost = pixel_get_current_time() - t_start;
    //printf("boxfilter_naive() time cost %.4lf ms\n", t_cost);

    boxfilter_xydir(src, dst, height, width, channels, kernel_size.height, kernel_size.width, anchor.y, anchor.x, norm, PX_BORDER_DEFAULT);
    t_cost = pixel_get_current_time() - t_start;
    printf("boxfilter_xydir() time cost %.4lf ms\n", t_cost);

    if (print_mat) {
        std::cout << "--- dst_zz is " << std::endl;
        std::cout << dst_zz << std::endl;
    }

    cv::Mat diff;
    cv::absdiff(dst_opencv, dst_zz, diff);

    cv::Scalar diff_scalar = cv::sum(diff);
    int total_diff = diff_scalar[0] + diff_scalar[1] + diff_scalar[2] + diff_scalar[3];
    std::cout << "-----------------------" << std::endl;
    std::cout << "total_diff is " << total_diff << std::endl;
    if (total_diff != 0 && print_mat) {
        std::cout << "--- dst_opencv - dst_zz " << std::endl;
        std::cout << diff << std::endl;
    }

    cv::imwrite("sky_gray_boxfilter_opencv4.png", dst_opencv);
    cv::imwrite("sky_gray_boxfilter_zz.png", dst_zz);

    return 0;
}
