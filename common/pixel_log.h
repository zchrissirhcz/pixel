/*********************************************************************************
 * [Usage]:
 *      const char* name = "ChrisZZ";
 *      PIXEL_LOGD("hello, %s", name);
 *      PIXEL_LOGI("This is an info message");
 *      PIXEL_LOGE("I am error !");
 *      int n = 5;
 *      PIXEL_LOG_IF_ERROR(n!=0, "n is not zero, this is error");
 * 
 * [Customization]:
 *  0. Search [user setting] and modify, such as:
 *  1. Turn on/off one log level by modify values of:
 *      _PXL_LOG_DEBUG_ENABLED
 *      _PXL_LOG_INFO_ENABLED
 *      _PXL_LOG_WARN_ENABLED
 *      _PXL_LOG_ERROR_ENABLED
 *  2. Select pre-defined log format by define to 1 in **only one** of:
 *      _PXL_LOGFMT_FULL
 *      _PXL_LOGFMT_MEDIUM
 *      _PXL_LOGFMT_SIMPLE
 *********************************************************************************/

#ifndef PIXEL_LOG_H
#define PIXEL_LOG_H

#include <stdio.h>
#include <time.h>
#include <string.h>

#ifdef ANDROID
#include <android/log.h>
#endif

// Compatible with Android NDK
enum PixelLogPriority {
    PIXEL_LOG_DEBUG = 3,
    PIXEL_LOG_INFO,
    PIXEL_LOG_WARN,
    PIXEL_LOG_ERROR,
};
const static char* g_pixel_log_priority_str[7] = {
    0, 0, 0,
    "Debug",
    "Info",
    "Warn",
    "Error"
};

//--------------------------------------------------------------------------------
// => switches for each log level
// [user setting]
//--------------------------------------------------------------------------------
#define _PXL_LOG_DEBUG_ENABLED      1
#define _PXL_LOG_INFO_ENABLED       1
#define _PXL_LOG_WARN_ENABLED       1
#define _PXL_LOG_ERROR_ENABLED      1

//--------------------------------------------------------------------------------
// => select log format
// [user setting]
//--------------------------------------------------------------------------------
#define _PXL_LOGFMT_FULL            0
#define _PXL_LOGFMT_MEDIUM          1
#define _PXL_LOGFMT_SIMPLE          0

//--------------------------------------------------------------------------------
// => filename
//--------------------------------------------------------------------------------
// only filename
#ifdef _MSC_VER
    #define _PXL_FILE strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__
#else
    #define _PXL_FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
#endif
// contain folder name such as src/log.cpp
//#define _FILE __FILE__

//--------------------------------------------------------------------------------
// => function
//--------------------------------------------------------------------------------
// only function name
#define _PXL_FUNCTION __FUNCTION__
// function name with parameter types
//#define _FUNCTION __PRETTY_FUNCTION__

//--------------------------------------------------------------------------------
// => tag
//--------------------------------------------------------------------------------
// the tag for current module/library/file
#define _PXL_MODULE_TAG "pixel"


#if _PXL_LOGFMT_FULL
static inline char* timenow() {
    time_t rawtime = time(NULL);
    struct tm* timeinfo = localtime(&rawtime);
    static char now[64];

//--------------------------------------------------------------------------------
// => timestamp format
//--------------------------------------------------------------------------------
#define _PXL_TIMESTAMP_FMT "%Y-%m-%d %H:%M:%S"
// if timezone required, use %z
//#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S(%z)"

    now[strftime(now, sizeof(now), _PXL_TIMESTAMP_FMT, timeinfo)] = '\0';
#undef _PXL_TIMESTAMP_FMT
    return now;
}
#endif

//--------------------------------------------------------------------------------
// => log format
//--------------------------------------------------------------------------------
// the param `fmt` is user specified format
// _PXL_LOG_FULL_FMT(fmt) ammed pre-defined stuffs (time, filename, line num, function) with colors
#define _PXL_LOG_FULL_FMT(fmt)                   ("%s | %s | %s[%-5s]\x1b[0m | %s, line %d, %s | " fmt)
#define _PXL_LOG_FULL_ARGS(tag, color, priority) timenow(), tag, color, g_pixel_log_priority_str[priority], _PXL_FILE, __LINE__, _PXL_FUNCTION

// the param `fmt` is user specified format
// _PXL_LOG_MEDIUM_FMT(fmt) ammed pre-defined stuffs (filename, line num)
#define _PXL_LOG_MEDIUM_FMT(fmt)                   ("%s/%s%-5s\x1b[0m | %s, line %d | " fmt)
#define _PXL_LOG_MEDIUM_ARGS(tag, color, priority) tag, color, g_pixel_log_priority_str[priority], _PXL_FILE, __LINE__

