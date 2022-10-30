#pragma once

#include <time.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    // random numer's seed
    static inline uint32_t get_seed() {
        return (uint32_t)time((time_t*)0);
    }


    /// @brief generate a set of number that subject to gaussian distribution
    /// Example:
    /// @code{.cpp}
    /// srand(get_seed());
    /// float mean = 0, variance = 20;
    /// for(int i=0; i<1000; i++) {
    ///     float y = gauss_rand(mean, variance);
    /// }
    /// @endcode
    float gauss_rand(float mean, float variance);

#ifdef __cplusplus
}
#endif
