#ifndef PIXEL_IMPROC_BORDER_CLIP_H
#define PIXEL_IMPROC_BORDER_CLIP_H

// https://docs.opencv.org/4.5.2/d2/de8/group__core__array.html
// https://vovkos.github.io/doxyrest-showcase/opencv/sphinx_rtd_theme/enum_cv_BorderTypes.html

typedef enum BorderType {
    kBorderConstant = 0,
    kBorderReplicate = 1,
    kBorderReflect = 2,
    kBorderWrap = 3,
    kBorderReflect101 = 4,
    //BORDER_TRANSPARENT = 5,
    kBorderDefault = kBorderReflect101,
    //BORDER_ISOLATED = 16
} BorderType;

#ifdef __cplusplus
extern "C" {
#endif

static int border_clip_constant(int x, int size, int i);
static int border_clip_replicate(int x, int size);
static int border_clip_reflect(int x, int size);
static int border_clip_wrap(int x, int size);
static int border_clip_reflect101(int x, int size);

static int border_clip(BorderType border_type, int x, int size);

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------

// iiiiii|abcdefgh|iiiiiii with some specified i
int border_clip_constant(int x, int size, int i)
{
    if (x<0 || x>=size) {
        return i;
    }
    return x;
}

// aaaaaa|abcdefgh|hhhhhhh
int border_clip_replicate(int x, int size)
{
    if (x<0) {
        return 0;
    }
    if (x>size-1) {
        return size-1;
    }
    return x;
}

// fedcba|abcdefgh|hgfedcb
int border_clip_reflect(int x, int size)
{
    if (x<0) {
        return border_clip_reflect(-x-1, size);
    }
    if (x>size-1) {
        //return size-(x-size) - 1 => size - x + size - 1 => 2*size - x - 1
        return border_clip_reflect(2*size - x - 1, size);
    }
    return x;
}

// cdefgh|abcdefgh|abcdefg
int border_clip_wrap(int x, int size)
{
    if (x<0) {
        return border_clip_wrap(size+x, size);
    }
    if (x>size-1) {
        return border_clip_wrap(x-size, size);
    }
    return x;
}

// gfedcb|abcdefgh|gfedcba
int border_clip_reflect101(int x, int size) {
    if (x<0) {
        return border_clip_reflect101(-x, size);
    }
    else if (x>size-1) {
        //return 2*size - 2 - x; // size-1 - (x-(size-1))  =>  size - 1 - (x - size + 1) => size - 1 - x + size - 1 => 2*size - 2 - x
        return border_clip_reflect101(2*size - 2 - x, size);
    }
    else {
        return x;
    }
}

// uvwxyz|absdefgh|ijklmno
// https://github.com/opencv/opencv/issues/6015  , BORDER_TRANSPARENT is for ROI image
// int border_clip_transparent(){
// }

int border_clip(BorderType border_type, int x, int size)
{
    switch(border_type) {
        case kBorderConstant:
            return border_clip_constant(x, size, 0);
        case kBorderReplicate:
            return border_clip_replicate(x, size);
        case kBorderReflect:
            return border_clip_reflect(x, size);
        case kBorderWrap:
            return border_clip_wrap(x, size);
        case kBorderReflect101:
            return border_clip_reflect101(x, size);
        default:
            return -233;
    }
}


#endif // PIXEL_IMPROC_BORDER_CLIP_H