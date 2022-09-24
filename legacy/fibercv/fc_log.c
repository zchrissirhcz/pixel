#include <stdio.h>

//----- log -----
#include "fc_log.h"
#include <stdarg.h>

#if ANDROID
#include <android/log.h>
#define LOG_TAG "FC_LOG"
#endif

static const char* level_names[] = {
    //"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
    "DEBUG", "ERROR"
};

void fc_log(FcLogLevel level, const char* file, int line, const char* fmt, ...) {
    if (level <= kFcLogLevel_Begin || level >= kFcLogLevel_End) {
        return;
    }

//print to console for both PC & Android
#if defined(_MSC_VER) || (defined(__linux__) || defined(__APPLE__))
    va_list args;
    va_start(args, fmt);

    FILE* fout = stdout;
    if (level == kFcLogLevel_Debug) {
        fout = stdout;
    }
    else if (level == kFcLogLevel_Error) {
        fout = stderr;
    }

    time_t t = time(NULL);
    struct tm* lt = localtime(&t);
    char now[100];
    now[strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
    fprintf(fout, "%s [%-5s] %s:%d: ", now, level_names[level], file, line);

    vfprintf(fout, fmt, args);
    va_end(args);
    fprintf(fout, "\n");
    fflush(fout);

#elif defined(TI_C66)
    //we could use Vps_printf(__VA_ARGS__) to replace printf(). but here may not.
#endif

//for android, we also print to logcat
#if ANDROID
    va_start(args, fmt);
    if (level == kFcLogLevel_Debug) {
        __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, fmt, args);
    }
    else if (level == kFcLogLevel_Error) {
        __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, fmt, args);
    }
    va_end(args);
#endif

}