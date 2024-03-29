#include "px_image.h"
#include "px_log.h"

// https://docs.opencv.org/4.5.2/d2/de8/group__core__array.html
// https://vovkos.github.io/doxyrest-showcase/opencv/sphinx_rtd_theme/enum_cv_BorderTypes.html

// iiiiii|abcdefgh|iiiiiii with some specified i
int px_border_clip_constant(int x, int size, int i)
{
    if (x < 0 || x >= size)
    {
        return i;
    }
    return x;
}

// aaaaaa|abcdefgh|hhhhhhh
int px_border_clip_replicate(int x, int size)
{
    if (x < 0)
    {
        return 0;
    }
    if (x > size-1)
    {
        return size-1;
    }
    return x;
}

// fedcba|abcdefgh|hgfedcb
int px_border_clip_reflect(int x, int size)
{
    if (x < 0)
    {
        return px_border_clip_reflect(-x-1, size);
    }
    if (x > size-1)
    {
        //return size-(x-size) - 1 => size - x + size - 1 => 2*size - x - 1
        return px_border_clip_reflect(2*size - x - 1, size);
    }
    return x;
}

// cdefgh|abcdefgh|abcdefg
int px_border_clip_wrap(int x, int size)
{
    if (x < 0)
    {
        return px_border_clip_wrap(size + x, size);
    }
    if (x > size-1)
    {
        return px_border_clip_wrap(x - size, size);
    }
    return x;
}

// gfedcb|abcdefgh|gfedcba
int px_border_clip_reflect101(int x, int size)
{
    if (x < 0)
    {
        return px_border_clip_reflect101(-x, size);
    }
    else if (x > size-1)
    {
        //return 2*size - 2 - x; // size-1 - (x-(size-1))  =>  size - 1 - (x - size + 1) => size - 1 - x + size - 1 => 2*size - 2 - x
        return px_border_clip_reflect101(2 * size - 2 - x, size);
    }
    else
    {
        return x;
    }
}

// uvwxyz|absdefgh|ijklmno
// https://github.com/opencv/opencv/issues/6015  , BORDER_TRANSPARENT is for ROI image
// int border_clip_transparent(){
// }

int px_border_clip(PX_BORDER_TYPE border_type, int x, int size)
{
    switch (border_type)
    {
    case PX_BORDER_CONSTANT:
        return px_border_clip_constant(x, size, 0);
    case PX_BORDER_REPLICATE:
        return px_border_clip_replicate(x, size);
    case PX_BORDER_REFLECT:
        return px_border_clip_reflect(x, size);
    case PX_BORDER_WRAP:
        return px_border_clip_wrap(x, size);
    case PX_BORDER_REFLECT101:
        return px_border_clip_reflect101(x, size);
    default:
        PX_LOGE("not suported PX_BORDER_TYPE: %d\n", border_type);
        return -233;
    }
}

