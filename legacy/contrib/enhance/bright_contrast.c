#include "bright_contrast.h"
#include "px_arithm.h"

void bright_contrast(unsigned char* src_rgb, size_t height, size_t width, int bright, int contrast, unsigned char* dst_rgb)
{
    bright = px_clamp(bright, -100, 100);
    contrast = px_clamp(contrast, -100, 100);

    // calculate image-level average brightness
    unsigned int sum = 0;
    size_t total_len = height * width;
    unsigned char* src_ptr = src_rgb;
    for (size_t i = 0; i < total_len; i++)
    {
        sum += (77*src_ptr[0] + 151*src_ptr[1] + 28*src_ptr[2]) >> 8;
        src_ptr += 3;
    }
    int average = sum / total_len;

    unsigned char bc_map[256]; // bright-contrast map
    for (int i=0; i<256; i++)
    {
        int temp = contrast > 0 ? px_clamp(i+bright, 0, 255) : i;
        if (contrast > 0)
        {
            temp = px_clamp(i+bright, 0, 255);
            temp = px_clamp(average + (temp-average)*(1.0f/(1.0f - contrast/100.f)), 0, 255);
        }
        else
        {
            temp = i;
            temp = px_clamp(average +(temp-average)*(1.0f + contrast/100.0f), 0, 255);
        }
        bc_map[i] = temp;
    }
    src_ptr = src_rgb;
    unsigned char* dst_ptr = dst_rgb;
    for (size_t i = 0; i < total_len; i++)
    {
        dst_ptr[0] = bc_map[src_ptr[0]];
        dst_ptr[1] = bc_map[src_ptr[1]];
        dst_ptr[2] = bc_map[src_ptr[2]];
        dst_ptr += 3;
        src_ptr += 3;
    }
}