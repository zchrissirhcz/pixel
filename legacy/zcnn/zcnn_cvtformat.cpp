#include "zcnn_cvtformat.h"
#include "px_arithm.h"

#define fix(x,n) (int)((x)*(1 << (n)) + 0.5)

#define FIX_SHIFT 10
#define yuvYr  fix(0.299, FIX_SHIFT)
#define yuvYg  fix(0.587, FIX_SHIFT)
#define yuvYb  fix(0.114, FIX_SHIFT)
#define yuvCr  fix(0.713, FIX_SHIFT)
#define yuvCb  fix(0.564, FIX_SHIFT)
//#define yuvCr  fix(0.877, FIX_SHIFT)
//#define yuvCb  fix(0.492, FIX_SHIFT)

#define  rgbRv fix(1.4075, FIX_SHIFT)
#define  rgbGu fix(0.3455, FIX_SHIFT)
#define  rgbGv fix(0.7169, FIX_SHIFT)
#define  rgbBu fix(1.779, FIX_SHIFT)

#define ALIGN(size) (((size)+3) & -4)
#define EVEN_DOWN(x) (x&0xfffffffe)
#define CLIP(x) (uint8_t)((x)&(~255)?((-x)>>31):(x))
#define LINE_BYTES(Width, BitCt)    (((int)(Width) * (BitCt) + 31) / 32 * 4)


// 要求开辟yuv_data时的宽和高均为偶数; width&height分别为原始图像的宽和高, 可以为奇数
void bgr_to_yuv420(px_image_t* bgr, uint8_t* yuv_data)
{
    int i, j;
    int YUVwidth, YUVheight;
    uint8_t* pY, *pU, *pV;

    YUVwidth = ((bgr->width >> 1) << 1);
    YUVheight = ((bgr->height >> 1) << 1);

    pY = yuv_data;
    pU = yuv_data + YUVwidth * YUVheight;
    pV = pU + YUVwidth * YUVheight / 4;

    const int bgr_line_elem = bgr->stride;

    for (i = 0; i < YUVheight; i += 2)
    {
        for (j = 0; j < YUVwidth; j += 2)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            j++;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;


            i++; j--;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            j++;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;


            cb1 = CLIP((cb1 >> 2));
            cr1 = CLIP((cr1 >> 2));

            *pU++ = cb1;
            *pV++ = cr1;

            j--;
            i--;
        }
    }
}

void bgr_to_yuv420_rotate90(px_image_t* bgr, uint8_t* yuv_data)
{
    int i, j;
    int YUVwidth, YUVheight;
    uint8_t* pY, *pU, *pV;

    YUVwidth = ((bgr->width >> 1) << 1);
    YUVheight = ((bgr->height >> 1) << 1);

    pY = yuv_data;
    pU = yuv_data + YUVwidth * YUVheight;
    pV = pU + YUVwidth * YUVheight / 4;

    const int bgr_line_elem = bgr->stride;

    for (j = 0; j < YUVwidth; j += 2)
    {
        for (i = 0; i < YUVheight; i += 2)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            j++;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;



            i++; j--;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            j++;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            cb1 = CLIP((cb1 >> 2));
            cr1 = CLIP((cr1 >> 2));

            *pU++ = cb1;
            *pV++ = cr1;

            j--;
            i--;
        }
    }
}

void bgr_to_yuv420_rotate270(px_image_t* bgr, uint8_t* yuv_data)
{
    int i, j;
    int YUVwidth, YUVheight;
    uint8_t* pY, *pU, *pV;

    YUVwidth = ((bgr->width >> 1) << 1);
    YUVheight = ((bgr->height >> 1) << 1);

    pY = yuv_data;
    pU = yuv_data + YUVwidth * YUVheight;
    pV = pU + YUVwidth * YUVheight / 4;

    const int bgr_line_elem = bgr->stride;

    for (j = 0; j < YUVwidth; j += 2)
    {
        for (i = 0; i < YUVheight; i += 2)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3];
            g = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 1];
            r = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            j++;

            b = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3];
            g = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 1];
            r = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            i++; j--;

            b = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3];
            g = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 1];
            r = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            j++;

            b = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3];
            g = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 1];
            r = bgr->data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            cb1 = CLIP((cb1 >> 2));
            cr1 = CLIP((cr1 >> 2));

            *pU++ = cb1;
            *pV++ = cr1;

            j--;
            i--;
        }
    }
}

