#include <stdio.h>

//method1
//#define LOGD printf
//能支持format。简单粗暴快速
//缺点：只有print，没有fprintf(stderr)，也不能自动添加换行符、行号等

// 使用...表示宏的不定参数。不过行号和文件名并没有被输出，用户自行指定的format也没有被用上
//#define LOGD(...) printf("%s line=%d file=%s\n", ##__VA_ARGS__, __LINE__, __FILE__)

//把用户指定的format单独拿出来
//但如果用户的format串里没有格式，也就是LOGD()里头只有一个双引号引起来的部分，此时编译失败
//#define LOGD(fmt, ...) printf(fmt, __VA_ARGS__)

// 使用##__VA_ARGS__, 避免了LOGD("xxx")编译报错
//#define LOGD(fmt, ...) printf(fmt, ##__VA_ARGS__)

// 使用()把预定义的格式串和用户输入的格式串包起来，隐式构造了完整的格式串
// 这使得希望固定增加的行号、文件名、换行得以输出
//#define LOGD(fmt, ...) printf( ("line=%d file=%s " fmt "\n"), __LINE__, __FILE__,  ##__VA_ARGS__)

// 使用fprintf而不是printf，能够区分stdout和stderr，可分别用于debug和error两级log
// 当集成多个算法模块时，每个模块的log打印，应当有独立tag，用于和别的模块区分开来，便于搜索和快速定位问题
// 先定义一个LOGDT表示Debug级别的log宏的模板，允许传入fmt和tag；然后定义LOGD为：tag是DEFAULT_TAG的LOGDT调用
// 使用__PRETTY_FUNCTION__ 打印出函数名和传入的参数类型
//#define DEFAULT_TAG "pixel"
//#define LOGDT(fmt, tag, ...) fprintf(stdout, ("Debug/%s: %s [File %s][Line %d] " fmt "\n"), tag, __PRETTY_FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
//#define LOGD(fmt, ...) LOGDT(fmt, DEFAULT_TAG, ##__VA_ARGS__)

// android平台NDK的打印，包括两种情况：
// 1)集成在APP里的.so，此时用__android_log_print函数，打印内容在logcat里看到
// 2)console application，在root过的android手机上，或者专门的开发板上，编译出的可执行程序里，此时__android_log_print打印不会被看到，仍然需要fprintf
// 这两种情况的区分，无法通过编译器预定义的宏来判断，因此两种函数的输出都要调用；行尾使用\来连接多次函数调用
//#define DEFAULT_TAG "pixel"
//#ifdef ANDROID
//#include <android/log.h>
//#define LOGDT(fmt, tag, ...) \
//    __android_log_print(ANDROID_LOG_DEBUG, tag, ("%s: %s [File %s][Line %d] " fmt "\n"), __PRETTY_FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__); \
//    fprintf(stdout, ("Debug/%s: %s [File %s][Line %d] " fmt "\n"), tag, __PRETTY_FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
//#define LOGD(fmt, ...) LOGDT(fmt, DEFAULT_TAG, ##__VA_ARGS__)
//#else
//#define LOGDT(fmt, tag, ...) \
//    fprintf(stdout, ("Debug/%s: %s [File %s][Line %d] " fmt "\n"), tag, __PRETTY_FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__)
//#endif // ANDROID
//#define LOGD(fmt, ...) LOGDT(fmt, DEFAULT_TAG, ##__VA_ARGS__)

// 在STDIO情况下的log，带颜色的话，可以区分不同level的警告；通常error是红色，是必须消除的warning；
// 基于ANSI的颜色转义规则可以做到这点；android apk里头的颜色，我们暂时不管;
// 简单起见，先只考虑debug级log的颜色
//#define log_colors_debug "\x1b[94m"
//#define LOGD(fmt, ...) \
//    fprintf(stdout, ("%s[%-5s]\x1b[0m" fmt "\n"), log_colors_debug, "debug", ##__VA_ARGS__)


//#define LOGD(fmt, ...) fprintf(stdout, (fmt "\n"), ##__VA_ARGS__)
//#define LOGE(fmt, ...) fprintf(stderr, (fmt "\n"), ##__VA_ARGS__)

//#define LOGT(fd, fmt, ...) fprintf(fd, (fmt "\n"), ##__VA_ARGS__)
//#define LOGD(fmt, ...) LOGT(stdout, fmt, ##__VA_ARGS__)
//#define LOGE(fmt, ...) LOGT(stderr, fmt, ##__VA_ARGS__)

//#define DEFAULT_TAG "pixel"
//#define LOGT(fd, tag, fmt, ...) fprintf(fd, ("%s/[File %s][Line %d] %s" fmt "\n"), tag, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
//#define LOGD(fmt, ...) LOGT(stdout, DEFAULT_TAG, fmt, ##__VA_ARGS__)
//#define LOGE(fmt, ...) LOGT(stderr, DEFAULT_TAG, fmt, ##__VA_ARGS__)

