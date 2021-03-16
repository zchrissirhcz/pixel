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
    cv::Mat image = cv::imread("sky.png");
    cv::Size size = image.size();
    size_t height = size.height;
    size_t width = size.width;
    cv::Mat gray(size, CV_8UC1);
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::Mat result_naive(size, CV_8UC1);
    cv::Mat result_neighborline(size, CV_8UC1);
    cv::Mat result_neighborline_asimd(size, CV_8UC1);
    cv::Mat result_opencv(size, CV_8UC1);

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

    // neighbor line
    src_buf = gray.data;
    dst_buf = result_neighborline.data;
    t_start = pixel_get_current_time();
    matrix_column_max_u8_neighborline(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max u8, neighborline, time cost %4lf ms\n", t_cost);

    // neighbor line asimd
    src_buf = gray.data;
    dst_buf = result_neighborline_asimd.data;
    t_start = pixel_get_current_time();
    matrix_column_max_u8_neighborline_asimd(src_buf, height, width, dst_buf);
    t_cost = pixel_get_current_time() - t_start;
    PIXEL_LOGD("matrix column max u8, neighborline asimd, time cost %4lf ms\n", t_cost);

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
        if ( (result_naive.data[i] != result_opencv.data[i]) ||
             (result_naive.data[i] != result_neighborline.data[i]) ||
             (result_naive.data[i] != result_neighborline_asimd.data[i])
            ) {
            mis_count++;
        }
    }
    printf("mis_count=%zu\n", mis_count);


    // TODO: opencv里怎么对二维数组逐列求最小/最大？ 似乎是minMaxLoc和minMaxIdx，但是没看出区别来
    // https://blog.csdn.net/zhangqinghao0811/article/details/81538173
    // https://stackoverflow.com/questions/46024744/how-find-index-of-max-in-each-row-of-opencv-mat-boject
    //
    //
    //uchar data[25];
    //for (int i = 0; i < 25; i++) {
    //    data[i] = i;
    //}
    //cv::Mat foo1(5, 5, CV_8UC1, data);

    //double minVal, maxVal;
    //int minIdx[2];
    //int maxIdx[2];
    //cv::minMaxIdx(foo1, &minVal, &maxVal, minIdx, maxIdx);

}


void process_original_C(uint8_t* data, uint8_t* mask, int width, int height)
{
    for (int i = 0; i < width; i++) 
    {
        int max_chl = 0;
        int max_val = -1;
        for (int j = 0; j < height; j++) 
        {
            int idx = i + j * width;
            if (data[idx] >= max_val) 
            {
                max_val = data[idx];
                max_chl = j;
            }
        }
        mask[i] = max_chl;
    }
}


void process_original_Neon(uint8_t* data, uint8_t* mask, int width, int height) 
{
    int width_NN = width >> 4;
    
    uint8x16_t data_c0_mask = vdupq_n_u8(0);
    uint8x16_t data_c1_mask = vdupq_n_u8(1);
    uint8x16_t data_c2_mask = vdupq_n_u8(2);
    uint8x16_t data_c3_mask = vdupq_n_u8(3);
    uint8x16_t data_c4_mask = vdupq_n_u8(4);
    uint8x16_t data_c5_mask = vdupq_n_u8(5);
    uint8x16_t data_c6_mask = vdupq_n_u8(6);
    uint8x16_t data_c7_mask = vdupq_n_u8(7);
    uint8x16_t data_c8_mask = vdupq_n_u8(8);
    
    for (int i = 0; i < width_NN; i++) 
    {
        int max_chl = 0;
        int max_val = -1;
        
        uint8_t*  data_c0 = data + i*16;
        uint8_t*  data_c1 = data_c0 + width;
        uint8_t*  data_c2 = data_c1 + width;
        uint8_t*  data_c3 = data_c2 + width;
        uint8_t*  data_c4 = data_c3 + width;
        uint8_t*  data_c5 = data_c4 + width;
        uint8_t*  data_c6 = data_c5 + width;
        uint8_t*  data_c7 = data_c6 + width;
        uint8_t*  data_c8 = data_c7 + width;
        
        uint8_t* mask_ = mask + i*16;
        
        uint8x16_t data_c0_V = vld1q_u8(data_c0);
        uint8x16_t data_c1_V = vld1q_u8(data_c1);
        uint8x16_t data_c2_V = vld1q_u8(data_c2);
        uint8x16_t data_c3_V = vld1q_u8(data_c3);
        uint8x16_t data_c4_V = vld1q_u8(data_c4);
        uint8x16_t data_c5_V = vld1q_u8(data_c5);
        uint8x16_t data_c6_V = vld1q_u8(data_c6);
        uint8x16_t data_c7_V = vld1q_u8(data_c7);
        uint8x16_t data_c8_V = vld1q_u8(data_c8);


        uint8x16_t V_tmp = vmaxq_u8(data_c0_V, data_c1_V);
        uint8x16_t V_result_mask = vcgtq_u8(data_c0_V, data_c1_V);
        uint8x16_t V_result = vbslq_u8(V_result_mask, data_c0_mask, data_c1_mask);
    
        V_result_mask = vcgtq_u8(V_tmp, data_c2_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c2_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c2_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c3_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c3_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c3_V);
        
        V_result_mask = vcgtq_u8(V_tmp, data_c4_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c4_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c4_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c5_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c5_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c5_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c6_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c6_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c6_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c7_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c7_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c7_V);

        V_result_mask = vcgtq_u8(V_tmp, data_c8_V);
        V_result = vbslq_u8(V_result_mask, V_result, data_c8_mask);
        V_tmp = vmaxq_u8(V_tmp, data_c8_V);

        vst1q_u8(mask_, V_result);
        
    }

}


static void ld_test()
{
    //srand(time(0));
    double t_start, t_cost;
    
    //int m = 9; //out_channels
    int height = 9;

    int width = 49152; //num_elem
    //int n = 48; //num_elem

    size_t buf_size = width * sizeof(uint8_t);

    //mask_OutData
    uint8_t* data = (uint8_t*)malloc(height*width);
    for (int i = 0; i < height*width; i++) {
        data[i] = (uint8_t)(rand()%127);
    }
    uint8_t* mask_c = (uint8_t*)malloc(width);
    uint8_t* mask_neon = (uint8_t*)malloc(width);
    memset(mask_c, 0, width);
    memset(mask_neon, 0, width);
    
    //-------------------------------------------
    // naive C impl
    //-------------------------------------------
    {
        t_start = pixel_get_current_time();
        process_original_C(data, mask_c, width, height);
        t_cost = pixel_get_current_time() - t_start;
        printf("c time: %.4lf \n", t_cost);
    }
    
    //-------------------------------------------
    // naive Neon impl
    //-------------------------------------------	
    {
        t_start = pixel_get_current_time();
        process_original_Neon(data, mask_neon, width, height);
        t_cost = pixel_get_current_time() - t_start;
        printf("neon time: %.4lf \n", t_cost);
    }
    
    int error = 0;
    
    printf(".......................inputdata........................\n");
    
    #if 1
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            printf("%d,  ", data[j + i*width]);
        }
        
        printf("\n");
        
    }
    
    printf("\n");
    #endif
    
    
    
    for(int i =0; i < width; i++)
    {
        if(mask_c[i]!=mask_neon[i])
        {
            error++;

            printf("c: %d, neon: %d    i: %d \n", mask_c[i], mask_neon[i], i);
        }
        
    }
    
    printf("error number %d  \n", error);

}



int main() {
    //matrix_column_max_test();
    ld_test();

    return 0;
}