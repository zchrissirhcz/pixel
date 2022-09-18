#include "zcnn_random.h"
#include <stdlib.h>
#include <math.h>

/// https://zhuanlan.zhihu.com/p/97905713
/// @param E: mean
/// @param V: variance
float gauss_rand(float E, float V)
{
    static float V1, V2, S;
    static int phase = 0;
    float X;
    if (phase == 0) {
        do {
            // TODO: don't use RAND_MAX.
            float U1 = (float)rand() / (1.0f*RAND_MAX);
            float U2 = (float)rand() / (1.0f*RAND_MAX);

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while (S >= 1 || S == 0);
        X = V1 * sqrtf(-2 * logf(S) / S);
    }
    else {
        X = V2 * sqrtf(-2 * logf(S) / S);
    }
    phase = 1 - phase;
    return X = X * V + E;
}
