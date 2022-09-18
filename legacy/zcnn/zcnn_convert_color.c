#include "zcnn_convert_color.h"
#include <assert.h>
#include <stdio.h>


//ref: icvCvt_BGR2Gray_8u_C3C1R(), from OpenCV 3.4.9
//opencv/modules/imgcodecs/src/utils.cpp
#define  SCALE  14
#define  cR  (int)(0.299*(1 << SCALE) + 0.5)
#define  cG  (int)(0.587*(1 << SCALE) + 0.5)
#define  cB  ((1 << SCALE) - cR - cG)

#define  descale(x,n)  (((x) + (1 << ((n)-1))) >> (n))

// Gray = R*0.299 + G*0.587 + B*0.114
void convert_bgr_to_gray(const image_t* input, image_t* output)
{
    assert(input->channels == 3);
    assert(input->data != NULL);
    assert(output->channels == 1);
    assert(output->data != NULL);
    assert(input->data != output->data);

    /// naive impl
#if 0
    uchar* buf = input->data;
    for (int h = 0; h < input->height; h++) {
        for (int w = 0; w < input->width; w++) {
            int src_idx = h * input->linebytes + w * 3;
            int y = 0.299*buf[src_idx + 2] + 0.587*buf[src_idx + 1] + 0.114*buf[src_idx];
            if (y > 255) y = 255;
            if (y < 0) y = 0;
            int dst_idx = h * output->linebytes + w;
            output->data[dst_idx] = (uchar)y;
        }
    }
#endif

    /// fixed point impl, copied from OpenCV
#if 1
    uchar* bgr = input->data;
    uchar* gray = output->data;
    const int bgr_line_elem = input->width * input->channels;
    const int gray_line_elem = output->width;
    for (int h = 0; h < input->height; h++, gray+=gray_line_elem) {
        short cRGB0 = cR;
        short cRGB2 = cB;
        for (int w = 0; w < input->width; w++, bgr+=3) {
            int t = descale(bgr[0] * cRGB0 + bgr[1] * cG + bgr[2] * cRGB2, SCALE);
            gray[w] = (uchar)t;
        }
        bgr += bgr_line_elem - input->width * 3;
    }
#endif

}

void convert_bgr_to_rgb(image_t* bgr, image_t* rgb)
{
    assert(bgr!=NULL && rgb!=NULL);
    assert(bgr->data!=NULL && rgb->data!=NULL);
    assert(bgr->height==rgb->height && bgr->width==rgb->width);
    assert(bgr->channels==3 && rgb->channels==3);

    const int height = bgr->height;
    const int width = bgr->width;
    uchar* src = bgr->data;
    uchar* dst = rgb->data;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            const int idx = i * width * 3 + j * 3;
            dst[idx + 0] = src[idx + 2];
            dst[idx + 1] = src[idx + 1];
            dst[idx + 2] = src[idx + 0];

            // im->data[idx] ^= im->data[idx + 2] ^= im->data[idx] ^= im->data[idx + 2];
        }
    }
}
