#pragma once

#ifdef __cplusplus
extern "C" {
#endif

double px_get_current_time();
void px_sleep(int milliseconds);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <string>

namespace pixel {

class AutoTimer
{
public:
    explicit AutoTimer(const std::string& name);
    ~AutoTimer();
private:
    AutoTimer(const AutoTimer&);
    AutoTimer& operator=(const AutoTimer&);
    class Impl;
    Impl* mImpl;
};

} // namespace pixel

#endif // __cplusplus