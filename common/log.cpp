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

#include <time.h>
static inline char* timenow() {
    time_t rawtime = time(NULL);
    struct tm* timeinfo = localtime(&rawtime);
    static char now[64];
    // if timezone required, use %z
    //now[strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S(%z)", timeinfo)] = '\0';
    now[strftime(now, sizeof(now), "%Y-%m-%d %H:%M:%S", timeinfo)] = '\0';
    return now;
}
#define DEFAULT_TAG "pixel"
#define LOGT(level, fmt, tag, fd, color, ...) fprintf(fd, ("%s | %s | %s[%-5s]\x1b[0m [File %s][Line %d] %s" fmt "\n"), timenow(), tag, color, level, __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#define LOGD(fmt, ...) LOGT("DEBUG", fmt, DEFAULT_TAG, stdout, "\x1b[36m", ##__VA_ARGS__)
#define LOGE(fmt, ...) LOGT("ERROR", fmt, DEFAULT_TAG, stderr, "\x1b[31m", ##__VA_ARGS__)

// 似乎把这么全面格式的宏定义在一行内实现，可维护性并不那么好，考虑参照如下开源项目改进
// https://github.com/dmcrodrigues/macro-logger/blob/master/macrologger.h
// 该项目中还定义了 LOG_IF_ERROR 宏，这和Caffe中的CHECK_IF，或者ASSERT_IF，比较类似



void hello(const char* name) {
    LOGD("hellol, %s", name);
}

int main() {

    LOGD("hello world %d", 3);
    const char* str = "hello";
    int n = 10;
    LOGD("num %d, str %s", n, str);
    LOGD("%d", 3);
    hello("ChrisZZ");
    LOGE("this is an error message");

    return 0;
}



