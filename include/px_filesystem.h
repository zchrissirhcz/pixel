#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "px_log.h"

#define PX_MAX_PATH 256

#define PX_CHECK_WRITE_FILE(fp, filename) \
    if (fp == NULL)                    \
        PX_LOGE("Failed to open file %s for write in %s:%d", filename, __FILE__, __LINE__); \
        exit;

#define PX_CHECK_READ_FILE(fp, filename) \
    if (fp == NULL)                   \
        PX_LOGE("Failed to open file %s for read in %s:%d", filename, __FILE__, __LINE__); \
        exit;

typedef enum PX_MKDIR_ERROR
{
    PX_MKDIR_CREATE_SUCCESS,
    PX_MKDIR_CREATE_FAILED,
    PX_MKDIR_ALREADY_EXIST,
} PX_MKDIR_ERROR;

PX_MKDIR_ERROR px_mkdir(const char* dirname);
bool px_is_directory_exist(const char* dirname);
char* px_getpwd();

#ifdef __cplusplus
}
#endif