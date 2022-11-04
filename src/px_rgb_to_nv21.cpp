#include "px_image.h"
#include "px_arithm.h"
#include "px_compare.h"
#include "px_assert.h"
#include "px_log.h"

namespace {

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
    static int get_y(int r, int g, int b, bool require_shift_right = true)
    {
        int y = (19 * r + 38 * g + 7 * b);
        if (require_shift_right)
        {
            y = y >> 6;
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

// https://blog.csdn.net/denisyq/article/details/52625692
class RgbToYuv_Converter_v3
{
public:
    static int get_y(int r, int g, int b, bool require_shift_right = true)
    {
        int y = (C2 * r + C1 * g + C0 * b + offset1);
        if (require_shift_right)
        {
            y = y >> shift;
        }
        return y;
    }

    static int get_u(int r, int g, int b)
    {
        int y = get_y(r, g, b);
        int u = (C4 * (b - y) + offset2) >> shift;
        return px_clamp(u, 0, 255);
    }

    static int get_v(int r, int g, int b)
    {
        int y = get_y(r, g, b);
        int v = (C3 * (r - y) + offset2) >> shift;
        return px_clamp(v, 0, 255);
    }

private:
    const static int shift = 14;
    const static int offset1 = 8192;
    const static int offset2 = 2105344;
    const static int C0 = 1868;
    const static int C1 = 9617;
    const static int C2 = 4899;
    const static int C3 = 11682;
    const static int C4 = 9241;
};

} // namespace

void px_rgb_to_nv21(px_image_t* rgb, px_image_t* y_plane, px_image_t* uv_plane)
{
    PX_ASSERT(px_is_valid_rgb_image(rgb));
    PX_ASSERT(px_is_valid_yuv420sp_image_pair(y_plane, uv_plane));
    PX_ASSERT(px_image_equal_in_size(rgb, y_plane));

    const int width = y_plane->width;
    const int height = y_plane->height;

    const int bIdx = 2;
    const int cn = rgb->channel;
    const int uIdx = 1;
    int r, g, b;
    int y, u, v;

    RgbToYuv_Converter_v2 converter;
    for (int i = 0; i < height; i++)
    {
        uint8_t* ypt = y_plane->data + i * y_plane->stride;
        uint8_t* uvpt = uv_plane->data + (i / 2) * uv_plane->stride;
        const uint8_t* src = rgb->data + i * rgb->stride;

        for (int j = 0; j < width; j++)
        {
            r = src[2 - bIdx];
            g = src[1];
            b = src[bIdx];
            src += cn;

            y = converter.get_y(r, g, b);
            *(ypt++) = px_clamp(y, 0, 255);
            
            if (i % 2 == 0 && j % 2 == 0)
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