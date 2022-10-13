#include "px_filesystem.h"
#include "px_log.h"

#if __linux__ || __APPLE__
#include <sys/stat.h>
#include <unistd.h>
#elif _MSC_VER
#include <direct.h>
#include <io.h>
#endif

#include <stdbool.h>
#include <errno.h>

bool px_is_directory_exist(const char* dirname)
{
#if __linux__ || __APPLE__
    if (0 == access(dirname, W_OK))
    {
        return true;
    }
    else
    {
        return false;
    }
#elif _MSC_VER
    PX_LOGE("warning: %s is not implemented well for MSVC!", __FUNCTION__);
    return false;
#else
    // TODO
    PX_LOGE("warning: %s is not implemented well for current platform!", __FUNCTION__);
    return false;
#endif
}

PX_MKDIR_ERROR px_mkdir(const char* dirname)
{
#if __linux__ || __APPLE__
    if (px_is_directory_exist(dirname))
    {
        return PX_MKDIR_ALREADY_EXIST;
    }
    else
    {
        if (0 != mkdir(dirname, 0744))
        {
            return PX_MKDIR_CREATE_FAILED;
        } 
        else
        {
            return PX_MKDIR_CREATE_SUCCESS;
        }
    }
#elif _MSC_VER
    if(_mkdir(dirname) == 0)
    {
        return PX_MKDIR_CREATE_SUCCESS;
    }
    else
    {
        if (errno == EEXIST)
        {
            return PX_MKDIR_ALREADY_EXIST;
        }
        else if (errno == ENOENT)
        {
            return PX_MKDIR_CREATE_FAILED;
        }
    }
#else
    PX_LOGE("%s not implemented yet for current platform!\n", __FUNCTION__);
#endif
    return PX_MKDIR_CREATE_FAILED;
}

char* px_getpwd()
{
    char* buffer = NULL;
#ifdef _MSC_VER
    buffer = _getcwd(NULL, 0);
#elif defined(__GNUC__)
    buffer = getcwd(NULL, 0);
#else
    PX_LOGE("%s not implemented yet for current platform!\n", __FUNCTION__);
#endif

    if (buffer == NULL)
    {
        PX_LOGE("Failed to get current directory\n");
    }
    return buffer;
}

PX_FILE_STATE px_get_file_or_directory_state(const char* fname)
{
#if _MSC_VER
    #define access _access
#endif
    if (access(fname, 0)!=-1)
    {
        if (access(fname, 2)!=-1)
        {
            return PX_FILE_EXIST_AND_WRITABLE;
        }
        else
        {
            return PX_FILE_EXIST_AND_NOT_WRITABLE;
        }
    }
    else
    {
        return PX_FILE_NOT_EXIST;
    }
}

bool px_is_file_exist(const char* fname)
{
    return px_get_file_or_directory_state(fname) != PX_FILE_NOT_EXIST;
}