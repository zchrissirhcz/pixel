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
void bgr_to_yuv420(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int i, j;
    int YUVwidth, YUVheight;
    uint8_t* pY, *pU, *pV;

    YUVwidth = ((bgr_width >> 1) << 1);
    YUVheight = ((bgr_height >> 1) << 1);

    pY = yuv_data;
    pU = yuv_data + YUVwidth * YUVheight;
    pV = pU + YUVwidth * YUVheight / 4;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (i = 0; i < YUVheight; i += 2)
    {
        for (j = 0; j < YUVwidth; j += 2)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            j++;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;


            i++; j--;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            j++;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
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

void bgr_to_yuv420_rotate90(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int i, j;
    int YUVwidth, YUVheight;
    uint8_t* pY, *pU, *pV;

    YUVwidth = ((bgr_width >> 1) << 1);
    YUVheight = ((bgr_height >> 1) << 1);

    pY = yuv_data;
    pU = yuv_data + YUVwidth * YUVheight;
    pV = pU + YUVwidth * YUVheight / 4;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (j = 0; j < YUVwidth; j += 2)
    {
        for (i = 0; i < YUVheight; i += 2)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            j++;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;



            i++; j--;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            j++;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

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

void bgr_to_yuv420_rotate270(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int i, j;
    int YUVwidth, YUVheight;
    uint8_t* pY, *pU, *pV;

    YUVwidth = ((bgr_width >> 1) << 1);
    YUVheight = ((bgr_height >> 1) << 1);

    pY = yuv_data;
    pU = yuv_data + YUVwidth * YUVheight;
    pV = pU + YUVwidth * YUVheight / 4;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (j = 0; j < YUVwidth; j += 2)
    {
        for (i = 0; i < YUVheight; i += 2)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3];
            g = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 1];
            r = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            j++;

            b = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3];
            g = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 1];
            r = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            i++; j--;

            b = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3];
            g = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 1];
            r = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[j*YUVheight + i] = y1;

            j++;

            b = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3];
            g = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 1];
            r = bgr_data[(YUVheight - 1 - i)*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

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





void bgr_to_yuv422(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int b, g, r, y1, y2, cr1, cr2, cb1, cb2, i, j;
    int YUVwidth = EVEN_DOWN(bgr_width);
    int YUVheight = EVEN_DOWN(bgr_height);

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (i = 0; i < YUVheight; i++)
    {
        for (j = 0; j < YUVwidth; j += 2)
        {
            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            b = bgr_data[i*bgr_line_elem + (j + 1) * 3];
            g = bgr_data[i*bgr_line_elem + (j + 1) * 3 + 1];
            r = bgr_data[i*bgr_line_elem + (j + 1) * 3 + 2];

            y2 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb2 = ((b - y2)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr2 = ((r - y2)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*YUVwidth * 2 + j * 2] = y1;
            yuv_data[i*YUVwidth * 2 + j * 2 + 1] = (cb1 + cb2) >> 1;
            yuv_data[i*YUVwidth * 2 + j * 2 + 2] = y2;
            yuv_data[i*YUVwidth * 2 + j * 2 + 3] = (cr1 + cr2) >> 1;
        }
    }
}

void bgr_to_yvyu422(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int b, g, r, y1, y2, cr1, cr2, cb1, cb2, i, j;
    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);
    for (i = 0; i < bgr_height; i++)
    {
        for (j = 0; j < bgr_width; j += 2)
        {
            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            b = bgr_data[i*bgr_line_elem + (j + 1) * 3];
            g = bgr_data[i*bgr_line_elem + (j + 1) * 3 + 1];
            r = bgr_data[i*bgr_line_elem + (j + 1) * 3 + 2];

            y2 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb2 = ((b - y2)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr2 = ((r - y2)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*bgr_width * 2 + j * 2] = y2;
            yuv_data[i*bgr_width * 2 + j * 2 + 1] = (cb1 + cb2) >> 1;
            yuv_data[i*bgr_width * 2 + j * 2 + 2] = y1;
            yuv_data[i*bgr_width * 2 + j * 2 + 3] = (cr1 + cr2) >> 1;
        }
    }
}

void bgr_to_uyvy422(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int b, g, r, y1, y2, cr1, cr2, cb1, cb2, i, j;
    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);
    for (i = 0; i < bgr_height; i++)
    {
        for (j = 0; j < bgr_width; j += 2)
        {
            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            b = bgr_data[i*bgr_line_elem + (j + 1) * 3];
            g = bgr_data[i*bgr_line_elem + (j + 1) * 3 + 1];
            r = bgr_data[i*bgr_line_elem + (j + 1) * 3 + 2];

            y2 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb2 = ((b - y2)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr2 = ((r - y2)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*bgr_width * 2 + j * 2] = (cb1 + cb2) >> 1;
            yuv_data[i*bgr_width * 2 + j * 2 + 1] = y1;
            yuv_data[i*bgr_width * 2 + j * 2 + 2] = (cr1 + cr2) >> 1;
            yuv_data[i*bgr_width * 2 + j * 2 + 3] = y2;
        }
    }
}

void bgr_to_vyuy422(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int b, g, r, y1, y2, cr1, cr2, cb1, cb2, i, j;
    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);
    for (i = 0; i < bgr_height; i++)
    {
        for (j = 0; j < bgr_width; j += 2)
        {
            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            b = bgr_data[i*bgr_line_elem + (j + 1) * 3];
            g = bgr_data[i*bgr_line_elem + (j + 1) * 3 + 1];
            r = bgr_data[i*bgr_line_elem + (j + 1) * 3 + 2];

            y2 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb2 = ((b - y2)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr2 = ((r - y2)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*bgr_width * 2 + j * 2] = (cr1 + cr2) >> 1;
            yuv_data[i*bgr_width * 2 + j * 2 + 1] = y1;
            yuv_data[i*bgr_width * 2 + j * 2 + 2] = (cb1 + cb2) >> 1;
            yuv_data[i*bgr_width * 2 + j * 2 + 3] = y2;
        }
    }
}






void bgr_to_gray(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* gray_data)
{
    int i, j;
    uint8_t* pY, *pU, *pV;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (i = 0; i < bgr_height; i++)
    {
        for (j = 0; j < bgr_width; j++)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;

            gray_data[i*bgr_width + j] = y1;
        }
    }
}


void bgr_to_bgr565(uint8_t* bgr_data, int nW, int nH, uint8_t* bgr565_data)
{
    short *pdst = (short *)bgr565_data;
    int iSrcXStride = LINE_BYTES(nW, 24);
    int iDstYStride = LINE_BYTES(nW, 16) / 2;
    int i, j;
    for (i = 0; i < nH; i++)
    {
        for (j = 0; j < nW; j++)
        {
            uint8_t b = bgr_data[i*iSrcXStride + j * 3];
            uint8_t g = bgr_data[i*iSrcXStride + j * 3 + 1];
            uint8_t r = bgr_data[i*iSrcXStride + j * 3 + 2];
            short wColor = (r >> 3 << 11) | (g >> 2 << 5) | (b >> 3);
            pdst[i*iDstYStride + j] = wColor;
        }
    }
}


void bgr_to_bgra(uint8_t* bgr_data, int nW, int nH, uint8_t* bgra_data)
{
    int iSrcXStride = LINE_BYTES(nW, 24);
    int iDstXStride = LINE_BYTES(nW, 32);
    int i, j;
    for (i = 0; i < nH; i++)
    {
        for (j = 0; j < nW; j++)
        {
            bgra_data[i * iDstXStride + (j << 2)] = bgr_data[i * iSrcXStride + j * 3];
            bgra_data[i * iDstXStride + (j << 2) + 1] = bgr_data[i * iSrcXStride + j * 3 + 1];
            bgra_data[i * iDstXStride + (j << 2) + 2] = bgr_data[i * iSrcXStride + j * 3 + 2];
        }
    }
}



void bgr_to_nv21_separate(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int i, j, YUVwidth, YUVheight;
    uint8_t* pY, *pU, *pV;

    YUVwidth = EVEN_DOWN(bgr_width);
    YUVheight = EVEN_DOWN(bgr_height);

    pY = yuv_data;
    pU = yuv_data + YUVwidth * YUVheight;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (i = 0; i < YUVheight; i += 2)
    {
        for (j = 0; j < YUVwidth;)
        {
            int b, g, r, y1, cr1, cb1;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 = ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 = ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            j++;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            i++; j--;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb1 += ((b - y1)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr1 += ((r - y1)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            pY[i*YUVwidth + j] = y1;

            j++;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y1 = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
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

void bgr_to_yuv444(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data)
{
    int i, j;
    int YUVwidth, YUVheight;
    int widthstep_yuv = 3 * bgr_width;
    YUVwidth = bgr_width;
    YUVheight = bgr_height;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (i = 0; i < YUVheight; i++)
    {
        for (j = 0; j < YUVwidth; j++)
        {
            int b, g, r, y, cr, cb;

            b = bgr_data[i*bgr_line_elem + j * 3];
            g = bgr_data[i*bgr_line_elem + j * 3 + 1];
            r = bgr_data[i*bgr_line_elem + j * 3 + 2];

            y = (b*yuvYb + g * yuvYg + r * yuvYr) >> FIX_SHIFT;
            cb = ((b - y)*yuvCb + (128 << FIX_SHIFT)) >> FIX_SHIFT;
            cr = ((r - y)*yuvCr + (128 << FIX_SHIFT)) >> FIX_SHIFT;

            yuv_data[i*widthstep_yuv + j * 3] = y;
            yuv_data[i*widthstep_yuv + j * 3 + 1] = cb;
            yuv_data[i*widthstep_yuv + j * 3 + 2] = cr;

        }
    }
}

void bgr_from_yuv444(uint8_t* yuv_data, uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment)
{
    int i, j;
    int BGRwidth, BGRheight;
    int widthstep_yuv = 3 * bgr_width;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (i = 0; i < bgr_height; i++)
    {
        for (j = 0; j < bgr_width; j++)
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


            bgr_data[i*bgr_line_elem + j * 3] = b;
            bgr_data[i*bgr_line_elem + j * 3 + 1] = g;
            bgr_data[i*bgr_line_elem + j * 3 + 2] = r;

        }
    }
}

void bgr_from_yuv420(uint8_t* yuv_data, uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment)
{
    int i, j;
    int BGRwidth, BGRheight;
    uint8_t*py, *pu, *pv;
    int widthstep_y = bgr_width;
    int widthstep_u = bgr_width / 2;
    int widthstep_v = bgr_width / 2;
    py = yuv_data;
    pu = yuv_data + bgr_width * bgr_height;
    pv = pu + bgr_width * bgr_height / 4;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (i = 0; i < bgr_height; i++)
    {
        for (j = 0; j < bgr_width; j++)
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


            bgr_data[i*bgr_line_elem + j * 3] = b;
            bgr_data[i*bgr_line_elem + j * 3 + 1] = g;
            bgr_data[i*bgr_line_elem + j * 3 + 2] = r;

        }
    }
}

void bgr_from_nv21(uint8_t* yuv_data, uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment)
{
    int i, j;
    int BGRwidth, BGRheight;
    uint8_t*py, *pv;
    int widthstep_y = bgr_width;
    int widthstep_u = bgr_width / 2;
    int widthstep_v = bgr_width / 2;
    py = yuv_data;
    pv = yuv_data + bgr_width * bgr_height;

    const int bgr_line_elem = px_align_up(bgr_width*3, bgr_line_alignment);

    for (i = 0; i < bgr_height; i++)
    {
        for (j = 0; j < bgr_width; j++)
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

            bgr_data[i*bgr_line_elem + j * 3] = b;
            bgr_data[i*bgr_line_elem + j * 3 + 1] = g;
            bgr_data[i*bgr_line_elem + j * 3 + 2] = r;

        }
    }
}
