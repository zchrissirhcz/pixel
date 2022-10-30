#pragma once

#include <stdio.h>

#if __ANDROID_API__ >= 8
#include <android/log.h>
#define PX_LOGE(...) do { \
    fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); \
    __android_log_print(ANDROID_LOG_WARN, "pixel", ##__VA_ARGS__); } while(0)
#else // __ANDROID_API__ >= 8
#include <stdio.h>
#define PX_LOGE(...) do { \
    fprintf(stderr, ##__VA_ARGS__); fprintf(stderr, "\n"); } while(0)
#endif // __ANDROID_API__ >= 8

// the simples
// #define PX_LOGE(...) fprintf(stderr, ##__VA_ARGS__)