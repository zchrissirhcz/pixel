#include "px_image.h"
#include "px_assert.h"
#include "px_compare.h"
#include "px_arithm.h"
#include <algorithm>

namespace {

class YuvToRgb_Converter_v1
{
public:
    static int get_r(int y, int u, int v)
    {
        v = v - 128;

        int rdif = ((v * 45) >> 5);
        int r = y + rdif;
        return px_clamp(r, 0, 255);
    }

    static int get_g(int y, int u, int v)
    {
        u = u - 128;
        v = v - 128;

        int gdif = ((u * 11) >> 5) + ((v * 23) >> 5);
        int g = y - gdif;
        return px_clamp(g, 0, 255);
    }

    static int get_b(int y, int u, int v)
    {
        u = u - 128;

        int bdif = ((u * 57) >> 5);
        int b = y + bdif;
        return px_clamp(b, 0, 255);
    }
};

class YuvToRgb_Converter_v2
{
public:
    static int get_r(int y, int u, int v)
    {
        y = std::max(16, y);
        int r = 1.164 * (y - 16) + 1.596 * (v - 128) + 0.5;
        return px_clamp(r, 0, 255);
    }

    static int get_g(int y, int u, int v)
    {
        y = std::max(16, y);
        int g = 1.164 * (y - 16) - 0.813 * (v - 128) - 0.391 * (u - 128) + 0.5;
        return px_clamp(g, 0, 255);
    }

    static int get_b(int y, int u, int v)
    {
        y = std::max(16, y);
        int b = 1.164 * (y - 16) + 2.018 * (u - 128) + 0.5;
        return px_clamp(b, 0, 255);
    }
};

} // namespace

void px_nv21_to_rgb(px_image_t* y_plane, px_image_t* uv_plane, px_image_t* rgb)
{
    PX_ASSERT(px_is_valid_yuv420sp_image_pair(y_plane, uv_plane));
    PX_ASSERT(px_is_valid_rgb_image(rgb));
    PX_ASSERT(px_image_equal_in_size(rgb, y_plane));

    const int width = y_plane->width;
    const int height = y_plane->height;

    const int uIdx = 1;
    const int bIdx = 2;
    const int cn = 3;

    YuvToRgb_Converter_v2 converter;
    for (int i = 0; i <= height - 2; i += 2)
    {
        const unsigned char* y_ptr0 = y_plane->data + i * y_plane->stride;
        const unsigned char* y_ptr1 = y_plane->data + (i + 1) * y_plane->stride;

        unsigned char* rgb0 = rgb->data + i * rgb->stride;
        unsigned char* rgb1 = rgb->data + (i + 1) * rgb->stride;

        const unsigned char* uv_ptr = uv_plane->data + (i / 2) * uv_plane->stride;

        for (int j = 0; j <= width - 2; j += 2)
        {
            int y;
            int u = uv_ptr[uIdx];
            int v = uv_ptr[1 - uIdx];

            // y00
            y = y_ptr0[0];
            rgb0[2 - bIdx] = converter.get_r(y, u, v);
            rgb0[1] = converter.get_g(y, u, v);
            rgb0[bIdx] = converter.get_b(y, u, v);
            rgb0 += cn;

            // y01
            y = y_ptr0[1];
            rgb0[2 - bIdx] = converter.get_r(y, u, v);
            rgb0[1] = converter.get_g(y, u, v);
            rgb0[bIdx] = converter.get_b(y, u, v);
            rgb0 += cn;

            // y10
            y = y_ptr1[0];
            rgb1[2 - bIdx] = converter.get_r(y, u, v);
            rgb1[1] = converter.get_g(y, u, v);
            rgb1[bIdx] = converter.get_b(y, u, v);
            rgb1 += cn;

            // y11
            y = y_ptr1[1];
            rgb1[2 - bIdx] = converter.get_r(y, u, v);
            rgb1[1] = converter.get_g(y, u, v);
            rgb1[bIdx] = converter.get_b(y, u, v);
            rgb1 += cn;

            y_ptr0 += 2;
            y_ptr1 += 2;
            uv_ptr += 2;
        }
    }
}
