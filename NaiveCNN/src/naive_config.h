#ifndef NAIVE_CNN_CONFIG_H
#define NAIVE_CNN_CONFIG_H

#define CHECK_WRITE_FILE(fp, filename) \
    if (fp==NULL) \
        printf("write file %s failed in line %d, file %s\n", filename, __LINE__, __FILE__);

#define CHECK_READ_FILE(fp, filename) \
    if (fp==NULL) \
        printf("read file %s failed in line %d, file %s\n", filename, __LINE__, __FILE__);

#define NC_MAX_PATH 256
#define NC_IMAGE_ALIGN 1 // for simplicity, we don't align now
#define NC_CHANNEL_ALIGN 1 // for simplicity, we don't align now
#define NC_MAX_BLOB_PER_LAYER 100

#if __ANDROID__
    const char* project_dir = "/data/local/tmp";
#elif __linux__
    static const char* project_dir = "/home/zz/work/NaiveCNN/";
#elif _MSC_VER
    static const char* project_dir = "F:/zhangzhuo/dev/NaiveCNN/";
#elif __APPLE__ && !(__ARM_NEON)
    static const char* project_dir = "/Users/chris/work/gitee/NaiveCNN/";
#elif __APPLE__ && __ARM_NEON
    static const char* project_dir = "/Users/zz/work/NaiveCNN/";
#else
#pragma error
#endif

static int nc_align_up(int x, int n) {
    return ((x + n - 1) / n ) * n;
}

#endif
