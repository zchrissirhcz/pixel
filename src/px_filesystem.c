#include "px_filesystem.h"
#include "px_log.h"

#if __linux__ || __APPLE__
#include <sys/stat.h>
#include <unistd.h>
#elif _MSC_VER
#include <direct.h>
#endif

PX_MKDIR_ERROR px_mkdir(const char* dirname)
{
#if __linux__ || __APPLE__
    if (0 == access(dirname, W_OK))
    {
        return PX_MKDIR_ALREADY_EXIST;
    } else
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
    if( _mkdir(dirname) == 0 )
    {
        return PX_MKDIR_CREATE_SUCCESS
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
    PIXEL_LOGE("%s not implemented yet for current platform!\n", __FUNCTION__);
#endif
}

