#include <benchmark/benchmark.h>
#include <opencv2/opencv.hpp>
#include "rgb2gray.h"

class Rgb2grayFixture : public benchmark::Fixture {
public:
    void SetUp(const ::benchmark::State& state) {
        src = cv::imread("sky.jpg");
        assert(!src.empty());
        cv::cvtColor(src, src, cv::COLOR_BGR2RGB);
        rgb_buf = src.data;
        height = src.rows;
        width = src.cols;
        res = cv::Mat(src.size(), CV_8UC1);
        gray_buf = res.data;

        //TODO: bind big core??
        
    }

    void TearDown(const ::benchmark::State& state) {
    }

    cv::Mat src;
    unsigned char* rgb_buf;
    unsigned char* gray_buf;
    size_t height;
    size_t width;
    cv::Mat res;
};

BENCHMARK_DEFINE_F(Rgb2grayFixture, naive)(benchmark::State& st) {
    for (auto _ : st) {
        pixel_rgb2gray_naive(rgb_buf, height, width, gray_buf);
    }
}
BENCHMARK_REGISTER_F(Rgb2grayFixture, naive)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(Rgb2grayFixture, fixed)(benchmark::State& st) {
    for (auto _ : st) {
        pixel_rgb2gray_fixed(rgb_buf, height, width, gray_buf);
    }
}
BENCHMARK_REGISTER_F(Rgb2grayFixture, fixed)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(Rgb2grayFixture, fixed_asimd)(benchmark::State& st) {
    for (auto _ : st) {
        pixel_rgb2gray_fixed_asimd(rgb_buf, height, width, gray_buf);
    }
}
BENCHMARK_REGISTER_F(Rgb2grayFixture, fixed_asimd)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(Rgb2grayFixture, opencv)(benchmark::State& st) {
    for (auto _ : st) {
        cv::Mat dst;
        cv::cvtColor(src, dst, cv::COLOR_RGB2GRAY);
    }
}
BENCHMARK_REGISTER_F(Rgb2grayFixture, opencv)->Unit(benchmark::kMillisecond);


BENCHMARK_DEFINE_F(Rgb2grayFixture, asm)(benchmark::State& st) {
    for (auto _ : st) {
        pixel_rgb2gray_fixed_asm(rgb_buf, height, width, gray_buf);
    }
}
BENCHMARK_REGISTER_F(Rgb2grayFixture, fixed_asimd)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();

