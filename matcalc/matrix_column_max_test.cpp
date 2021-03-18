#include "matrix_column_max.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <opencv2/opencv.hpp>

#include "common/pixel_benchmark.h"
#include "common/pixel_log.h"

static void matrix_column_max_test()
{
    //cv::Mat image = cv::imread("sky.png");
    //cv::Mat image = cv::imread("colorhouse-32x32.png");
    cv::Mat image = cv::imread("river_bank2.png");
    cv::Size size = image.size();
    size_t height = size.height;
    size_t width = size.width;
    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat result_naive = cv::Mat::zeros(1, width, CV_8UC1);
    cv::Mat result_cacheline = cv::Mat::zeros(1, width, CV_8UC1);
    cv::Mat result_cacheline_asimd = cv::Mat::zeros(1, width, CV_8UC1);
    cv::Mat result_opencv = cv::Mat::zeros(1, width, CV_8UC1);

    printf("image info: height=%zu, width=%zu\n", height, width);

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

    // cacheline
    src_buf = gray.data;
    dst_buf = result_cacheline.data;
    t_start = pixel_get_current_time();
    matrix_column_max_u8_cacheline(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max u8, cacheline, time cost %4lf ms\n", t_cost);

    // cacheline asimd
    src_buf = gray.data;
    dst_buf = result_cacheline_asimd.data;
    t_start = pixel_get_current_time();
    matrix_column_max_u8_cacheline_asimd(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max u8, cacheline asimd, time cost %4lf ms\n", t_cost);

    // opencv
    src_buf = gray.data;
    dst_buf = result_opencv.data;
    t_start = pixel_get_current_time();
    //cv::minMaxIdx(gray, )
    //(src_buf, height, width, dst_buf);
    // 对于逐列求矩阵最大值，由于cv::minMaxIdx是求一个Mat的全局最小和最大值（以及索引），因此需要逐列调用cv::minMaxIdx
    for (size_t i = 0; i < width; i++) {
        double minVal, maxVal;
        cv::minMaxIdx(gray.col(i), &minVal, &maxVal, NULL, NULL);
        dst_buf[i] = maxVal;
    }
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max u8, opencv, time cost %4lf ms\n", t_cost);

    // compare result, make sure same
    size_t mis_count = 0;
    for (size_t i = 0; i < width; i++) {
        if ( 
             (result_naive.data[i] != result_opencv.data[i]) ||
             (result_naive.data[i] != result_cacheline.data[i]) ||
             (result_naive.data[i] != result_cacheline_asimd.data[i])
            ) {
            mis_count++;
        }
    }
    printf("mis_count=%zu\n", mis_count);
}

static void matrix_column_max_and_idx_test()
{
    //cv::Mat image = cv::imread("sky.png");
    //cv::Mat image = cv::imread("colorhouse-32x32.png");
    cv::Mat image = cv::imread("river_bank2.png");
    cv::Size size = image.size();
    size_t height = size.height;
    size_t width = size.width;
    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat result_naive = cv::Mat::zeros(1, width, CV_8UC1);
    cv::Mat result_cacheline = cv::Mat::zeros(1, width, CV_8UC1);
    cv::Mat result_cacheline_asimd = cv::Mat::zeros(1, width, CV_8UC1);
    cv::Mat result_opencv = cv::Mat::zeros(1, width, CV_8UC1);

    printf("image info: height=%zu, width=%zu\n", height, width);

    //FILE* fout = fopen("dump-input32x32.txt", "w");
    //for (size_t i = 0; i < height; i++) {
    //    for (size_t j = 0; j < width; j++) {
    //        size_t idx = i * width + j;
    //        fprintf(fout, "%3d ", gray.data[idx]);
    //    }
    //    fprintf(fout, "\n");
    //}
    //fclose(fout);

    double t_start, t_cost;
    unsigned char* src_buf = NULL;
    unsigned char* dst_buf = NULL;
    size_t buf_size = width*sizeof(size_t);
    size_t* max_indicies_naive = (size_t*)malloc(buf_size);
    //memset(max_indicies_naive, 0, buf_size);
    for (size_t i = 0; i < width; i++) {
        max_indicies_naive[i] = 0;
    }

    size_t* max_indicies_cacheline = (size_t*)malloc(buf_size);
    //memset(max_indicies_cacheline, 0, buf_size);
    for (size_t i = 0; i < width; i++) {
        max_indicies_cacheline[i] = 0;
    }

    size_t* max_indicies_cacheline_asimd = (size_t*)malloc(buf_size);
    //memset(max_indicies_cacheline_asimd, 0, buf_size);
    for (size_t i = 0; i < width; i++) {
        max_indicies_cacheline_asimd[i] = 0;
    }

    size_t* max_indicies_opencv = (size_t*)malloc(buf_size);
    //memset(max_indicies_opencv, 0, buf_size);
    for (size_t i = 0; i < width; i++) {
        max_indicies_opencv[i] = 0;
    }

    // naive
    src_buf = gray.data;
    dst_buf = result_naive.data;
    t_start = pixel_get_current_time();
    matrix_column_max_and_idx_u8_naive(src_buf, height, width, dst_buf, max_indicies_naive);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max and idx u8, naive, time cost %4lf ms\n", t_cost);

    // cache line
    src_buf = gray.data;
    dst_buf = result_cacheline.data;
    t_start = pixel_get_current_time();
    matrix_column_max_and_idx_u8_cacheline(src_buf, height, width, dst_buf, max_indicies_cacheline);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max and idx u8, cacheline, time cost %4lf ms\n", t_cost);

    // cache line asimd
    src_buf = gray.data;
    dst_buf = result_cacheline_asimd.data;
    t_start = pixel_get_current_time();
    matrix_column_max_and_idx_u8_cacheline_asimd(src_buf, height, width, dst_buf, max_indicies_cacheline_asimd);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max and idx u8, cacheline asimd, time cost %4lf ms\n", t_cost);

    // opencv
    src_buf = gray.data;
    dst_buf = result_opencv.data;
    t_start = pixel_get_current_time();
    //cv::minMaxIdx(gray, )
    //(src_buf, height, width, dst_buf);
    // 对于逐列求矩阵最大值，由于cv::minMaxIdx是求一个Mat的全局最小和最大值（以及索引），因此需要逐列调用cv::minMaxIdx
    for (size_t i = 0; i < width; i++) {
        double minVal, maxVal;
        int minIdx[2];
        int maxIdx[2];
        cv::minMaxIdx(gray.col(i), &minVal, &maxVal, minIdx, maxIdx);
        dst_buf[i] = maxVal;
        max_indicies_opencv[i] = maxIdx[0];
    }
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max and idx u8, opencv, time cost %4lf ms\n", t_cost);

    // compare result, make sure same
    size_t mis_count = 0;
    size_t mis_idx_count = 0;
    for (size_t i = 0; i < width; i++) {
        if ( 
              (result_naive.data[i] != result_opencv.data[i]) ||
              (result_naive.data[i] != result_cacheline.data[i]) || 
              (result_naive.data[i] != result_cacheline_asimd.data[i])
            ) {
            printf("mis@%zu(naive=%d, cache=%d) ", i, result_naive.data[i], result_cacheline.data[i]);
            mis_count++;
        }
        if ( (max_indicies_naive[i] != max_indicies_opencv[i]) ||
             (max_indicies_naive[i] != max_indicies_cacheline[i]) ||
             (max_indicies_naive[i] != max_indicies_cacheline_asimd[i])
            ) {
            mis_idx_count++;
        }
    }
    printf("mis_count=%zu, mis_idx_count=%zu\n", mis_count, mis_idx_count);

    free(max_indicies_naive);
    free(max_indicies_cacheline);
    free(max_indicies_cacheline_asimd);
    free(max_indicies_opencv);
}



int main() {
    matrix_column_max_test();
    //matrix_column_max_and_idx_test();
    //ld_test();

    return 0;
}