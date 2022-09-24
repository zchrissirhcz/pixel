#include "fibercv.h"

px_image_t* histogram(px_image_t* im)
{
    bool is_gray = false;
    if(im->channel==1)
    {
        is_gray = true;
    }

    px_image_t* gray;
    if(is_gray)
    {
        gray = im;
    }
    else
    {
        gray = px_create_image(im->height, im->width, 1);
        px_bgr2gray(im, gray);
    }

    int len = gray->height * gray->width;
    int gray_lut[256] = {0};
    for(int i = 0; i < len; i++)
    {
        gray_lut[gray->data[i]]++;
    }
    if(!is_gray)
    {
        px_destroy_image(gray);
    }

    int max_gray = 0;
    for(int i=0; i<256; i++)
    {
        if(gray_lut[i]>max_gray) max_gray=gray_lut[i];
    }

    int width = 256;
    int height = 100;
    int channel = 3;
    px_image_t* hist = px_create_image(height, width, channel);
    memset(hist->data, 255, hist->height * hist->stride);

    int linebytes = hist->stride;
    for(int i = 0; i < hist->width; i++)
    {
        int sum = gray_lut[i] * hist->height / max_gray;
        for(int j = 0; j < sum; j++)
        {
            int idx = (hist->height-1-j)*linebytes + i*hist->channel;
            hist->data[idx] = 128;
            hist->data[idx+1] = 128;
            hist->data[idx+2] = 128;
        }
    }
    return hist;
}
