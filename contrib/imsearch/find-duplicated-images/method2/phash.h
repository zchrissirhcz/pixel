#pragma once


#include <bitset>
#include <string>
#include <opencv2/opencv.hpp>

void get_phash(const cv::Mat& image, std::bitset<64>& feature);
std::string get_phash(cv::Mat& image);

