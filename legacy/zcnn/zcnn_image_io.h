#pragma once

#include "zcnn_image.h"

#ifdef __cplusplus
extern "C" {
#endif

    /// load RGB order image.
    image_t* load_image(const char* filename);

    /// only support .bmp, .jpg, .png
    bool save_image(image_t* im, const char* filename);

#ifdef __cplusplus
}
#endif
