#include "px_image.h"

void px_rgb_to_rgba(px_image_t* rgb, px_image_t* rgba)
{
    const int width = rgb->width;
    const int height = rgb->height;
    const uint8_t* sp = rgb->data;
    uint8_t* dp = rgba->data;
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            dp[0] = sp[0];
            dp[1] = sp[1];
            dp[2] = sp[2];
            dp[3] = 255;
            dp += 4;
            sp += 3;
        }
    }
}