void bgr_to_yuv422(px_image_t* bgr, uint8_t* yuv_data)
{
    int b, g, r, y1, y2, cr1, cr2, cb1, cb2, i, j;
    int YUVwidth = EVEN_DOWN(bgr->width);
    int YUVheight = EVEN_DOWN(bgr->height);

    const int bgr_line_elem = bgr->stride;

    for (i = 0; i < YUVheight; i++)
    {
        for (j = 0; j < YUVwidth; j += 2)
        {
            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            b = bgr->data[i*bgr_line_elem + (j + 1) * 3];
            g = bgr->data[i*bgr_line_elem + (j + 1) * 3 + 1];
            r = bgr->data[i*bgr_line_elem + (j + 1) * 3 + 2];

            y2 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb2 = ((b - y2)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr2 = ((r - y2)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*YUVwidth * 2 + j * 2] = y1;
            yuv_data[i*YUVwidth * 2 + j * 2 + 1] = (cb1 + cb2) >> 1;
            yuv_data[i*YUVwidth * 2 + j * 2 + 2] = y2;
            yuv_data[i*YUVwidth * 2 + j * 2 + 3] = (cr1 + cr2) >> 1;
        }
    }
}

void bgr_to_yvyu422(px_image_t* bgr, uint8_t* yuv_data)
{
    int b, g, r, y1, y2, cr1, cr2, cb1, cb2, i, j;
    const int bgr_line_elem = bgr->stride;
    for (i = 0; i < bgr->height; i++)
    {
        for (j = 0; j < bgr->width; j += 2)
        {
            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            b = bgr->data[i*bgr_line_elem + (j + 1) * 3];
            g = bgr->data[i*bgr_line_elem + (j + 1) * 3 + 1];
            r = bgr->data[i*bgr_line_elem + (j + 1) * 3 + 2];

            y2 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb2 = ((b - y2)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr2 = ((r - y2)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*bgr->width * 2 + j * 2] = y2;
            yuv_data[i*bgr->width * 2 + j * 2 + 1] = (cb1 + cb2) >> 1;
            yuv_data[i*bgr->width * 2 + j * 2 + 2] = y1;
            yuv_data[i*bgr->width * 2 + j * 2 + 3] = (cr1 + cr2) >> 1;
        }
    }
}

void bgr_to_uyvy422(px_image_t* bgr, uint8_t* yuv_data)
{
    int b, g, r, y1, y2, cr1, cr2, cb1, cb2, i, j;
    const int bgr_line_elem = bgr->stride;
    for (i = 0; i < bgr->height; i++)
    {
        for (j = 0; j < bgr->width; j += 2)
        {
            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            b = bgr->data[i*bgr_line_elem + (j + 1) * 3];
            g = bgr->data[i*bgr_line_elem + (j + 1) * 3 + 1];
            r = bgr->data[i*bgr_line_elem + (j + 1) * 3 + 2];

            y2 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb2 = ((b - y2)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr2 = ((r - y2)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*bgr->width * 2 + j * 2] = (cb1 + cb2) >> 1;
            yuv_data[i*bgr->width * 2 + j * 2 + 1] = y1;
            yuv_data[i*bgr->width * 2 + j * 2 + 2] = (cr1 + cr2) >> 1;
            yuv_data[i*bgr->width * 2 + j * 2 + 3] = y2;
        }
    }
}

void bgr_to_vyuy422(px_image_t* bgr, uint8_t* yuv_data)
{
    int b, g, r, y1, y2, cr1, cr2, cb1, cb2, i, j;
    const int bgr_line_elem = bgr->stride;
    for (i = 0; i < bgr->height; i++)
    {
        for (j = 0; j < bgr->width; j += 2)
        {
            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            b = bgr->data[i*bgr_line_elem + (j + 1) * 3];
            g = bgr->data[i*bgr_line_elem + (j + 1) * 3 + 1];
            r = bgr->data[i*bgr_line_elem + (j + 1) * 3 + 2];

            y2 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb2 = ((b - y2)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr2 = ((r - y2)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*bgr->width * 2 + j * 2] = (cr1 + cr2) >> 1;
            yuv_data[i*bgr->width * 2 + j * 2 + 1] = y1;
            yuv_data[i*bgr->width * 2 + j * 2 + 2] = (cb1 + cb2) >> 1;
            yuv_data[i*bgr->width * 2 + j * 2 + 3] = y2;
        }
    }
}

void bgr_to_gray(px_image_t* bgr, px_image_t* gray)
{
    const int bIdx = 0;
    const int rgbCn = 3;
    for (int i = 0; i < bgr->height; i++)
    {
        uint8_t* sp = bgr->data + i * bgr->stride;
        uint8_t* dp = gray->data + i * gray->stride;
        for (int j = 0; j < bgr->width; j++)
        {
            int b = sp[bIdx];
            int g = sp[1];
            int r = sp[2-bIdx];

            int y = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            *dp++ = y;

            sp += rgbCn;
        }
    }
}

void bgr_to_bgr565(px_image_t* bgr, uint8_t* bgr565_data)
{
    short *pdst = (short *)bgr565_data;
    const int nW = bgr->width;
    const int nH = bgr->height;
    int iSrcXStride = LINE_BYTES(nW, 24);
    int iDstYStride = LINE_BYTES(nW, 16) / 2;
    int i, j;
    for (i = 0; i < nH; i++)
    {
        for (j = 0; j < nW; j++)
        {
            uint8_t b = bgr->data[i*iSrcXStride + j * 3];
            uint8_t g = bgr->data[i*iSrcXStride + j * 3 + 1];
            uint8_t r = bgr->data[i*iSrcXStride + j * 3 + 2];
            short wColor = (r >> 3 << 11) | (g >> 2 << 5) | (b >> 3);
            pdst[i*iDstYStride + j] = wColor;
        }
    }
}


void bgr_to_nv21_separate(px_image_t* bgr, uint8_t* yuv_data)
{
    int i, j, YUVwidth, YUVheight;
    uint8_t* pY, *pU, *pV;

    YUVwidth = EVEN_DOWN(bgr->width);
    YUVheight = EVEN_DOWN(bgr->height);

    pY = yuv_data;
    pU = yuv_data + YUVwidth * YUVheight;

    const int bgr_line_elem = bgr->stride;

    for (i = 0; i < YUVheight; i += 2)
    {
        for (j = 0; j < YUVwidth;)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            j++;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            i++; j--;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            j++;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            cb1 = CLIP((cb1 >> 2));
            cr1 = CLIP((cr1 >> 2));

            *pU++ = cr1;
            *pU++ = cb1;

            j++;
            i--;
        }
    }
}

void bgr_to_yuv444(px_image_t* bgr, uint8_t* yuv_data)
{
    int i, j;
    int YUVwidth, YUVheight;
    int widthstep_yuv = 3 * bgr->width;
    YUVwidth = bgr->width;
    YUVheight = bgr->height;

    const int bgr_line_elem = bgr->stride;

    for (i = 0; i < YUVheight; i++)
    {
        for (j = 0; j < YUVwidth; j++)
        {
            int b, g, r, y, cr, cb;

            b = bgr->data[i*bgr_line_elem + j * 3];
            g = bgr->data[i*bgr_line_elem + j * 3 + 1];
            r = bgr->data[i*bgr_line_elem + j * 3 + 2];

            y = (b * yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb = ((b - y)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr = ((r - y)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*widthstep_yuv + j * 3] = y;
            yuv_data[i*widthstep_yuv + j * 3 + 1] = cb;
            yuv_data[i*widthstep_yuv + j * 3 + 2] = cr;

        }
    }
}

void bgr_from_yuv444(uint8_t* yuv_data, px_image_t* bgr)
{
    int i, j;
    int BGRwidth, BGRheight;
    int widthstep_yuv = 3 * bgr->width;

    const int bgr_line_elem = bgr->stride;

    for (i = 0; i < bgr->height; i++)
    {
        for (j = 0; j < bgr->width; j++)
        {
            int b, g, r, y, u, v;

            y = yuv_data[i*widthstep_yuv + j * 3];
            u = yuv_data[i*widthstep_yuv + j * 3 + 1];
            v = yuv_data[i*widthstep_yuv + j * 3 + 2];

            u -= 128;
            v -= 128;

            r = y + ((v*rgbRv) >> FIX_SHIFT);
            g = y - ((u*rgbGu + v * rgbGv) >> FIX_SHIFT);
            b = y + ((u*rgbBu) >> FIX_SHIFT);


            bgr->data[i*bgr_line_elem + j * 3] = b;
            bgr->data[i*bgr_line_elem + j * 3 + 1] = g;
            bgr->data[i*bgr_line_elem + j * 3 + 2] = r;

        }
    }
}

void bgr_from_yuv420(uint8_t* yuv_data, px_image_t* bgr)
{
    int i, j;
    int BGRwidth, BGRheight;
    uint8_t*py, *pu, *pv;
    int widthstep_y = bgr->width;
    int widthstep_u = bgr->width / 2;
    int widthstep_v = bgr->width / 2;
    py = yuv_data;
    pu = yuv_data + bgr->width * bgr->height;
    pv = pu + bgr->width * bgr->height / 4;

    const int bgr_line_elem = bgr->stride;

    for (i = 0; i < bgr->height; i++)
    {
        for (j = 0; j < bgr->width; j++)
        {
            int b, g, r, y, u, v;

            y = py[i*widthstep_y + j];
            u = pu[i / 2 * widthstep_u + j / 2];
            v = pv[i / 2 * widthstep_v + j / 2];

            u -= 128;
            v -= 128;

            r = y + ((v*rgbRv) >> FIX_SHIFT);
            g = y - ((u*rgbGu + v * rgbGv) >> FIX_SHIFT);
            b = y + ((u*rgbBu) >> FIX_SHIFT);


            bgr->data[i*bgr_line_elem + j * 3] = b;
            bgr->data[i*bgr_line_elem + j * 3 + 1] = g;
            bgr->data[i*bgr_line_elem + j * 3 + 2] = r;

        }
    }
}

void bgr_from_nv21(uint8_t* yuv_data, px_image_t* bgr)
{
    int i, j;
    int BGRwidth, BGRheight;
    uint8_t*py, *pv;
    int widthstep_y = bgr->width;
    int widthstep_u = bgr->width / 2;
    int widthstep_v = bgr->width / 2;
    py = yuv_data;
    pv = yuv_data + bgr->width * bgr->height;

    const int bgr_line_elem = bgr->stride;

    for (i = 0; i < bgr->height; i++)
    {
        for (j = 0; j < bgr->width; j++)
        {
            int b, g, r, y, u, v;

            y = py[i*widthstep_y + j];
            v = pv[i / 2 * widthstep_u + j / 2]; //索引计算错误：应改为EVEN_DOWN(i)*width/2 + EVEN_DOWN(j)
            u = pv[i / 2 * widthstep_u + j / 2 + 1];

            u -= 128;
            v -= 128;

            r = y + ((v*rgbRv) >> FIX_SHIFT);
            g = y - ((u*rgbGu + v * rgbGv) >> FIX_SHIFT);
            b = y + ((u*rgbBu) >> FIX_SHIFT);

            // 溢出问题：没有对b，g和r做clip到[0,255]的操作

            bgr->data[i*bgr_line_elem + j * 3] = b;
            bgr->data[i*bgr_line_elem + j * 3 + 1] = g;
            bgr->data[i*bgr_line_elem + j * 3 + 2] = r;

        }
    }
}
