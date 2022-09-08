void test()
{

    Mat kH = (cv::Mat_<double>(1, 3) << -1, 0, 1); // differential kernel in x
    Mat Dx;
    filter2D(gray, Dx, CV_32F, kH, cv::Point(-1, -1), 0);
}

int main() {

    

    return 0;
}