
#include <opencv2/opencv.hpp>


#include "px_image.h"
#include "improc_zcx.h"
#include <iostream>

#include "px_opencv_adapter.hpp"


// copy_make_border_naive( ) 仅支持单通道图像的补边、且只补0；用来形象的说明补边的基本原理
// 对于多通道图像、多种类型补边策略的支持，请用 copy_make_border2_naive( )
void copy_make_border_naive(unsigned char* src, int src_height, int src_width, int channels, unsigned char* dst, int top, int bottom, int left, int right)
{
    int dst_height = src_height + top + bottom;
    int dst_width = src_width + left + right;

    int src_linebytes = src_width * channels * sizeof(unsigned char);
    int dst_linebytes = dst_width * channels * sizeof(unsigned char);
    for (int i=0; i<top; i++)
    {
        memset(dst+i*dst_linebytes, 0, dst_linebytes);
    }

    int left_bytes = left * sizeof(unsigned char) * channels;
    int right_bytes = right * sizeof(unsigned char) * channels;
    for (int i=0; i<src_height; i++)
    {
        int dst_i = i+top;
        memset(dst+dst_i*dst_linebytes, 0, left_bytes);

        int src_i = i;
        memcpy(dst+dst_i*dst_linebytes+left_bytes, src+src_i*src_linebytes, src_linebytes);

        memset(dst+dst_i*dst_linebytes + left_bytes + src_linebytes, 0, right_bytes);
    }

    for (int i=0; i<bottom; i++)
    {
        // for (int j=0; j<dst_width; j++){
        // }
        int dst_i = i + top + src_height;
        memset(dst+dst_i*dst_linebytes, 0, dst_linebytes);
    }
}

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

    px_image_t* px_src_image = px_create_image_header(src_height, src_width, channels);
    px_src_image->data = src_image.data;

    px_image_t* px_dst_image = px_create_image_header(dst_size.height, dst_size.width, channels);
    px_dst_image->data = dst_image_naive.data;

    px_pad_t pad;
    pad.top = top;
    pad.bottom = bottom;
    pad.left = left;
    pad.right = right;

    //copy_make_border2_naive(src, src_height, src_width, channels, dst, top, bottom, left, right, PX_BORDER_CONSTANT);
    //px_copy_make_border(src, src_height, src_width, channels, dst, top, bottom, left, right, PX_BORDER_REPLICATE);
    px_copy_make_border(px_src_image, px_dst_image, pad, PX_BORDER_REPLICATE);
    
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

int test_with_opencv()
{
    cv::Mat src_image = cv::imread("IU.bmp");
    
    cv::Size size = src_image.size();
    int src_height = size.height;
    int src_width = size.width;
    
    px_pad_t pad;
    pad.top = 2;
    pad.bottom = 2;
    pad.left = 2;
    pad.right = 2;

    int dst_height = src_height + pad.top + pad.bottom;
    int dst_width = src_width + pad.left + pad.right;

    cv::Size dst_size;
    dst_size.height = dst_height;
    dst_size.width = dst_width;
    cv::Mat dst_zcx(dst_size, src_image.type());
    cv::Mat dst_opencv(dst_size, src_image.type());

    px_image_t* px_src = px_image_from_opencv(src_image);
    px_image_t* px_dst = px_image_from_opencv(dst_zcx);
    px_copy_make_border(px_src, px_dst, pad, PX_BORDER_REFLECT101);
    cv::copyMakeBorder(src_image, dst_opencv, pad.top, pad.bottom, pad.left, pad.right, cv::BORDER_REFLECT101);

    px_destroy_image_header(px_src);
    px_destroy_image_header(px_dst);

    return 0;
}