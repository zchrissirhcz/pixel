#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>
#include "flip.h"
#include "px_image.h"


//cv::flip(src, dst, mode);
//mode=0, 上下颠倒  vertically
//mode=1，左右颠倒  horizontally
//mode=-1, against center point

class FlipHorizonRgbFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state)
    {
        src = cv::imread("sky.jpg");
        assert(!src.empty());
        cv::flip(src, expected, 1);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC3);
        dst_buf = res.data;
    }

    void TearDown(const ::benchmark::State& state) {
    }

    cv::Mat src;
    unsigned char* src_buf;
    unsigned char* dst_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

BENCHMARK_DEFINE_F(FlipHorizonRgbFixture, naive)(benchmark::State& st)
{
    for (auto _ : st)
    {
        px_image_t* src_image = px_create_image_header(height, width, src.channels());
        src_image->data = src_buf;
        px_image_t* dst_image = px_create_image(height, width, src.channels());
        flip_horiz_rgb_naive(src_image, dst_image);
        px_destroy_image_header(src_image);
        px_destroy_image(dst_image);
    }
}
BENCHMARK_REGISTER_F(FlipHorizonRgbFixture, naive)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipHorizonRgbFixture, idxopt)(benchmark::State& st) {
    for (auto _ : st) {
        flip_horiz_rgb_idxopt(src_buf, height, width, dst_buf);
    }
}
BENCHMARK_REGISTER_F(FlipHorizonRgbFixture, idxopt)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipHorizonRgbFixture, asimd)(benchmark::State& st) {
    for (auto _ : st) {
        flip_horiz_rgb_asimd(src_buf, height, width, dst_buf);
    }
}
BENCHMARK_REGISTER_F(FlipHorizonRgbFixture, asimd)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipHorizonRgbFixture, opencv)(benchmark::State& st) {
    for (auto _ : st) {
        cv::flip(src, res, 1);
    }
}
BENCHMARK_REGISTER_F(FlipHorizonRgbFixture, opencv)->Unit(benchmark::kMillisecond);


class FlipHorizonGrayFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) {
        src = cv::imread("sky.jpg");
        assert(!src.empty());
        cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
        cv::flip(src, expected, 1);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC1);
        dst_buf = res.data;
    }

    void TearDown(const ::benchmark::State& state) {
    }

    cv::Mat src;
    unsigned char* src_buf;
    unsigned char* dst_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

BENCHMARK_DEFINE_F(FlipHorizonGrayFixture, naive)(benchmark::State& st) {
    for (auto _ : st) {
        flip_horiz_gray_naive(src_buf, height, width, dst_buf);
    }
}
BENCHMARK_REGISTER_F(FlipHorizonGrayFixture, naive)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipHorizonGrayFixture, asimd)(benchmark::State& st) {
    for (auto _ : st) {
        flip_horiz_gray_asimd(src_buf, height, width, dst_buf);
    }
}
BENCHMARK_REGISTER_F(FlipHorizonGrayFixture, asimd)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipHorizonGrayFixture, opencv)(benchmark::State& st) {
    for (auto _ : st) {
        cv::flip(src, res, 1);
    }
}
BENCHMARK_REGISTER_F(FlipHorizonGrayFixture, opencv)->Unit(benchmark::kMillisecond);


class FlipVerticalRgbFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) {
        src = cv::imread("sky.jpg");
        assert(!src.empty());
        cv::flip(src, expected, 0);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC3);
        dst_buf = res.data;
    }

    void TearDown(const ::benchmark::State& state) {
    }

    cv::Mat src;
    unsigned char* src_buf;
    unsigned char* dst_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

BENCHMARK_DEFINE_F(FlipVerticalRgbFixture, naive)(benchmark::State& st) {
    for (auto _ : st) {
        flip_vert_rgb_naive(src_buf, height, width, dst_buf);
    }
}
BENCHMARK_REGISTER_F(FlipVerticalRgbFixture, naive)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipVerticalRgbFixture, opencv)(benchmark::State& st) {
    for (auto _ : st) {
        cv::flip(src, res, 0);
    }
}
BENCHMARK_REGISTER_F(FlipVerticalRgbFixture, opencv)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipVerticalRgbFixture, opencv_inplace)(benchmark::State& st) {
    for (auto _ : st) {
        res = src;
        cv::flip(res, res, 0);
    }
}
BENCHMARK_REGISTER_F(FlipVerticalRgbFixture, opencv_inplace)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipVerticalRgbFixture, bylines)(benchmark::State& st) {
    for (auto _ : st) {
        flip_vert_rgb_bylines(src_buf, height, width, dst_buf);
    }
}
BENCHMARK_REGISTER_F(FlipVerticalRgbFixture, bylines)->Unit(benchmark::kMillisecond);


class FlipVerticalGrayFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) {
        src = cv::imread("sky.jpg");
        assert(!src.empty());
        cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
        cv::flip(src, expected, 0);
        src_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC1);
        dst_buf = res.data;
    }

    void TearDown(const ::benchmark::State& state) {
    }

    cv::Mat src;
    unsigned char* src_buf;
    unsigned char* dst_buf;
    size_t height;
    size_t width;
    cv::Mat res;
    cv::Mat expected;
};

BENCHMARK_DEFINE_F(FlipVerticalGrayFixture, naive)(benchmark::State& st) {
    for (auto _ : st) {
        flip_vert_gray_naive(src_buf, height, width, dst_buf);
    }
}
BENCHMARK_REGISTER_F(FlipVerticalGrayFixture, naive)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipVerticalGrayFixture, bylines)(benchmark::State& st) {
    for (auto _ : st) {
        flip_vert_gray_bylines(src_buf, height, width, dst_buf);
    }
}
BENCHMARK_REGISTER_F(FlipVerticalGrayFixture, bylines)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(FlipVerticalGrayFixture, opencv)(benchmark::State& st) {
    for (auto _ : st) {
        cv::flip(src, res, 0);
    }
}
BENCHMARK_REGISTER_F(FlipVerticalGrayFixture, opencv)->Unit(benchmark::kMillisecond);


BENCHMARK_MAIN();

