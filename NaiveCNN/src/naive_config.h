#pragma once

#include "px_log.h"

#define CHECK_WRITE_FILE(fp, filename) \
    if (fp==NULL) \
        PX_LOGE("write file %s failed in line %d, file %s\n", filename, __LINE__, __FILE__);

#define CHECK_READ_FILE(fp, filename) \
    if (fp==NULL) \
        PX_LOGE("read file %s failed in line %d, file %s\n", filename, __LINE__, __FILE__);

#define NC_MAX_PATH 256
#define NC_IMAGE_ALIGN 1 // for simplicity, we don't align now
#define NC_CHANNEL_ALIGN 1 // for simplicity, we don't align now
#define NC_MAX_BLOB_PER_LAYER 100