//#define DEFAULT_TAG "pixel"
//#define LOGT(level, fmt, tag, fd, ...) fprintf(fd, ("%s/%s [File %s][Line %d] %s" fmt "\n"), tag, level, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
//#define LOGD(fmt, ...) LOGT("DEBUG", fmt, DEFAULT_TAG, stdout, ##__VA_ARGS__)
//#define LOGE(fmt, ...) LOGT("ERROR", fmt, DEFAULT_TAG, stderr, ##__VA_ARGS__)

//#define DEFAULT_TAG "pixel"
//#define LOGT(level, fmt, tag, fd, color, ...) fprintf(fd, ("%s/ %s[%-5s]\x1b[0m [File %s][Line %d] %s" fmt "\n"), tag, color, level, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
//#define LOGD(fmt, ...) LOGT("DEBUG", fmt, DEFAULT_TAG, stdout, "\x1b[36m", ##__VA_ARGS__)
//#define LOGE(fmt, ...) LOGT("ERROR", fmt, DEFAULT_TAG, stderr, "\x1b[31m", ##__VA_ARGS__)

// 显示当前运行时间，也是很有用的信息
//#include <time.h>
//static inline char* timenow() {
//    time_t rawtime = time(NULL);
//    struct tm* timeinfo = localtime(&rawtime);
//    static char now[64];
//    // if timezone required, use %z
//    //now[strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S(%z)", timeinfo)] = '\0';
//    now[strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S", timeinfo)] = '\0';
//    return now;
//}
//#define LOGT(fmt, fd, ...) fprintf(fd, ("%s " fmt "\n"), timenow(), ##__VA_ARGS__)
//#define LOGD(fmt, ...) LOGT(fmt, stdout, ##__VA_ARGS__)
//#define LOGE(fmt, ...) LOGT(fmt, stderr, ##__VA_ARGS__)

//#include <time.h>
//static inline char* timenow() {
//    time_t rawtime = time(NULL);
//    struct tm* timeinfo = localtime(&rawtime);
//    static char now[64];
//    // if timezone required, use %z
//    //now[strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S(%z)", timeinfo)] = '\0';
//    now[strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S", timeinfo)] = '\0';
//    return now;
//}
//#define DEFAULT_TAG "pixel"
//#define LOGT(level, fmt, tag, fd, color, ...) fprintf(fd, ("%s | %s | %s[%-5s]\x1b[0m [File %s][Line %d] %s" fmt "\n"), timenow(), tag, color, level, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
//#define LOGD(fmt, ...) LOGT("DEBUG", fmt, DEFAULT_TAG, stdout, "\x1b[36m", ##__VA_ARGS__)
//#define LOGE(fmt, ...) LOGT("ERROR", fmt, DEFAULT_TAG, stderr, "\x1b[31m", ##__VA_ARGS__)

// 似乎把这么全面格式的宏定义在一行内实现，可维护性并不那么好，考虑参照如下开源项目改进
// https://github.com/dmcrodrigues/macro-logger/blob/master/macrologger.h
// 该项目中还定义了 LOG_IF_ERROR 宏，这和Caffe中的CHECK_IF，或者ASSERT_IF，比较类似



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
#define _PXL_LOGFMT_FULL            1
#define _PXL_LOGFMT_MEDIUM          0
#define _PXL_LOGFMT_SIMPLE          0

//--------------------------------------------------------------------------------
// => filename
//--------------------------------------------------------------------------------
// only filename
#define _PXL_FILE strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__
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
#define _PXL_LOG_FULL_FMT(fmt)                   ("%s | %s | %s[%-5s]\x1b[0m | %s, line %d, %s | " fmt "\n")
#define _PXL_LOG_FULL_ARGS(tag, color, priority) timenow(), tag, color, g_pixel_log_priority_str[priority], _PXL_FILE, __LINE__, _PXL_FUNCTION

// the param `fmt` is user specified format
// _PXL_LOG_MEDIUM_FMT(fmt) ammed pre-defined stuffs (filename, line num)
#define _PXL_LOG_MEDIUM_FMT(fmt)                   ("%s/%s%-5s\x1b[0m | %s, line %d | " fmt "\n")
#define _PXL_LOG_MEDIUM_ARGS(tag, color, priority) tag, color, g_pixel_log_priority_str[priority], _PXL_FILE, __LINE__


//--------------------------------------------------------------------------------
// => log template for all levels
//--------------------------------------------------------------------------------
// the full format log
#ifdef ANDROID
#define _PXL_LOGT_FULL(priority, fmt, tag, fd, color, ...) do { \
        __android_log_print(priority, tag, _PXL_LOG_FULL_FMT(fmt), _PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, _PXL_LOG_FULL_FMT(fmt), _PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
    } while(0)
