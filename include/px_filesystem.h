#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#define PX_MAX_PATH 256

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