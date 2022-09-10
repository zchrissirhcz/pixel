#include "px_timer.h"
#include "px_log.h"

#if _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace pixel {

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

public:
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
    PX_LOGE("%s: took %lf ms", mImpl->mName.c_str(), mImpl->GetElapsed());
    delete mImpl;
    mImpl = NULL;
}

} // namespace pixel
