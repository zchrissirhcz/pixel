#include "gtest/gtest.h"
#include "px_filesystem.h"
#include "px_mnist.h"
#include "px_directories.h"

TEST(mnist, extract_and_save_image)
{
    std::string mnist_data_dir = project_dir + "/NaiveCNN/mnist";
    std::string save_dir = mnist_data_dir + "/testImgs";

    const char* mnist_filename = "t10k-images.idx3-ubyte";

    px_extract_mnist_image_and_save(mnist_data_dir.c_str(), mnist_filename, save_dir.c_str());
}