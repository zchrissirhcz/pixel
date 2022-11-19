#pragma once

#include "px_typebase.h"

typedef struct px_image_t
{
    uint8_t* data;
    int width;
    int height;
    int channel;
    int stride;
} px_image_t;

typedef enum px_fourcc_fmt_t
{
    PX_FOURCC_FMT_NV21,
    PX_FOURCC_FMT_NV12,
    PX_FOURCC_FMT_BGR,
    PX_FOURCC_FMT_RGB,
    PX_FOURCC_FMT_RGBA,
    PX_FOURCC_FMT_BGRA,
} px_fourcc_fmt_t;

typedef struct px_fourcc_t
{
    int width;
    int height;
    int pitch[4];
    uint8_t planes[4];
    px_fourcc_fmt_t fmt;
} px_fourcc_t;

#ifdef __cplusplus
extern "C" {
#endif

px_image_t* px_create_image_header(int height, int width, int channel);
px_image_t* px_create_image(int height, int width, int channel);
void px_destroy_image(px_image_t* image);
void px_destroy_image_header(px_image_t* image);

px_size_t px_get_image_size(px_image_t* image);

uint8_t px_get_pixel(px_image_t* image, int i, int j, int k);
void px_set_pixel(px_image_t* image, int i, int j, int k, uint8_t value);

/// color format conversion
void px_rgb_to_gray(px_image_t* src, px_image_t* dst);
void px_rgb_to_gray_fixed(px_image_t* src, px_image_t* dst);

void px_bgr_to_gray(px_image_t* src, px_image_t* dst);

void px_rgb_to_bgr(px_image_t* src, px_image_t* dst);
void px_rgb_to_bgr_inplace(px_image_t* image);
void px_rgba_to_bgra(px_image_t* src, px_image_t* dst);
void px_rgba_to_bgra_inplace(px_image_t* image);
void px_rgb_to_rgba(px_image_t* rgb, px_image_t* rgba);

void px_rgb_to_nv21(px_image_t* rgb, px_image_t* y_plane, px_image_t* uv_plane);
void px_nv21_to_rgb(px_image_t* y_plane, px_image_t* uv_plane, px_image_t* rgb);

// flipmode
// input     vertical    horizontal    both
// A  B      C  D        B  A          D  C
// C  D      A  B        D  C          B  A
typedef enum PX_FLIP_MODE
{
    PX_FLIP_VERTICAL = 0,
    PX_FLIP_HORIZONTAL = 1,
    PX_FLIP_BOTH = 2
} PX_FLIP_MODE;
void px_flip(px_image_t* src, px_image_t* dst, PX_FLIP_MODE mode);
void px_flip_inplace(px_image_t* image, PX_FLIP_MODE mode);

typedef enum PX_HISTOGRAM_MODE
{
    PX_HISTOGRAM_GRAY = 0,
    PX_HISTOGRAM_R = 1,
    PX_HISTOGRAM_G = 2,
    PX_HISTOGRAM_B = 3
} PX_HISTOGRAM_MODE;
px_image_t* px_histogram_rgb(px_image_t* src, PX_HISTOGRAM_MODE mode);
px_image_t* px_histogram(px_image_t* im);

typedef enum PX_INTERP_MODE
{
    PX_INTERP_NEAREST,
    PX_INTERP_LINEAR,
    PX_INTERP_CUBIC
} PX_INTERP_MODE;

typedef enum px_border_type
{
    PX_BORDER_CONSTANT = 0,
    PX_BORDER_REPLICATE = 1,
    PX_BORDER_REFLECT = 2,
    PX_BORDER_WRAP = 3,
    PX_BORDER_REFLECT101 = 4,
    // PX_BORDER_TRANSPARENT = 5,
    PX_BORDER_DEFAULT = PX_BORDER_REFLECT101,
    // PX_BORDER_ISOLATED = 16
} px_border_type;

/// image resize
void px_resize_nearest(px_image_t* src, px_image_t* dst, px_size_t dsize);
void px_resize_linear(px_image_t* src, px_image_t* dst, px_size_t dsize);
void px_resize_cubic(px_image_t* src, px_image_t* dst, px_size_t dsize); // TODO
void px_resize(px_image_t* src, px_image_t* dst, px_size_t dsize, PX_INTERP_MODE interp_mode);

void px_threshold(px_image_t* gray, px_image_t* binary, uint8_t thresh, uint8_t minval, uint8_t maxval);

/// border handling
int px_border_clip_constant(int x, int size, int i);
int px_border_clip_replicate(int x, int size);
int px_border_clip_reflect(int x, int size);
int px_border_clip_wrap(int x, int size);
int px_border_clip_reflect101(int x, int size);
int px_border_clip(px_border_type border_type, int x, int size);

void px_copy_cut_border(px_image_t* src, px_image_t* dst, px_pad_t pad);
void px_copy_make_border(px_image_t* src, px_image_t* dst, px_pad_t pad, px_border_type border_type);

/// rotate
void px_image_rotate90(px_image_t* src, px_image_t* dst); // clockwise
void px_image_rotate270(px_image_t* src, px_image_t* dst); // clockwise

void px_sobel3x3(px_image_t* src, px_image_t* dst);
  
#ifdef __cplusplus
}
#endif
