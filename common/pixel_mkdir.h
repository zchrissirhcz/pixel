#ifndef PIXEL_MKDIR_H
#define PIXEL_MKDIR_H

#if __linux__ || __APPLE__
#include <sys/stat.h>
#include <unistd.h>
#elif _MSC_VER
#include <direct.h>
#endif

//#include "common/pixel_log.h"
//if not including pixel_log.h, please uncomment this two lines
//#define PIXEL_LOGD(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)
//#define PIXEL_LOGE(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)


static void pixel_mkdir(const char* dirname) {
#if __linux__ || __APPLE__
    if (0==access(dirname, W_OK)) {
        PIXEL_LOGD("Directory %s already exists\n", dirname);
    } else {
        if (0!=mkdir(dirname, 0744)) {
            PIXEL_LOGD("Failed to create directory %s\n", dirname);
        } else {
            PIXEL_LOGD("Directory %s was successfully created\n", dirname);
        }
    }
#elif _MSC_VER
    if( _mkdir(dirname) == 0 ) {
        PIXEL_LOGD("Directory %s was successfully created\n", dirname);
    } else {
        if (errno == EEXIST) {
            PIXEL_LOGD("Problem creating directory %s, already exists\n", dirname);
        }
        else if (errno == ENOENT) {
            PIXEL_LOGD("Problem creating directory %s, Path was not found\n", dirname);
        }
    }
#else
    PIXEL_LOGE("%s not implemented yet!\n", __FUNCTION__);
#endif
}

#endif // PIXEL_MKDIR_H
