#pragma once

#if __ANDROID__
const char* project_dir = "/data/local/tmp";
#elif __linux__
static const char* project_dir = "@CMAKE_SOURCE_DIR@/legacy/NaiveCNN";
#elif _MSC_VER
static const char* project_dir = "F:/zhangzhuo/dev/NaiveCNN/";
#elif __APPLE__ && !(__ARM_NEON)
static const char* project_dir = "/Users/chris/work/gitee/NaiveCNN/";
#elif __APPLE__ && __ARM_NEON
static const char* project_dir = "/Users/zz/work/pixel/legacy/NaiveCNN/";
#else
#pragma error
#endif
