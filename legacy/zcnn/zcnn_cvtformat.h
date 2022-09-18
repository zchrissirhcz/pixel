#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uchar;

/******** for transformation of image format ********/

/// @param width 实际有效的宽度（单位：像素数量）
/// @param bgr_line_alignment how many elements in the bgr image, should be >= 1, such as 4

void bgr_to_yuv420(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);
void bgr_to_yuv420_rotate90(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);
void bgr_to_yuv420_rotate270(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);

void bgr_to_yuv422(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);
void bgr_to_yvyu422(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);
void bgr_to_uyvy422(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);
void bgr_to_vyuy422(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);

void bgr_to_gray(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* gray_data);
void bgr_to_bgr565(uchar* bgr_data, int nW, int nH, uchar* bgr565_data);
void bgr_to_bgra(uchar* bgr_data, int nW, int nH, uchar* bgra_data);

void bgr_to_nv21(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);
void bgr_to_yuv444(uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uchar* yuv_data);

void bgr_from_yuv444(uchar* yuv_data, uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment);
void bgr_from_yuv420(uchar* yuv_data, uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment);
void bgr_from_nv21(uchar* yuv_data, uchar* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment);


#ifdef __cplusplus
}
#endif
