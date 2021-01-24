#include "pixel_bmp.h"
#include "pixel_log.h"
#include <opencv2/opencv.hpp>

void test_decode_bmp()
{
    // gray image (bpp=8, using palette to fill color)
    {
        const char* filename = "ring.bmp";

        _PxlImageFormat target_fmt = _PxlImage_BGR;
        unsigned char* buffer = NULL;
        int height;
        int width;
        int channel;
        _pxl_decode_bmp(filename, 1, &height, &width, &channel, &buffer, false);
        PIXEL_LOGD("height:%d, width:%d, channel:%d", height, width, channel);

        cv::Size size;
        size.height = height;
        size.width = width;
        cv::Mat mat(size, CV_8UC1, buffer);
        cv::imshow("image", mat);
        cv::waitKey(0);
        free(buffer);
    }

    // bpp=3, BGR image, not aligned
    {
        const char* filename = "mingren.bmp";

        _PxlImageFormat target_fmt = _PxlImage_BGR;
        unsigned char* buffer = NULL;
        int height;
        int width;
        int channel;
        int align = 1;
        _pxl_decode_bmp(filename, align, &height, &width, &channel, &buffer, false);
        PIXEL_LOGD("height:%d, width:%d, channel:%d", height, width, channel);

        cv::Size size;
        size.height = height;
        size.width = width;
        cv::Mat mat(size, CV_8UC3, buffer);
        cv::imshow("image", mat);
        cv::waitKey(0);
        free(buffer);
    }

    // bpp=3, BGR image, aligned
    {
        const char* filename = "mingren.bmp";

        _PxlImageFormat target_fmt = _PxlImage_BGR;
        unsigned char* buffer = NULL;
        int height;
        int width;
        int channel;
        int align = 4;
        _pxl_decode_bmp(filename, align, &height, &width, &channel, &buffer, false);
        PIXEL_LOGD("height:%d, width:%d, channel:%d", height, width, channel);

        cv::Size size;
        size.height = height;
        size.width = width;
        int step = align_up(width*channel, align); //!! important
        cv::Mat mat(size, CV_8UC3, buffer, step);
        cv::imshow("image", mat);
        cv::waitKey(0);
        free(buffer);
    }
}

void test_encode_bmp()
{
    //TODO
}

int main(){

    //test_decode_bmp();

    test_encode_bmp();

    return 0;
}