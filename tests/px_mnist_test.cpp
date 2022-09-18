#include "gtest/gtest.h"
#include "px_filesystem.h"
#include "px_mnist.h"

#if __ANDROID__
const char* project_dir = "/data/local/tmp";
#elif __linux__
static const char* project_dir = "/home/zz/work/NaiveCNN/";
#elif _MSC_VER
static const char* project_dir = "F:/zhangzhuo/dev/NaiveCNN/";
#elif __APPLE__ && !(__ARM_NEON)
static const char* project_dir = "/Users/chris/work/gitee/NaiveCNN/";
#elif __APPLE__ && __ARM_NEON
static const char* project_dir = "/Users/zz/work/pixel/NaiveCNN/";
#else
#pragma error
#endif

TEST(mnist, extract_and_save_image)
{
    std::string mnist_data_dir = std::string(project_dir) + "/mnist";
    std::string save_dir = mnist_data_dir + "/testImgs";

    const char* mnist_filename = "t10k-images.idx3-ubyte";

    px_extract_mnist_image_and_save(mnist_data_dir.c_str(), mnist_filename, save_dir.c_str());
}