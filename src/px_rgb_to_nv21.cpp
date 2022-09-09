#include "px_image.h"
#include "px_arithm.h"
#include "px_compare.h"
#include "px_assert.h"
#include "px_log.h"

class RgbToYuv_Converter_v1
{
public:
    static float get_y(int r, int g, int b)
    {
        float y = 0.299 * (float)r + 0.587 * (float)g + 0.114 * (float)b;
        return y;
    }

    static int get_u(int r, int g, int b)
    {
        int u = (int)(-0.1678 * (float)r + (-0.3313) * (float)g + 0.5 * (float)b) + 128;
        return u;
    }

    static int get_v(int r, int g, int b)
    {
        int v = (int)(0.5 * (float)r + (-0.4187) * (float)g + (-0.0813) * (float)b) + 128;
        return v;
    }
};

// The mobilecv actual used one
class RgbToYuv_Converter_v2
{
public:
    static int get_y(int r, int g, int b, bool require_shift_right=true)
    {
        int y;
        if (require_shift_right)
        {
            y = (19 * r + 38 * g + 7 * b) >> 6;
        }
        else
        {
            y = (19 * r + 38 * g + 7 * b);
        }
        return y;
    }
    
    static int get_u(int r, int g, int b)
    {
        int y = get_y(r, g, b);
        int u = ((9 * (b - y)) >> 4) + 128;
        return px_clamp(u, 0, 255);
    }

    static int get_v(int r, int g, int b)
    {
        int y = get_y(r, g, b);
        int v = ((91 * (r - y)) >> 7) + 128;
        return px_clamp(v, 0, 255);
    }
};

void px_rgb_to_nv21(px_image_t* rgb, px_image_t* y_plane, px_image_t* uv_plane)
{
    px_size_t rgb_size = px_get_image_size(rgb);
    px_size_t y_size = px_get_image_size(y_plane);
    px_size_t uv_size = px_get_image_size(uv_plane);
    PX_ASSERT(px_size_equal(rgb_size, y_size));
    PX_ASSERT(y_size.height == 2 * uv_size.height);
    PX_ASSERT(y_size.width == 2 * uv_size.width);
    PX_ASSERT(uv_plane->channel == 2);

    const int width = y_size.width;
    const int height = y_size.height;
    if (width < 2 || height < 2)
    {
        PX_LOGE("invalid dimension\n");
        return;
    }

    const int bIdx = 2;
    const int cn = rgb->channel;
    const int uIdx = 1;
    int r, g, b;
    int y, u, v;

    RgbToYuv_Converter_v2 converter;
    for(int i = 0; i < height; i++)
    {
        uint8_t* ypt = y_plane->data + i * y_plane->stride;
        uint8_t* uvpt = uv_plane->data + (i / 2) * uv_plane->stride;
        const uint8_t* src = rgb->data + i * rgb->stride;

        for(int j = 0; j < width; j++)
        {
            r = src[2 - bIdx];
            g = src[1];
            b = src[bIdx];
            src += cn;

            y = converter.get_y(r, g, b);
            *(ypt++) = px_clamp(y, 0, 255);;
            if (i%2 == 0 && j%2 == 0)
            {
                u = converter.get_u(r, g, b);
                v = converter.get_v(r, g, b);
                uvpt[uIdx] = px_clamp(u, 0, 255);
                uvpt[1 - uIdx] = px_clamp(v, 0, 255);
                uvpt += 2;
            }
        }
    }
}