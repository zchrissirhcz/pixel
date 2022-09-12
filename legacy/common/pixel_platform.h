#pragma once

#include "pixel_log.h"

void pxl_platform_echo()
{
#ifdef _MSC_VER
    PIXEL_LOGI("MSVC\n");
#endif

#if ANDROID
    PIXEL_LOGI("android\n");
#endif

#if __linux__ && !(ANDROID)
    PIXEL_LOGI("linux\n");
#endif

#if __APPLE__
    PIXEL_LOGI("Apple\n");
#endif

    PIXEL_LOGI("hello\n");
}


