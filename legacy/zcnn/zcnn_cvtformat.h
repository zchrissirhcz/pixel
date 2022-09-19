#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/******** for transformation of image format ********/

/// @param width 实际有效的宽度（单位：像素数量）
/// @param bgr_line_alignment how many elements in the bgr image, should be >= 1, such as 4

void bgr_to_yuv420(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);
void bgr_to_yuv420_rotate90(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);
void bgr_to_yuv420_rotate270(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);

void bgr_to_yuv422(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);
void bgr_to_yvyu422(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);
void bgr_to_uyvy422(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);
void bgr_to_vyuy422(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);

void bgr_to_gray(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* gray_data);
void bgr_to_bgr565(uint8_t* bgr_data, int nW, int nH, uint8_t* bgr565_data);
void bgr_to_bgra(uint8_t* bgr_data, int nW, int nH, uint8_t* bgra_data);

void bgr_to_nv21(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);
void bgr_to_yuv444(uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment, uint8_t* yuv_data);

void bgr_from_yuv444(uint8_t* yuv_data, uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment);
void bgr_from_yuv420(uint8_t* yuv_data, uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment);
void bgr_from_nv21(uint8_t* yuv_data, uint8_t* bgr_data, int bgr_width, int bgr_height, int bgr_line_alignment);


#ifdef __cplusplus
}
#endif
