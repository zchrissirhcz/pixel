#include "px_timer.h"
//#include <iostream>
#if _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace pixel {

#if 0
class AutoTimer::Impl
{
public:
    double GetElapsed() const
    {
#ifdef _WIN32
        return (GetTickCount() - mStartTime) / 1e3;
#else
        struct timeval end_time;
        gettimeofday(&end_time, NULL);
        double t1 = mStartTime.tv_usec / 1e6 + mStartTime.tv_sec;
        double t2 = end_time.tv_usec / 1e6 + end_time.tv_sec;
        return t2 - t1;
#endif
    }

    std::string mName;
#ifdef _WIN32
    DWORD mStartTime;
#else
    struct timeval mStartTime;
#endif
};

AutoTimer::AutoTimer(const std::string& name):
    mImpl(new AutoTimer::Impl())
{
    mImpl->mName = name;
#ifdef _WIN32
    mImpl->mStartTime = GetTickCount();
#else
    gettimeofday(&mImpl->mStartTime, NULL);
#endif
}

AutoTimer::~AutoTimer()
{
    // std::cout << mImpl->mName << ": took " << 
    //     << " secs" << std::endl;
    fprintf(stderr, "%s: took %lf secs\n", mImpl->GetElapsed());
    delete mImpl;
    mImpl = NULL;
}

#else

class AutoTimer::Impl
{
public:
    double GetElapsed() const
    {
        return getCurrentTime() - mStartTime;
    }

    static inline double getCurrentTime()
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

    std::string mName;

    double mStartTime;
};

AutoTimer::AutoTimer(const std::string& name):
    mImpl(new AutoTimer::Impl())
{
    mImpl->mName = name;
    mImpl->mStartTime = AutoTimer::Impl::getCurrentTime();
}

AutoTimer::~AutoTimer()
{
    // std::cout << mImpl->mName << ": took " << mImpl->GetElapsed()
    //     << " ms" << std::endl;
    fprintf(stderr, "%s: took %lf ms\n", mImpl->mName.c_str(), mImpl->GetElapsed());
    delete mImpl;
    mImpl = NULL;
}

} // namespace pixel

#endif 