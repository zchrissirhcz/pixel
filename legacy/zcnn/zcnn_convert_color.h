#pragma once

#include "zcnn_image.h"

#ifdef __cplusplus
extern "C" {
#endif

    // Gray = R*0.299 + G*0.587 + B*0.114
    void convert_bgr_to_gray(const image_t* input, image_t* output);

    void convert_bgr_to_rgb(image_t* bgr, image_t* rgb);

#ifdef __cplusplus
}
#endif