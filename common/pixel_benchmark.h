#include <time.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#else // _WIN32
#include <sys/time.h>
#endif // _WIN32

//--------------------------------------------------------------------------------
// declaration
//--------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

static inline double get_current_time();

#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------------
// implementation
//--------------------------------------------------------------------------------
#ifdef _WIN32
static inline double get_current_time()
{
    LARGE_INTEGER freq;
    LARGE_INTEGER pc;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&pc);

    return pc.QuadPart * 1000.0 / freq.QuadPart;
}
#else // _WIN32
// expected to be more accurate
static inline double get_current_time()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec/1000000.0;
}
// not that accurate
static inline double get_current_time2()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
}
#endif // _WIN32