#pragma once

#include <stdint.h>
#include "px_image.h"

#ifdef __cplusplus
extern "C" {
#endif

/******** for transformation of image format ********/

/// @param width 实际有效的宽度（单位：像素数量）
/// @param bgr_line_alignment how many elements in the bgr image, should be >= 1, such as 4

void bgr_to_yuv420(px_image_t* bgr, uint8_t* yuv_data);
void bgr_to_yuv420_rotate90(px_image_t* bgr, uint8_t* yuv_data);
void bgr_to_yuv420_rotate270(px_image_t* bgr, uint8_t* yuv_data);

void bgr_to_yuv422(px_image_t* bgr, uint8_t* yuv_data);
void bgr_to_yvyu422(px_image_t* bgr, uint8_t* yuv_data);
void bgr_to_uyvy422(px_image_t* bgr, uint8_t* yuv_data);
void bgr_to_vyuy422(px_image_t* bgr, uint8_t* yuv_data);

void bgr_to_gray(px_image_t* bgr, uint8_t* gray_data);
void bgr_to_bgr565(px_image_t* bgr, uint8_t* bgr565_data);
void bgr_to_bgra(px_image_t* bgr, uint8_t* bgra_data);

void bgr_to_nv21(px_image_t* bgr, uint8_t* yuv_data);
void bgr_to_yuv444(px_image_t* bgr, uint8_t* yuv_data);

void bgr_from_yuv444(uint8_t* yuv_data, px_image_t* bgr);
void bgr_from_yuv420(uint8_t* yuv_data, px_image_t* bgr);
void bgr_from_nv21(uint8_t* yuv_data, px_image_t* bgr);


#ifdef __cplusplus
}
#endif
