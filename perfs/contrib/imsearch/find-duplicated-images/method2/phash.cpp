#include "phash.h"

// https://www.ruanyifeng.com/blog/2011/07/principle_of_similar_image_search.html
// https://www.hackerfactor.com/blog/index.php?/archives/432-Looks-Like-It.html
void get_phash(const cv::Mat& image, std::bitset<64>& feature) {
    // step1 resize to 8x8
    cv::Size size(8, 8);
    cv::Mat small(size, image.type());
    cv::resize(image, small, size);

    // step2 color image to grayscale image
    cv::Mat gray(size, CV_8UC1);
    if (small.type() == CV_8UC1)
    {
        gray = small;
    }
    else {
        cv::cvtColor(small, gray, cv::COLOR_BGR2GRAY);
    }

    // step3 get mean value of gray image
    cv::Scalar mean_scalar = cv::mean(gray);
    uchar mean = static_cast<uchar>(mean_scalar[0]);
    feature.reset(); // all bit set to 0
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            int pos = i * 8 + j;
            if (gray.at<uchar>(i,j) > mean) {
                feature.set(pos, 1);
            }
        }
    }
}

std::string get_phash(cv::Mat& image) {
    std::bitset<64> feature;
    get_phash(image, feature);
    return feature.to_string();
}

int hamming_distance(const std::string& feature, const std::string& feature2)
{
    if (feature.length() != feature2.length()) {
        return -1;
    }
    if (feature.length() == 0 || feature2.length() == 0) {
        return -2;
    }
    int len = feature.length();
    int dist = 0;
    for (int i=0; i<len; i++) {
        if (feature[i]!=feature2[i]) {
            dist ++;
        }
    }
    return dist;
}

#ifdef UNIT_TEST

#include <iostream>

static int test() {
    //std::string image_name = "000001.png";
    std::string image_name = "Alyson_Hannigan_200512.jpg";
    cv::Mat image = cv::imread(image_name);
    //cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    std::bitset<64> feature;
    get_phash(image, feature);
    unsigned long long res = feature.to_ullong();
    std::cout << "feature is: " << feature << " to_ullong()=" << res << std::endl;
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            int pos = i*8 + j;
            std::cout << feature.test(pos) << " ";
        }
        std::cout << endl;
    }
    return 0;
}

#endif
