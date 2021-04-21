
//#include <opencv2/opencv.hpp>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "copy_make_border.h"

using namespace cv;


int main2( int argc, char** argv )
{
    // Declare the variables
    Mat src, dst;
    int top, bottom, left, right;
    int borderType = BORDER_CONSTANT;
    const char* window_name = "copyMakeBorder Demo";
    RNG rng(12345);


    const char* imageName = argc >=2 ? argv[1] : "IU.bmp";
    // Loads an image
    src = imread( samples::findFile( imageName ), IMREAD_COLOR ); // Load an image
    // Check if image is loaded fine
    if( src.empty()) {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default lena.jpg] \n");
        return -1;
    }
    // Brief how-to for this program
    printf( "\n \t copyMakeBorder Demo: \n" );
    printf( "\t -------------------- \n" );
    printf( " ** Press 'c' to set the border to a random constant value \n");
    printf( " ** Press 'r' to set the border to be replicated \n");
    printf( " ** Press 'ESC' to exit the program \n");
    namedWindow( window_name );
    // Initialize arguments for the filter
    top = (int) (0.05*src.rows); bottom = top;
    left = (int) (0.05*src.cols); right = left;
    for(;;)
    {
        Scalar value( rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255) );
        copyMakeBorder( src, dst, top, bottom, left, right, borderType, value );
        imshow( window_name, dst );
        char c = (char)waitKey(500);
        if( c == 27 )
        { break; }
        else if( c == 'c' )
        { borderType = BORDER_CONSTANT; }
        else if( c == 'r' )
        { borderType = BORDER_REPLICATE; }
    }
    return 0;
}

// CV_EXPORTS_W void copyMakeBorder(InputArray src, OutputArray dst,
//                                  int top, int bottom, int left, int right,
//                                  int borderType, const Scalar& value = Scalar() );

int main()
{
    cv::Mat src_image = cv::imread("IU.bmp");
    cv::Size src_size = src_image.size();

    int border = 10;

    int top = border;
    int bottom = border;
    int left = border;
    int right = border;

    int src_height = src_size.height;
    int src_width = src_size.width;

    cv::Size dst_size;
    dst_size.height = src_size.height + top + bottom;
    dst_size.width = src_size.width + left + right;

    cv::Mat dst_image_opencv(dst_size, src_image.type());
    cv::Mat dst_image_naive(dst_size, src_image.type());

    cv::copyMakeBorder(src_image, dst_image_opencv, top, bottom, left, right, cv::BORDER_CONSTANT);

    //-----------------------------

    int channels = src_image.channels();
    unsigned char* dst = dst_image_naive.data;
    unsigned char* src = src_image.data;

    copy_make_border_naive(src, src_height, src_width, channels, dst, top, bottom, left, right);

    cv::imwrite("IU_withborder_opencv.png", dst_image_opencv);
    cv::imwrite("IU_withborder_naive.png", dst_image_naive);

    return 0;
}