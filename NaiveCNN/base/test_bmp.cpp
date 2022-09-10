#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "nc_image.h"

int main__(){
    NcImage* im = nc_make_empty_image();
    const char* im_pth = "/Users/chris/work/gitee/naiveimg/hh/src/image.bmp";
    nc_image_load_bmp(im_pth, &im->data, &im->h, &im->w, 3, false);
    printf("im->h=%d, im->w=%d\n", im->h, im->w);

    cv::Mat cv_im(cv::Size(im->h, im->w), CV_8UC3);
    cv_im.data = im->data;

    cv::imshow("image", cv_im);

    cv::waitKey();

    return 0;
}

int main(){
    NcImage* im = nc_make_image_zero(1024, 1024, 3);
    printf("im->h=%d, im->w=%d\n", im->h, im->w);
    uchar* buf = im->data;
    for(int h=0; h<im->h; h++){
        for(int w=0; w<im->w; w++) {
            *buf++ = 255;
            *buf++ = (uchar)(w*1.0/im->w*255);
            *buf++ = (uchar)(h*1.0/im->h*255);
        }
    }

    nc_image_save_bmp("testq.bmp", im->data, im->h, im->w, im->c);

    free(im->data);
    free(im);

    return 0;
}
