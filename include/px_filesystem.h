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

typedef enum PX_FILE_STATE
{
    PX_FILE_EXIST_AND_WRITABLE = 1,       //!< file exist, and has write permission
    PX_FILE_EXIST_AND_NOT_WRITABLE = 2,   //!< file exist, but not has write permission
    PX_FILE_NOT_EXIST = 3                 //!< file not exist
} PX_FILE_STATE;

PX_MKDIR_ERROR px_mkdir(const char* dirname);
bool px_is_directory_exist(const char* dirname);
char* px_getpwd();


/// @param fname filename or directory name
PX_FILE_STATE px_get_file_or_directory_state(const char* fname);
bool px_is_file_exist(const char* fname);

#ifdef __cplusplus
}
#endif