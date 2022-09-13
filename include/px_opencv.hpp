#pragma once

#include <opencv2/opencv.hpp>
#include "px_log.h"


// https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
// NOTE: There is `cv::typeToString()` in OpenCV since 3.4.2 and 4.0.0
// May also search the table:
// +--------+----+----+----+----+------+------+------+------+
// |        | C1 | C2 | C3 | C4 | C(5) | C(6) | C(7) | C(8) |
// +--------+----+----+----+----+------+------+------+------+
// | CV_8U  |  0 |  8 | 16 | 24 |   32 |   40 |   48 |   56 |
// | CV_8S  |  1 |  9 | 17 | 25 |   33 |   41 |   49 |   57 |
// | CV_16U |  2 | 10 | 18 | 26 |   34 |   42 |   50 |   58 |
// | CV_16S |  3 | 11 | 19 | 27 |   35 |   43 |   51 |   59 |
// | CV_32S |  4 | 12 | 20 | 28 |   36 |   44 |   52 |   60 |
// | CV_32F |  5 | 13 | 21 | 29 |   37 |   45 |   53 |   61 |
// | CV_64F |  6 | 14 | 22 | 30 |   38 |   46 |   54 |   62 |
// +--------+----+----+----+----+------+------+------+------+
static std::string mat_type_to_string(int type)
{
    std::string r;

    //uchar depth = type & CV_MAT_DEPTH_MASK;
    int depth = CV_MAT_DEPTH(type);

    //uchar channels = 1 + (type >> CV_CN_SHIFT);
    int cn = CV_MAT_CN(type);

    switch (depth)
    {
    case CV_8U: r = "8U"; break;
    case CV_8S: r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    case CV_16F: r = "16F"; break;
    default: r = "User"; break;
    }

    r += "C";
    r += (cn + '0');

    return r;
}

template<typename T, typename PT = double>
static bool assertAlmostEqual(const void* pexpected0, const void* pactual0, int y, int cols, int cn, double eps)
{
    const T* expected = (const T*)pexpected0;
    const T* actual = (const T*)pactual0;
    const int len = cols * cn;
    for (int i = 0; i < len; ++i)
    {
        const double diff = std::abs((double)expected[i] - (double)actual[i]);
        if (diff > eps)
        {
            const int x = i / cn;
            const int c = i % cn;
            // fprintf(stderr, "array[%d,%d,%d] (%f) != expected[%d, %d, %d] (%f), diff = %f, EPS = %f\n",
            //     y, x, c, (double)actual[i],
            //     y, x, c, (double)expected[i],
            //     diff, eps
            // );
            std::cerr << "array[" << y << "," << x << "," << c
                      << "] (" << (PT)actual[i] << ") != expected[" << y << "," << x << "," << c
                      << "] (" << (PT)expected[i] << "), diff = " << diff << ", EPS = " << eps << std::endl;

            return false;
        }
    }
    return true;
}

template<typename T, typename PT>
static bool almostEqual(const cv::Mat& expected, const cv::Mat& actual, double eps)
{
    if (expected.type() != actual.type())
    {
        fprintf(stderr, "actual type (%s=%d) is not same as expected type (%s=%d)\n",
                mat_type_to_string(actual.type()).c_str(), actual.type(),
                mat_type_to_string(expected.type()).c_str(), expected.type());
        return false;
    }

    if ((expected.size() != actual.size()))
    {
        fprintf(stderr, "actual size (W=%d, H=%d) is not same as expected size (W=%d, H=%d)\n",
                actual.cols, actual.rows,
                expected.cols, expected.rows);
        return false;
    }

    const int height = expected.rows;
    const int cn = expected.channels();
    for (int y = 0; y < height; ++y)
    {
        auto res = assertAlmostEqual<T, PT>(expected.ptr(y), actual.ptr(y), y, expected.cols, cn, eps);
        if (!res)
            return res;
    }
    return true;
}

static bool almostEqual(const cv::Mat& expected, const cv::Mat& actual, double eps = 0)
{
    if ((expected.rows != actual.rows) || (expected.cols != actual.cols))
    {
        fprintf(stderr, "dims not match: expected(rows=%d, cols=%d) vs actual(rows=%d, cols=%d)\n",
                expected.rows, expected.cols,
                actual.rows, actual.cols);
        return false;
    }
    if (expected.channels() != actual.channels())
    {
        fprintf(stderr, "channels not match: expected(%d) vs actual(%d)\n",
                expected.channels(),
                actual.channels());
        return false;
    }
    if (expected.type() != actual.type())
    {
        fprintf(stderr, "types not match: expected(%s) vs actual(%s)\n",
                mat_type_to_string(expected.type()).c_str(),
                mat_type_to_string(actual.type()).c_str());
        return false;
    }

    uchar depth = CV_MAT_DEPTH(expected.type());
    switch (depth)
    {
    case CV_8U: return almostEqual<uchar, int>(expected, actual, eps);
    case CV_8S: return almostEqual<schar, int>(expected, actual, eps);
    case CV_16U: return almostEqual<ushort, int>(expected, actual, eps);
    case CV_16S: return almostEqual<short, int>(expected, actual, eps);
    case CV_32S: return almostEqual<int, int>(expected, actual, eps);
    case CV_32F: return almostEqual<float, int>(expected, actual, eps);
    case CV_64F: return almostEqual<double, int>(expected, actual, eps);
    case CV_16F: fprintf(stderr, "not implemented\n"); break;
    default: fprintf(stderr, "not implemented\n"); break;
    }

    return false;
}
