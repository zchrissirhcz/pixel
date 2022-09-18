#pragma once

#include <stdio.h>
#include <assert.h>


// the simple version
#define ZCNN_LOGE(...) fprintf(stderr, ##__VA_ARGS__)

// the verbose version
//#define ZCNN_LOGE(fmt, ...) fprintf(stderr, ("line=%d, file=%s, " fmt), __LINE__, __FILE__,  ##__VA_ARGS__)

// the verbose version, VSCode/IDE friendly for clicking-and-jumping-to-definition
//#define ZCNN_LOGE(fmt, ...) fprintf(stderr, ("%s:%d " fmt), __FILE__, __LINE__,  ##__VA_ARGS__)

/*
// TODO: merge android logging with ZCNN_LOGE

#define _DEBUG_ 1

#if _DEBUG_ // debug mode
    // -- begin of windows/linux printf
#if defined(_MSC_VER) || defined(__linux__)
#define FC_PRINTF(...) printf(__VA_ARGS__)
    // -- end of window/linux printf

    // -- begin of TI_C66 DSP printf
#elif defined(TI_C66)
#define FC_PRINTF(...) Vps_printf(__VA_ARGS__)
    // -- end of TI_C66 DSP printf

    // -- begin of android printf
#elif defined(ANDROID)
#include <android/log.h>
#define LOG_TAG "FC_LOG"
#define FC_PRINTF(...)	__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGV(...)	__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...)	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...)	__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...)	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
    // -- end of android printf
#endif

#else // release mode
#define FC_PRINTF(...) 
#endif

*/



#if _MSC_VER
#include <windows.h>
//TODO: WINDOWS MEAN AND LEAN macro
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#include <time.h>

/// return 毫秒(ms)
static inline double get_current_time()
{
#ifdef _WIN32
    LARGE_INTEGER freq;
    LARGE_INTEGER pc;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&pc);
    return pc.QuadPart * 1000.0 / freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec/1000000.0;
#endif // _WIN32
}


// for compatibility, I also provide `gettimeofday()`
// however, you should never use this for your new written code
// it is only for fast-transporting quick & dirty work
#if _MSC_VER
int gettimeofday(struct timeval* tp, void* tzp)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;

    GetLocalTime(&wtm);
    tm.tm_year = wtm.wYear - 1900;
    tm.tm_mon = wtm.wMonth - 1;
    tm.tm_mday = wtm.wDay;
    tm.tm_hour = wtm.wHour;
    tm.tm_min = wtm.wMinute;
    tm.tm_sec = wtm.wSecond;
    tm.tm_isdst = -1;
    clock = mktime(&tm);
    tp->tv_sec = clock;
    tp->tv_usec = wtm.wMilliseconds * 1000;

    return (0);
}
#endif


// for compaitibility purpose only
static inline void fc_sleep(int milli_secs)
{
#ifdef _MSC_VER
    Sleep(milli_secs);
#elif defined(__linux__)
    usleep(milli_secs * 1000); // 1 ms = 1000 us
#endif
}