//--------------------------------------------------------------------------------
// => log template for all levels
//--------------------------------------------------------------------------------
// the full format log
#ifdef ANDROID
#define _PXL_LOGT_FULL(priority, fmt, tag, fd, color, ...) do { \
        __android_log_print(priority, tag, _PXL_LOG_FULL_FMT(fmt), _PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, _PXL_LOG_FULL_FMT(fmt), _PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, "\n"); \
    } while(0)
#else
#define _PXL_LOGT_FULL(priority, fmt, tag, fd, color, ...) do { \
        fprintf(fd, _PXL_LOG_FULL_FMT(fmt), _PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, "\n"); \
    } while(0)
#endif

// the medium format log
#ifdef ANDROID
#define _PXL_LOGT_MEDIUM(priority, fmt, tag, fd, color, ...) do { \
        __android_log_print(priority, tag, _PXL_LOG_MEDIUM_FMT(fmt), _PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, _PXL_LOG_MEDIUM_FMT(fmt), _PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, "\n"); \
    } while(0)
#else
#define _PXL_LOGT_MEDIUM(priority, fmt, tag, fd, color, ...) do { \
        fprintf(fd, _PXL_LOG_MEDIUM_FMT(fmt), _PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while(0)
#endif

// simple format log
#ifdef ANDROID
#define _PXL_LOGT_SIMPLE(priority, fmt, ...) do { \
       __android_log_print(priority, _PXL_MODULE_TAG, fmt, ##__VA_ARGS__); \
       fprintf(stderr, (fmt "\n"), ##__VA_ARGS__); \
    } while(0)
#else
#define _PXL_LOGT_SIMPLE(priority, fmt, ...) do { \
        fprintf(stderr, (fmt "\n"), ##__VA_ARGS__); \
    } while(0)
#endif

//--------------------------------------------------------------------------------
// => log template for each level
//--------------------------------------------------------------------------------
// NOTE: if using stderr, `run-and-install.sh`(start on PC, run on Android) will mess up log order
// For better log order, always use stderr since it is not buffered
#if _PXL_LOGFMT_FULL
#define _PXL_LOGDT_FULL(fmt, ...) _PXL_LOGT_FULL(PIXEL_LOG_DEBUG, fmt, _PXL_MODULE_TAG, stderr, "\x1b[36m", ##__VA_ARGS__)
#define _PXL_LOGIT_FULL(fmt, ...) _PXL_LOGT_FULL(PIXEL_LOG_INFO,  fmt, _PXL_MODULE_TAG, stderr, "\x1b[32m", ##__VA_ARGS__)
#define _PXL_LOGWT_FULL(fmt, ...) _PXL_LOGT_FULL(PIXEL_LOG_WARN,  fmt, _PXL_MODULE_TAG, stderr, "\x1b[33m", ##__VA_ARGS__)
#define _PXL_LOGET_FULL(fmt, ...) _PXL_LOGT_FULL(PIXEL_LOG_ERROR, fmt, _PXL_MODULE_TAG, stderr, "\x1b[31m", ##__VA_ARGS__)
#elif _PXL_LOGFMT_MEDIUM
#define _PXL_LOGDT_MEDIUM(fmt, ...) _PXL_LOGT_MEDIUM(PIXEL_LOG_DEBUG, fmt, _PXL_MODULE_TAG, stderr, "\x1b[36m", ##__VA_ARGS__)
#define _PXL_LOGIT_MEDIUM(fmt, ...) _PXL_LOGT_MEDIUM(PIXEL_LOG_INFO,  fmt, _PXL_MODULE_TAG, stderr, "\x1b[32m", ##__VA_ARGS__)
#define _PXL_LOGWT_MEDIUM(fmt, ...) _PXL_LOGT_MEDIUM(PIXEL_LOG_WARN,  fmt, _PXL_MODULE_TAG, stderr, "\x1b[33m", ##__VA_ARGS__)
#define _PXL_LOGET_MEDIUM(fmt, ...) _PXL_LOGT_MEDIUM(PIXEL_LOG_ERROR, fmt, _PXL_MODULE_TAG, stderr, "\x1b[31m", ##__VA_ARGS__)
#elif _PXL_LOGFMT_SIMPLE
#define _PXL_LOGDT_SIMPLE(fmt, ...) _PXL_LOGT_SIMPLE(PIXEL_LOG_DEBUG, fmt, ##__VA_ARGS__)
#define _PXL_LOGIT_SIMPLE(fmt, ...) _PXL_LOGT_SIMPLE(PIXEL_LOG_INFO,  fmt, ##__VA_ARGS__)
#define _PXL_LOGWT_SIMPLE(fmt, ...) _PXL_LOGT_SIMPLE(PIXEL_LOG_WARN,  fmt, ##__VA_ARGS__)
#define _PXL_LOGET_SIMPLE(fmt, ...) _PXL_LOGT_SIMPLE(PIXEL_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
#pragma message("Please define one of _PXL_LOGFMT_FULL, _PXL_LOGFMT_MEDIUM or _PXL_LOGFMT_SIMPLE") 
#endif

//--------------------------------------------------------------------------------
// => specify each log level's template
// [user setting]
//--------------------------------------------------------------------------------
// use full format template
#if _PXL_LOGFMT_FULL
#define _PXL_LOGDT(fmt, ...) _PXL_LOGDT_FULL(fmt, ##__VA_ARGS__)
#define _PXL_LOGIT(fmt, ...) _PXL_LOGIT_FULL(fmt, ##__VA_ARGS__)
#define _PXL_LOGWT(fmt, ...) _PXL_LOGWT_FULL(fmt, ##__VA_ARGS__)
#define _PXL_LOGET(fmt, ...) _PXL_LOGET_FULL(fmt, ##__VA_ARGS__)
#elif _PXL_LOGFMT_MEDIUM
#define _PXL_LOGDT(fmt, ...) _PXL_LOGDT_MEDIUM(fmt, ##__VA_ARGS__)
#define _PXL_LOGIT(fmt, ...) _PXL_LOGIT_MEDIUM(fmt, ##__VA_ARGS__)
#define _PXL_LOGWT(fmt, ...) _PXL_LOGWT_MEDIUM(fmt, ##__VA_ARGS__)
#define _PXL_LOGET(fmt, ...) _PXL_LOGET_MEDIUM(fmt, ##__VA_ARGS__)
#elif _PXL_LOGFMT_SIMPLE
#define _PXL_LOGIT(fmt, ...) _PXL_LOGIT_SIMPLE(fmt, ##__VA_ARGS__)
#define _PXL_LOGDT(fmt, ...) _PXL_LOGDT_SIMPLE(fmt, ##__VA_ARGS__)
#define _PXL_LOGWT(fmt, ...) _PXL_LOGWT_SIMPLE(fmt, ##__VA_ARGS__)
#define _PXL_LOGET(fmt, ...) _PXL_LOGET_SIMPLE(fmt, ##__VA_ARGS__)
#else
#pragma message("Please define one of _PXL_LOGFMT_FULL, _PXL_LOGFMT_MEDIUM or _PXL_LOGFMT_SIMPLE") 
#endif

//--------------------------------------------------------------------------------
// => enable log macro according to LOG_LEVEL
//--------------------------------------------------------------------------------
#if _PXL_LOG_INFO_ENABLED
#define PIXEL_LOGI(fmt, ...) _PXL_LOGIT(fmt, ##__VA_ARGS__)
#else
#define PIXEL_LOGI(fmt, ...)
#endif

#if _PXL_LOG_DEBUG_ENABLED
#define PIXEL_LOGD(fmt, ...) _PXL_LOGDT(fmt, ##__VA_ARGS__)
#else
#define PIXEL_LOGD(fmt, ...)
#endif

#if _PXL_LOG_WARN_ENABLED
#define PIXEL_LOGW(fmt, ...) _PXL_LOGWT(fmt, ##__VA_ARGS__)
#else
#define PIXEL_LOGW(fmt, ...)
#endif

#if _PXL_LOG_ERROR_ENABLED
#define PIXEL_LOGE(fmt, ...) _PXL_LOGET(fmt, ##__VA_ARGS__)
#else
#define PIXEL_LOGE(fmt, ...)
#endif

#if _PXL_LOG_ERROR_ENABLED
#define PIXEL_LOG_IF_ERROR(condition, fmt, ...) do { \
        if (condition) PIXEL_LOGE(fmt, ##__VA_ARGS__); \
    } while(0)
#define PIXEL_ASSERT(condition, fmt, ...) do { \
        if (!(condition)) PIXEL_LOGE(fmt, ##__VA_ARGS__); \
    } while(0)
#else
#define PIXEL_LOG_IF_ERROR(condition, fmt, ...)
#define PIXEL_ASSERT(condition, fmt, ...)
#endif


#endif // PIXEL_LOG_H