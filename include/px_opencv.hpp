#pragma once

#include <opencv2/opencv.hpp>
#include "px_log.h"

static
bool isImageNearlyEqual(const cv::Mat expected, const cv::Mat actual, const uint32_t tolerance=0)
{
    if ( (expected.rows!=actual.rows) ||
         (expected.cols!=actual.cols) ||
         (expected.channels()!=actual.channels())
    )
    {
        return false;
    }

    const int h = expected.rows;
    const int w = expected.cols;
    const int c = expected.channels();
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            for (int k = 0; k < c; k++)
            {
                const int idx = i * expected.step + j * c + k;
                uint32_t diff = abs(expected.data[idx] - actual.data[idx]);
                if (diff > tolerance)
                {
                    PX_LOGE("pixel not equal, actual[%d,%d,%d](%d)!=expected[%d,%d,%d](%d)\n",
                        i, j, k, actual.data[idx],
                        i, j, k, expected.data[idx]
                    );
                    return false;
                }
            }
        }
    }
    return true;
}