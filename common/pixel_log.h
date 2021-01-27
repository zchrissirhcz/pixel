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
 *      PXL_LOG_DEBUG_ENABLED
 *      PXL_LOG_INFO_ENABLED
 *      PXL_LOG_WARN_ENABLED
 *      PXL_LOG_ERROR_ENABLED
 *  2. Select pre-defined log format by define to 1 in **only one** of:
 *      PXL_LOGFMT_FULL
 *      PXL_LOGFMT_MEDIUM
 *      PXL_LOGFMT_SIMPLE
 * [Note]
 *  1. `PXL_` is for internal usage macros
 *  2. `PIXEL_` is for public usage macros
 *  3. `_PXL_` will be potentially conflict with compiler implementations
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
#define PXL_LOG_DEBUG_ENABLED      1
#define PXL_LOG_INFO_ENABLED       1
#define PXL_LOG_WARN_ENABLED       1
#define PXL_LOG_ERROR_ENABLED      1

//--------------------------------------------------------------------------------
// => select log format
// [user setting]
//--------------------------------------------------------------------------------
#define PXL_LOGFMT_FULL            0
#define PXL_LOGFMT_MEDIUM          1
#define PXL_LOGFMT_SIMPLE          0

//--------------------------------------------------------------------------------
// => filename
//--------------------------------------------------------------------------------
// only filename
#ifdef _MSC_VER
    #define PXL_FILE strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__
#else
    #define PXL_FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
#endif
// contain folder name such as src/log.cpp
//#define PXL_FILE __FILE__

//--------------------------------------------------------------------------------
// => function
//--------------------------------------------------------------------------------
// only function name
#define PXL_FUNCTION __FUNCTION__
// function name with parameter types
//#define PXL_FUNCTION __PRETTY_FUNCTION__

//--------------------------------------------------------------------------------
// => tag
//--------------------------------------------------------------------------------
// the tag for current module/library/file
#define PXL_MODULE_TAG "pixel"


#if PXL_LOGFMT_FULL
static inline char* timenow() {
    time_t rawtime = time(NULL);
    struct tm* timeinfo = localtime(&rawtime);
    static char now[64];

//--------------------------------------------------------------------------------
// => timestamp format
//--------------------------------------------------------------------------------
#define PXL_TIMESTAMP_FMT "%Y-%m-%d %H:%M:%S"
// if timezone required, use %z
//#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S(%z)"

    now[strftime(now, sizeof(now), PXL_TIMESTAMP_FMT, timeinfo)] = '\0';
#undef PXL_TIMESTAMP_FMT
    return now;
}
#endif

//--------------------------------------------------------------------------------
// => log format
//--------------------------------------------------------------------------------
// the param `fmt` is user specified format
// PXL_LOG_FULL_FMT(fmt) amend pre-defined stuffs (time, filename, line num, function) with colors
#define PXL_LOG_FULL_FMT(fmt)                   ("%s | %s | %s[%-5s]\x1b[0m | %s, line %d, %s | " fmt)
#define PXL_LOG_FULL_ARGS(tag, color, priority) timenow(), tag, color, g_pixel_log_priority_str[priority], PXL_FILE, __LINE__, PXL_FUNCTION

// the param `fmt` is user specified format
// PXL_LOG_MEDIUM_FMT(fmt) amend pre-defined stuffs (filename, line num)
#define PXL_LOG_MEDIUM_FMT(fmt)                   ("%s/%s%-5s\x1b[0m | %s, line %d | " fmt)
#define PXL_LOG_MEDIUM_ARGS(tag, color, priority) tag, color, g_pixel_log_priority_str[priority], PXL_FILE, __LINE__

//--------------------------------------------------------------------------------
// => log template for all levels
//--------------------------------------------------------------------------------
// the full format log
#ifdef ANDROID
#define PXL_LOGT_FULL(priority, fmt, tag, fd, color, ...) do { \
        __android_log_print(priority, tag, PXL_LOG_FULL_FMT(fmt), PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, PXL_LOG_FULL_FMT(fmt), PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, "\n"); \
    } while(0)
