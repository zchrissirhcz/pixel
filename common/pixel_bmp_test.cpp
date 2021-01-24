#include "pixel_bmp.h"
#include "pixel_log.h"
#include <opencv2/opencv.hpp>

void test_decode_bmp()
{
    if (1)
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

    if(1)
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
        int step = _pxl_align_up(width*channel, align); //!! important
        cv::Mat mat(size, CV_8UC3, buffer, step);
        cv::imshow("image", mat);
        cv::waitKey(0);
        free(buffer);
    }

    if(1)
    // gray image (bpp=8, using palette to fill color), not aligned
    {
        const char* filename = "ring.bmp";

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
        cv::Mat mat(size, CV_8UC1, buffer);
        cv::imshow("image", mat);
        cv::waitKey(0);
        free(buffer);
    }

    // gray image (bpp=8, using palette to fill color), align = 4
    {
        const char* filename = "mingren_gray.bmp";

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
        int step = _pxl_align_up(width*channel, align); //!! important
        cv::Mat mat(size, CV_8UC1, buffer, step);
        PIXEL_LOGD("outsize, buffer is %p", buffer);
        cv::imshow("image", mat);
        cv::waitKey(0);
        free(buffer);
    }
}

void test_encode_bmp()
{
    if (1)
    // bpp=3, no align, save image
    {
        const char* fn = "mingren.bmp";
        cv::Mat mat = cv::imread(fn);

        unsigned char* buffer = mat.data;
        int linebytes = mat.step[0];
        cv::Size size = mat.size();
        int height = size.height;
        int width = size.width;
        int channels = mat.channels();
        _pxl_encode_bmp("result1.bmp", height, width, channels, buffer, linebytes, false);
    }

    if (1)
    // bpp=3, align, save image
    {
        const char* filename = "mingren.bmp";
        unsigned char* buffer = NULL;
        int height;
        int width;
        int channel;
        int align = 4;
        _pxl_decode_bmp(filename, align, &height, &width, &channel, &buffer, false);
        PIXEL_LOGD("height:%d, width:%d, channel:%d", height, width, channel);
        
        int linebytes = _pxl_align_up(width*channel, align);
        _pxl_encode_bmp("result2.bmp", height, width, channel, buffer, linebytes, false);
    }

    if(1)
    // bpp=3, no align, save image, rgb/bgr swap
    {
        const char* fn = "mingren.bmp";
        cv::Mat mat = cv::imread(fn);

        unsigned char* buffer = mat.data;
        int linebytes = mat.step[0];
        cv::Size size = mat.size();
        int height = size.height;
        int width = size.width;
        int channels = mat.channels();
        _pxl_encode_bmp("result3.bmp", height, width, channels, buffer, linebytes, true);
    }   

    if (1)
    // bpp=3, align, save image, rgb/bgr swap
    {
        const char* filename = "mingren.bmp";
        unsigned char* buffer = NULL;
        int height;
        int width;
        int channel;
        int align = 4;
        _pxl_decode_bmp(filename, align, &height, &width, &channel, &buffer, false);
        PIXEL_LOGD("height:%d, width:%d, channel:%d", height, width, channel);
        
        int linebytes = _pxl_align_up(width*channel, align);
        PIXEL_LOGD("width=%d, channel=%d, width*channel=%d, linebytes=%d",
            width, channel, width*channel, linebytes);
        _pxl_encode_bmp("result4.bmp", height, width, channel, buffer, linebytes, true);
    }

    if (1)
    // bpp=1, save image, no align
    {
        const char* filename = "ring.bmp";
        unsigned char* buffer = NULL;
        int height;
        int width;
        int channel;
        int align = 1;
        _pxl_decode_bmp(filename, align, &height, &width, &channel, &buffer, false);
        PIXEL_LOGD("height:%d, width:%d, channel:%d", height, width, channel);

        int linebytes = _pxl_align_up(width*channel, align);
        _pxl_encode_bmp("result5.bmp", height, width, channel, buffer, linebytes, false);
    }

    if (1)
    // bpp=1, save image, align=4
    {
        const char* filename = "mingren_gray.bmp";
        unsigned char* buffer = NULL;
        int height;
        int width;
        int channel;
        int align = 4;
        _pxl_decode_bmp(filename, align, &height, &width, &channel, &buffer, false);
        PIXEL_LOGD("height:%d, width:%d, channel:%d", height, width, channel);

        int linebytes = _pxl_align_up(width*channel, align);
        _pxl_encode_bmp("result6.bmp", height, width, channel, buffer, linebytes, false);
    }
}


void prepare_not_aligned_images()
{
    const char* fn = "mingren.bmp";//width=450, not multiple of 4
    cv::Mat mat = cv::imread(fn, 0);
    cv::imshow("image", mat);
    cv::waitKey(0);
    cv::imwrite("mingren_gray.bmp", mat);
}

int main(){
    //prepare_not_aligned_images();

    test_decode_bmp();

    test_encode_bmp();

    return 0;
}