#else
#define _PXL_LOGT_FULL(priority, fmt, tag, fd, color, ...) do { \
        fprintf(fd, _PXL_LOG_FULL_FMT(fmt), _PXL_LOG_FULL_ARGS(tag, color, priority), ##__VA_ARGS__); \
    } while(0)
#endif

// the medium format log
#ifdef ANDROID
#define _PXL_LOGT_MEDIUM(priority, fmt, tag, fd, color, ...) do { \
        __android_log_print(priority, tag, _PXL_LOG_MEDIUM_FMT(fmt), _PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
        fprintf(fd, _PXL_LOG_MEDIUM_FMT(fmt), _PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
    } while(0)
#else
#define _PXL_LOGT_MEDIUM(priority, fmt, tag, fd, color, ...) do { \
        fprintf(fd, _PXL_LOG_MEDIUM_FMT(fmt), _PXL_LOG_MEDIUM_ARGS(tag, color, priority), ##__VA_ARGS__); \
    } while(0)
#endif

// simple format log
#ifdef ANDROID
#define _PXL_LOGT_SIMPLE(priority, fmt, ...) do { \
       __android_log_print(priority, _PXL_MODULE_TAG, fmt, ##__VA_ARGS__); \
       fprintf(stdout, (fmt "\n"), ##__VA_ARGS__); \
    } while(0)
#else
#define _PXL_LOGT_SIMPLE(priority, fmt, ...) do { \
        fprintf(stdout, (fmt "\n"), ##__VA_ARGS__); \
    } while(0)
#endif


//--------------------------------------------------------------------------------
// => log template for each level
//--------------------------------------------------------------------------------
// NOTE: if using stderr, `run-and-install.sh`(start on PC, run on Android) will mess up log order
// For better log order, always use stdout
#if _PXL_LOGFMT_FULL
#define _PXL_LOGDT_FULL(fmt, ...) _PXL_LOGT_FULL(PIXEL_LOG_DEBUG, fmt, _PXL_MODULE_TAG, stdout, "\x1b[36m", ##__VA_ARGS__)
#define _PXL_LOGIT_FULL(fmt, ...) _PXL_LOGT_FULL(PIXEL_LOG_INFO,  fmt, _PXL_MODULE_TAG, stdout, "\x1b[32m", ##__VA_ARGS__)
#define _PXL_LOGWT_FULL(fmt, ...) _PXL_LOGT_FULL(PIXEL_LOG_WARN,  fmt, _PXL_MODULE_TAG, stdout, "\x1b[33m", ##__VA_ARGS__)
#define _PXL_LOGET_FULL(fmt, ...) _PXL_LOGT_FULL(PIXEL_LOG_ERROR, fmt, _PXL_MODULE_TAG, stdout, "\x1b[31m", ##__VA_ARGS__)
#elif _PXL_LOGFMT_MEDIUM
#define _PXL_LOGDT_MEDIUM(fmt, ...) _PXL_LOGT_MEDIUM(PIXEL_LOG_DEBUG, fmt, _PXL_MODULE_TAG, stdout, "\x1b[36m", ##__VA_ARGS__)
#define _PXL_LOGIT_MEDIUM(fmt, ...) _PXL_LOGT_MEDIUM(PIXEL_LOG_INFO,  fmt, _PXL_MODULE_TAG, stdout, "\x1b[32m", ##__VA_ARGS__)
#define _PXL_LOGWT_MEDIUM(fmt, ...) _PXL_LOGT_MEDIUM(PIXEL_LOG_WARN,  fmt, _PXL_MODULE_TAG, stdout, "\x1b[33m", ##__VA_ARGS__)
#define _PXL_LOGET_MEDIUM(fmt, ...) _PXL_LOGT_MEDIUM(PIXEL_LOG_ERROR, fmt, _PXL_MODULE_TAG, stdout, "\x1b[31m", ##__VA_ARGS__)
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
#else
#define PIXEL_LOG_IF_ERROR(condition, fmt, ...)
#endif

void hello(const char* name) {
    PIXEL_LOGD("hello, %s", name);
}

int main() {

    PIXEL_LOGD("hello world %d", 3);
    const char* str = "hello";
    int n = 10;
    PIXEL_LOGD("num %d, str %s", n, str);
    PIXEL_LOGD("%d", 3);
    hello("ChrisZZ");
    PIXEL_LOGE("this is an error message");
    PIXEL_LOGI("this is an info message");

    n=3;
    PIXEL_LOG_IF_ERROR(n!=0, "n is not zero");

    return 0;
}