#else
#define PXL_LOGT_FULL(priority, fmt, tag, fd, color, ...) do { \
        fprintf(fd, PXL_LOG_FULL_FMT(fmt), PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, "\n"); \
    } while(0)
#endif

// the medium format log
#ifdef ANDROID
#define PXL_LOGT_MEDIUM(priority, fmt, tag, fd, color, ...) do { \
        __android_log_print(priority, tag, PXL_LOG_MEDIUM_FMT(fmt), PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, PXL_LOG_MEDIUM_FMT(fmt), PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, "\n"); \
    } while(0)
#else
#define PXL_LOGT_MEDIUM(priority, fmt, tag, fd, color, ...) do { \
        fprintf(fd, PXL_LOG_MEDIUM_FMT(fmt), PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(stderr, "\n"); \
    } while(0)
#endif

// simple format log
#ifdef ANDROID
#define PXL_LOGT_SIMPLE(priority, fmt, ...) do { \
       __android_log_print(priority, PXL_MODULE_TAG, fmt, ##__VA_ARGS__); \
       fprintf(stderr, (fmt "\n"), ##__VA_ARGS__); \
    } while(0)
#else
#define PXL_LOGT_SIMPLE(priority, fmt, ...) do { \
        fprintf(stderr, (fmt "\n"), ##__VA_ARGS__); \
    } while(0)
#endif

//--------------------------------------------------------------------------------
// => log template for each level
//--------------------------------------------------------------------------------
// NOTE: if using stderr, `run-and-install.sh`(start on PC, run on Android) will mess up log order
// For better log order, always use stderr since it is not buffered
#if PXL_LOGFMT_FULL
#define PXL_LOGDT_FULL(fmt, ...) PXL_LOGT_FULL(PIXEL_LOG_DEBUG, fmt, PXL_MODULE_TAG, stderr, "\x1b[36m", ##__VA_ARGS__)
#define PXL_LOGIT_FULL(fmt, ...) PXL_LOGT_FULL(PIXEL_LOG_INFO,  fmt, PXL_MODULE_TAG, stderr, "\x1b[32m", ##__VA_ARGS__)
#define PXL_LOGWT_FULL(fmt, ...) PXL_LOGT_FULL(PIXEL_LOG_WARN,  fmt, PXL_MODULE_TAG, stderr, "\x1b[33m", ##__VA_ARGS__)
#define PXL_LOGET_FULL(fmt, ...) PXL_LOGT_FULL(PIXEL_LOG_ERROR, fmt, PXL_MODULE_TAG, stderr, "\x1b[31m", ##__VA_ARGS__)
#elif PXL_LOGFMT_MEDIUM
#define PXL_LOGDT_MEDIUM(fmt, ...) PXL_LOGT_MEDIUM(PIXEL_LOG_DEBUG, fmt, PXL_MODULE_TAG, stderr, "\x1b[36m", ##__VA_ARGS__)
#define PXL_LOGIT_MEDIUM(fmt, ...) PXL_LOGT_MEDIUM(PIXEL_LOG_INFO,  fmt, PXL_MODULE_TAG, stderr, "\x1b[32m", ##__VA_ARGS__)
#define PXL_LOGWT_MEDIUM(fmt, ...) PXL_LOGT_MEDIUM(PIXEL_LOG_WARN,  fmt, PXL_MODULE_TAG, stderr, "\x1b[33m", ##__VA_ARGS__)
#define PXL_LOGET_MEDIUM(fmt, ...) PXL_LOGT_MEDIUM(PIXEL_LOG_ERROR, fmt, PXL_MODULE_TAG, stderr, "\x1b[31m", ##__VA_ARGS__)
#elif PXL_LOGFMT_SIMPLE
#define PXL_LOGDT_SIMPLE(fmt, ...) PXL_LOGT_SIMPLE(PIXEL_LOG_DEBUG, fmt, ##__VA_ARGS__)
#define PXL_LOGIT_SIMPLE(fmt, ...) PXL_LOGT_SIMPLE(PIXEL_LOG_INFO,  fmt, ##__VA_ARGS__)
#define PXL_LOGWT_SIMPLE(fmt, ...) PXL_LOGT_SIMPLE(PIXEL_LOG_WARN,  fmt, ##__VA_ARGS__)
#define PXL_LOGET_SIMPLE(fmt, ...) PXL_LOGT_SIMPLE(PIXEL_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
#pragma message("Please define one of PXL_LOGFMT_FULL, PXL_LOGFMT_MEDIUM or PXL_LOGFMT_SIMPLE") 
#endif

//--------------------------------------------------------------------------------
// => specify each log level's template
// [user setting]
//--------------------------------------------------------------------------------
// use full format template
#if PXL_LOGFMT_FULL
#define PXL_LOGDT(fmt, ...) PXL_LOGDT_FULL(fmt, ##__VA_ARGS__)
#define PXL_LOGIT(fmt, ...) PXL_LOGIT_FULL(fmt, ##__VA_ARGS__)
#define PXL_LOGWT(fmt, ...) PXL_LOGWT_FULL(fmt, ##__VA_ARGS__)
#define PXL_LOGET(fmt, ...) PXL_LOGET_FULL(fmt, ##__VA_ARGS__)
#elif PXL_LOGFMT_MEDIUM
#define PXL_LOGDT(fmt, ...) PXL_LOGDT_MEDIUM(fmt, ##__VA_ARGS__)
#define PXL_LOGIT(fmt, ...) PXL_LOGIT_MEDIUM(fmt, ##__VA_ARGS__)
#define PXL_LOGWT(fmt, ...) PXL_LOGWT_MEDIUM(fmt, ##__VA_ARGS__)
#define PXL_LOGET(fmt, ...) PXL_LOGET_MEDIUM(fmt, ##__VA_ARGS__)
#elif PXL_LOGFMT_SIMPLE
#define PXL_LOGIT(fmt, ...) PXL_LOGIT_SIMPLE(fmt, ##__VA_ARGS__)
#define PXL_LOGDT(fmt, ...) PXL_LOGDT_SIMPLE(fmt, ##__VA_ARGS__)
#define PXL_LOGWT(fmt, ...) PXL_LOGWT_SIMPLE(fmt, ##__VA_ARGS__)
#define PXL_LOGET(fmt, ...) PXL_LOGET_SIMPLE(fmt, ##__VA_ARGS__)
#else
#pragma message("Please define one of PXL_LOGFMT_FULL, PXL_LOGFMT_MEDIUM or PXL_LOGFMT_SIMPLE") 
#endif

//--------------------------------------------------------------------------------
// => enable log macro according to LOG_LEVEL
//--------------------------------------------------------------------------------
#if PXL_LOG_INFO_ENABLED
#define PIXEL_LOGI(fmt, ...) PXL_LOGIT(fmt, ##__VA_ARGS__)
#else
#define PIXEL_LOGI(fmt, ...)
#endif

#if PXL_LOG_DEBUG_ENABLED
#define PIXEL_LOGD(fmt, ...) PXL_LOGDT(fmt, ##__VA_ARGS__)
#else
#define PIXEL_LOGD(fmt, ...)
#endif

#if PXL_LOG_WARN_ENABLED
#define PIXEL_LOGW(fmt, ...) PXL_LOGWT(fmt, ##__VA_ARGS__)
#else
#define PIXEL_LOGW(fmt, ...)
#endif

#if PXL_LOG_ERROR_ENABLED
#define PIXEL_LOGE(fmt, ...) PXL_LOGET(fmt, ##__VA_ARGS__)
#else
#define PIXEL_LOGE(fmt, ...)
#endif

#if PXL_LOG_ERROR_ENABLED
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