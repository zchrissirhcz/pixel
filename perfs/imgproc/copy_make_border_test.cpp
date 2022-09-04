
#include <opencv2/opencv.hpp>
#include <iostream>

#include "copy_make_border.h"
#include "improc_zcx.h"

#include "common/pixel_benchmark.h"

int main_old()
{
    bool print_mat = false;

#if 1
    print_mat = true;

    cv::Size src_size;
    src_size.height = 5;
    src_size.width = 5;
    srand(7767517);

    unsigned char* src_shadow = (unsigned char*)malloc(sizeof(unsigned char) * src_size.height * src_size.width);
    for (int i=0; i<src_size.height; i++) {
        for (int j=0; j<src_size.width; j++) {
            // int t = rand() % 9;
            // src_shadow[i*src_size.width + j] = t;

            for (int k=0; k<3; k++) {
                int t = rand() % 9;
                src_shadow[i*src_size.width*3 + j*3 + k] = t;
            }
        }
    }

    cv::Mat src_image(src_size, CV_8UC3, src_shadow);
#else
    cv::Mat src_image = cv::imread("IU.bmp");
    cv::cvtColor(src_image, src_image, cv::COLOR_BGR2GRAY);
    cv::Size src_size = src_image.size();
#endif

    // int border = 4;
    // int top = border;
    // int bottom = border;
    // int left = border;
    // int right = border;

    int top = 2;
    int bottom = 2;
    int left = 7;
    int right = 2;

    int src_height = src_size.height;
    int src_width = src_size.width;

    cv::Size dst_size;
    dst_size.height = src_size.height + top + bottom;
    dst_size.width = src_size.width + left + right;

    cv::Mat dst_image_opencv(dst_size, src_image.type());
    cv::Mat dst_image_naive(dst_size, src_image.type());

    //cv::copyMakeBorder(src_image, dst_image_opencv, top, bottom, left, right, cv::BORDER_CONSTANT);
    cv::copyMakeBorder(src_image, dst_image_opencv, top, bottom, left, right, cv::BORDER_REPLICATE);

    //-----------------------------

    int channels = src_image.channels();
    unsigned char* dst = dst_image_naive.data;
    unsigned char* src = src_image.data;

    //copy_make_border2_naive(src, src_height, src_width, channels, dst, top, bottom, left, right, kBorderConstant);
    copy_make_border2_naive(src, src_height, src_width, channels, dst, top, bottom, left, right, kBorderReplicate);
    
    if (print_mat) {
        std::cout << "--- src_image: " << std::endl;
        std::cout << src_image << std::endl;

        std::cout << "--- dst_image_opencv: " << std::endl;
        std::cout << dst_image_opencv << std::endl;

        std::cout << "--- dst_image_naive: " << std::endl;
        std::cout << dst_image_naive << std::endl;
    
        std::cout << "-----------------------" << std::endl;
    }

    cv::Mat diff;
    cv::absdiff(dst_image_opencv, dst_image_naive, diff);
    cv::Scalar diff_scalar = cv::sum(diff);
    int total_diff = diff_scalar[0] + diff_scalar[1] + diff_scalar[2] + diff_scalar[3];
    
    std::cout << "total_diff is " << total_diff << std::endl;
    if (print_mat) {
        if (total_diff != 0 && print_mat) {
            std::cout << "--- dst_opencv - dst_zz " << std::endl;
            std::cout << diff << std::endl;
        }
    }

    cv::imwrite("IU_withborder_opencv.png", dst_image_opencv);
    cv::imwrite("IU_withborder_naive.png", dst_image_naive);

    return 0;
}

int main()
{
    cv::Mat src_image = cv::imread("IU.bmp");
    
    cv::Size size = src_image.size();
    int src_height = size.height;
    int src_width = size.width;
    
    int pad_top = 2;
    int pad_bottom = 2;
    int pad_left = 2;
    int pad_right = 2;
    int pad[4] = {
        pad_left, pad_top, pad_right, pad_bottom
    };

    int dst_height = src_height + pad_top + pad_bottom;
    int dst_width = src_width + pad_left + pad_right;

    cv::Size dst_size;
    dst_size.height = dst_height;
    dst_size.width = dst_width;
    cv::Mat dst_zcx(dst_size, src_image.type());
    cv::Mat dst_opencv(dst_size, src_image.type());

    double t_start, t_cost;
    
    t_start = pixel_get_current_time();
    cv_image_padding(src_image.data, src_height, src_width, pad, dst_zcx.data);
    t_cost = pixel_get_current_time() - t_start;
    printf("cv_image_padding cost %.4lf ms\n", t_cost);

    t_start = pixel_get_current_time();
    cv::copyMakeBorder(src_image, dst_opencv, pad_top, pad_bottom, pad_left, pad_right, cv::BORDER_REFLECT101);
    t_cost = pixel_get_current_time() - t_start;
    printf("cv_image_padding cost %.4lf ms\n", t_cost);

    cv::imwrite("result_zcx.png", dst_zcx);
    cv::imwrite("result_opencv.png", dst_opencv);

    return 0;
}