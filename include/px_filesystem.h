#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum PX_MKDIR_ERROR
{
    PX_MKDIR_CREATE_SUCCESS,
    PX_MKDIR_CREATE_FAILED,
    PX_MKDIR_ALREADY_EXIST,
} PX_MKDIR_ERROR;

PX_MKDIR_ERROR px_mkdir(const char* dirname);

#ifdef __cplusplus
}
#endif