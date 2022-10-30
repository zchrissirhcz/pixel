#include <stdint.h>
#include "px_image.h"
#include "px_timer.h"
#include "px_image_io.h"
#include "cpu_helper.hpp"

#if __ARM_NEON
#include <arm_neon.h>
#endif // __ARM_NEON

void resize_by_two(uint8_t* src, uint8_t* dst)
{
    for (int h = 0; h < 240; h++)
    {
        for (int w = 0; w < 320; w++)
        {
            dst[h * 320 + w] = (src[640 * h * 2 + w * 2] + src[640 * h * 2 + w * 2 + 1] + src[640 * h * 2 + 640 + w * 2] + src[640 * h * 2 + 640 + w * 2 + 1]) / 4;
        }
    }
}

static void resize_line(uint8_t * __restrict src1, uint8_t * __restrict src2, uint8_t * __restrict dest)
{
    int i;
    for (i = 0; i < 640; i += 16)
    {
        // load upper line and add neighbor pixels:
        uint16x8_t a = vpaddlq_u8(vld1q_u8(src1));

        // load lower line and add neighbor pixels:
        uint16x8_t b = vpaddlq_u8(vld1q_u8(src2));

        // sum of upper and lower line:
        uint16x8_t c = vaddq_u16(a,b);

        // divide by 4, convert to char and store:
        vst1_u8(dest, vshrn_n_u16(c, 2));

        // move pointers to next chunk of data
        src1+=16;
        src2+=16;
        dest+=8;
    }
}

static void resize_line_v2(uint8_t * __restrict src1, uint8_t * __restrict src2, uint8_t * __restrict dest)
{
    int i;
    for (i=0; i<640; i+= 32)
    {
        uint8x16x2_t a, b;
        uint8x16_t c, d;

        /* load upper row, splitting even and odd pixels into a.val[0]
        * and a.val[1] respectively. */
        a = vld2q_u8(src1);

        /* as above, but for lower row */
        b = vld2q_u8(src2);

        /* compute average of even and odd pixel pairs for upper row */
        c = vrhaddq_u8(a.val[0], a.val[1]);
        /* compute average of even and odd pixel pairs for lower row */
        d = vrhaddq_u8(b.val[0], b.val[1]);

        /* compute average of upper and lower rows, and store result */
        vst1q_u8(dest, vrhaddq_u8(c, d));

        src1+=32;
        src2+=32;
        dest+=16;
    }
}

void resize_by_two_neon(uint8_t * src, uint8_t * dest)
{
    int h;
    for (h = 0; h < 240 - 1; h++)
    {
        resize_line(src+640*(h*2+0), src+640*(h*2+1), dest+320*h);
    }
}

void resize_by_two_neon_v2(uint8_t * src, uint8_t * dest)
{
    int h;
    for (h = 0; h < 240 - 1; h++)
    {
        resize_line_v2(src+640*(h*2+0), src+640*(h*2+1), dest+320*h);
    }
}

int main()
{
#if __ANDROID__
    plain::set_cpu_powersave(1);
#endif // __ANDROID__

    px_image_t* im0 = px_read_image("lena.bmp");
    const int height = 480;
    const int width = 640;
    px_image_t* im1 = px_create_image(height, width, im0->channel);
    px_size_t dsize;
    dsize.width = width;
    dsize.height = height;
    px_resize_linear(im0, im1, dsize);

    px_image_t* im2 = px_create_image(height, width, 1);
    px_bgr_to_gray(im1, im2);
    px_write_image(im2, "im2.png");

    px_image_t* res_naive = px_create_image(height / 2, width / 2, im2->channel);
    {
        pixel::AutoTimer timer("naive");
        resize_by_two(im2->data, res_naive->data);
    }
    //px_write_image(res_naive, "res_naive.png");

    px_image_t* res_neon = px_create_image(height / 2, width / 2, im2->channel);
    {
        pixel::AutoTimer timer("neon");
        resize_by_two_neon(im2->data, res_neon->data);
    }
    //px_write_image(res_neon, "res_neon.png");

    px_image_t* res_neon_v2 = px_create_image(height / 2, width / 2, im2->channel);
    {
        pixel::AutoTimer timer("neon_v2");
        resize_by_two_neon(im2->data, res_neon_v2->data);
    }
    //px_write_image(res_neon_v2, "res_neon_v2.png");

    px_destroy_image(im0);
    px_destroy_image(im1);
    px_destroy_image(im2);
    px_destroy_image(res_naive);
    px_destroy_image(res_neon);
    px_destroy_image(res_neon_v2);

    return 0;
}