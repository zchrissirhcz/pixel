#include "improc_zcx.h"
#include "px_assert.h"

#include <stdlib.h>
#include <string.h>

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stack>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846	/* pi */
#endif
//#define ROTATE_IMAGE

void cv_image_padding(unsigned char* image, int height, int width, int* pad, unsigned char* out, std::string method)
//memory must be premalloced
{
    //cv::Mat before_pad_image_(height, width, CV_8UC3, image);
    //cv::imwrite("./before_pad_image_.png", before_pad_image_);
    int channel = 3;
    int new_height = height + pad[1] + pad[3];
    int new_width = width + pad[0] + pad[2];
    if (method == "reflect") // 对应到 OpenCV 中的 BORDER_REFLECT101
    {
        for (int h = 0; h < height + pad[1] + pad[3]; h++)
        {
            int orig_h = h - pad[1];
            if (orig_h < 0)
                orig_h = -orig_h;
            if (orig_h >= height)
                orig_h = 2 * height - orig_h - 2;
            for (int w = 0; w < width + pad[0] + pad[2]; w++)
            {
                int orig_w = w - pad[0];
                if (orig_w < 0)
                    orig_w = -orig_w;
                if (orig_w >= width)
                    orig_w = 2 * width - orig_w - 2;
                for (int c = 0; c < 3; c++)
                {
                    out[h*new_width * channel + w*channel + c] = image[orig_h*width*channel + orig_w*channel + c];
                }
            }
        }
    }
}

template<class T>
void createFilter(T* gKernel, int kernel_size = 5)
{
    // set standard deviation to 1.0
    T sigma = 1.0;
    T r, s = 2.0 * sigma * sigma;

    // sum is for normalization
    T sum = 0.0;

    // generate 5x5 kernel
    int start = int(kernel_size / 2);
    for (int y = -start; y <= start; y++)
    {
        for (int x = -start; x <= start; x++)
        {
            r = sqrt(x*x + y*y);
            gKernel[(x + start) + (y + start)*kernel_size] = (exp(-(r*r) / s)) / (M_PI * s);
            sum += gKernel[(x + start) + (y + start)*kernel_size];
        }
    }

    // normalize the Kernel
    for (int i = 0; i < kernel_size; ++i)
    {
        for (int j = 0; j < kernel_size; ++j)
        {
            gKernel[i*kernel_size + j] /= sum;
        }
    }
}

void cv_gaussian_filter(unsigned char* image, unsigned char* out_image, int height, int width, int kernel_size)
{
    int channel = 3;
    float* kernel_ = (float*)malloc(kernel_size*kernel_size*sizeof(float));
    createFilter(kernel_, kernel_size);
    int half_size = int(kernel_size / 2);
    //std::cout << height << "  " << width << std::endl;
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            for (int c = 0; c < channel; c++)
            {
                float tem_data = 0;
                for (int k_h = 0; k_h < kernel_size; k_h++)
                {
                    for (int k_w = 0; k_w < kernel_size; k_w++)
                    {
                        int h_pos = h + k_h - half_size;
                        int w_pos = w + k_w - half_size;
                        if (h_pos < 0)
                            h_pos = -h_pos;
                        if (h_pos >= height)
                            h_pos = 2 * height - h_pos - 1;
                        if (w_pos < 0)
                            w_pos = -w_pos;
                        if (w_pos >= width)
                            w_pos = 2 * width - w_pos - 1;

                        tem_data +=
                            kernel_[k_h*kernel_size + k_w] * image[h_pos*width*channel + w_pos * channel + c];

                    }
                }
                out_image[h*width*channel + w * channel + c] = tem_data;
            }
        }
    }
    free(kernel_);
}


//int main()
//{
//	int kernel_size = 5;
//	double *gKernel = (double*)malloc(kernel_size*kernel_size*sizeof(double));
//	createFilter(gKernel, 5);
//	for (int i = 0; i < 5; ++i)
//	{
//		for (int j = 0; j < 5; ++j)
//			cout << gKernel[i*kernel_size + j] << "\t";
//		cout << endl;
//	}
//}



int transform_bilinear2d(unsigned char* output_data, int output_height, int output_width, int channels,
    unsigned char* input_data, int input_orig_height, int input_orig_width, px_pointf_t* quad, bool align_corners)
{
    px_pointf_t& point_start = quad[0];
    px_pointf_t& point_end = quad[3];
    float distance_orig = (point_end.y - point_start.y)*(point_end.y - point_start.y) +
        (point_end.x - point_start.x)*(point_end.x - point_start.x);
    distance_orig = std::sqrt(distance_orig);
    float theta = std::atan((point_end.y - point_start.y) / (point_end.x - point_start.x));
#ifndef ROTATE_IMAGE
    float costheta = 1;// #np.cos(theta)
    float sintheta = 0;// #np.sin(theta)
#else
    float costheta = std::cos(theta);
    float sintheta = std::sin(theta);
#endif
    float input_roi_orig_point[2] = { point_start.x, point_start.y };

    int input_height = distance_orig;//crop height
    int input_width = distance_orig;// #(y_distance x_distance)

    //align corners option from pytorch
    if (align_corners)
    {
        const float rheight = (output_height > 1) ? (float)(input_height - 1) / (output_height - 1) : 0.f;
        const float rwidth = (output_width > 1) ? (float)(input_width - 1) / (output_width - 1) : 0.f;
        //OMP_PARALLEL_FOR_
        for (int h2 = 0; h2 < output_height; ++h2)
        {
            for (int w2 = 0; w2 < output_width; ++w2)
            {
                float w1r = rwidth * w2;
                float h1r = rheight * h2;
                //roatate
                h1r = w1r*sintheta + h1r*costheta + input_roi_orig_point[1];
                w1r = w1r*costheta - h1r*sintheta + input_roi_orig_point[0];

                int h1 = int(h1r);
                const int h1p = (h1 < input_height - 1) ? 1 : 0;
                const float h1lambda = h1r - h1;
                const float h0lambda = (float)1. - h1lambda;

                const int w1 = static_cast<int>(w1r);
                const int w1p = (w1 < input_width - 1) ? 1 : 0;
                const float w1lambda = w1r - w1;
                const float w0lambda = (float)1. - w1lambda;

                const unsigned char *Xdata = &(input_data[h1*input_orig_width*channels + w1*channels]);
                unsigned char *Ydata = &(output_data[h2*output_width*channels + w2*channels]);
                for (int c = 0; c < channels; ++c)
                {
                    Ydata[c] = \
                        h0lambda * (w0lambda * Xdata[c] + w1lambda * Xdata[w1p*channels + c]) + \
                        h1lambda * (w0lambda * Xdata[h1p*input_orig_width*channels + c] + w1lambda * Xdata[h1p*input_orig_width*channels + w1p*channels + c]);
                }//(h1 + h1p)*input_orig_width*channels + (w1 + w1p)*channels + c
            }
        }
    }
    else
    {
        const float rheight = (output_height > 1) ? (float)(input_height) / (output_height) : 0.f;
        const float rwidth = (output_width > 1) ? (float)(input_width) / (output_width) : 0.f;

        //OMP_PARALLEL_FOR_
        for (int h2 = 0; h2 < output_height; ++h2)
        {
            for (int w2 = 0; w2 < output_width; ++w2)
            {
                float h1r = rheight * (h2 + 0.5) - 0.5;
                float w1r = rwidth * (w2 + 0.5) - 0.5;
                //roatate
                h1r = w1r*sintheta + h1r*costheta + input_roi_orig_point[1];
                w1r = w1r*costheta - h1r*sintheta + input_roi_orig_point[0];
                h1r = h1r >= 0 ? h1r : 0;
                const int h1 = static_cast<int>(h1r);
                const int h1p = (h1 < input_height - 1) ? 1 : 0;
                const float h1lambda = h1r - h1;
                const float h0lambda = (float)1. - h1lambda;


                w1r = w1r >= 0 ? w1r : 0;
                const int w1 = static_cast<int>(w1r);
                const int w1p = (w1 < input_width - 1) ? 1 : 0;
                const float w1lambda = w1r - w1;
                const float w0lambda = (float)1. - w1lambda;

                const unsigned char *Xdata = &(input_data[h1*input_orig_width*channels + w1*channels]);
                unsigned char *Ydata = &(output_data[h2*output_width*channels + w2*channels]);
                for (int c = 0; c < channels; ++c)
                {
                    Ydata[c] = \
                        h0lambda * (w0lambda * Xdata[c] + w1lambda * Xdata[w1p*channels + c]) + \
                        h1lambda * (w0lambda * Xdata[h1p*input_orig_width*channels + c] +
                        w1lambda * Xdata[h1p*input_orig_width*channels + w1p*channels + c]);
                }
            }
        }
    }
    return 0;
}

//#define USE_OPENCV
int  back_to_image_bilinear2d(unsigned char* output_data, int output_height, int output_width, int channels,
    unsigned char* input_data, int input_orig_height, int input_orig_width, px_pointf_t* quad, bool align_corners)
{
    px_pointf_t& point_start = quad[0];
    px_pointf_t& point_end = quad[3];
    float distance_orig = (point_end.y - point_start.y)*(point_end.y - point_start.y) +
        (point_end.x - point_start.x)*(point_end.x - point_start.x);
    distance_orig = std::sqrt(distance_orig);
    float theta = std::atan((point_end.y - point_start.y) / (point_end.x - point_start.x));
#ifndef ROTATE_IMAGE
    float costheta = 1;// #np.cos(theta)
    float sintheta = 0;// #np.sin(theta)
#else
    float costheta = std::cos(theta);
    float sintheta = std::sin(theta);
#endif
    float input_roi_orig_point[2] = { point_start.x, point_start.y };

    int input_height = distance_orig;//crop height
    int input_width = distance_orig;// #(y_distance x_distance)
    
#ifdef ROTATE_IMAGE
    float x_min = quad[0].x;
    float x_max = quad[0].x;
    float y_min = quad[0].y;
    float y_max = quad[0].y;
    for (int ind = 0; ind < 4; ind++)
    {
        x_min = std::min(x_min, quad[ind].x);
        x_max = std::max(x_max, quad[ind].x);
        y_min = std::min(y_min, quad[ind].y);
        y_max = std::max(y_max, quad[ind].y);
    }
#else
    float x_min = quad[0].x;
    float x_max = quad[0].x + distance_orig;
    float y_min = quad[0].y;
    float y_max = quad[0].y + distance_orig;
#endif
    int left_bounding = int(std::floor(x_min));
    int	top_bounding = int(std::floor(y_min));
    int	right_bounding = int(std::ceil(x_max));
    int	bottom_bounding = int(std::ceil(y_max));

    //align corners option from pytorch
    if (align_corners)
    {
        const float rheight = (output_height > 1) ? (float)(input_height - 1) / (output_height - 1) : 0.f;
        const float rwidth = (output_width > 1) ? (float)(input_width - 1) / (output_width - 1) : 0.f;
        //OMP_PARALLEL_FOR_
        for (int h1 = top_bounding; h1 < bottom_bounding + 1; h1++)
        {
            for (int w1 = left_bounding; w1 < right_bounding + 1; w1++)
            {
                float h1_cut = h1 - input_roi_orig_point[1];
                float w1_cut = w1 - input_roi_orig_point[0];
                //rotate
                //h1r = w1r * sintheta + h1r * costheta + input_roi_orig_point[1];
                //w1r = w1r * costheta - h1r * sintheta + input_roi_orig_point[0];

                float h1_ = -w1_cut*sintheta + h1_cut*costheta;
                float w1_ = w1_cut*costheta + h1_cut*sintheta;
                //scale
                float h2r = h1_ / rheight;
                float w2r = w1_ / rwidth;

                if (h2r > 0 && h2r<output_height && w2r>0 && w2r < output_width)
                {
                    int h2 = int(h2r);
                    int h2p = h2 < output_height - 1 ? 1 : 0;
                    float h1lambda = h2r - h2;
                    float h0lambda = 1. - h1lambda;

                    int w2 = int(w2r);
                    int w2p = w2 < output_width - 1 ? 1 : 0;

                    float w1lambda = w2r - w2;
                    float w0lambda = 1. - w1lambda;

                    unsigned char *Xdata = &(input_data[h1*input_orig_width*channels + w1*channels]);
                    unsigned char *Ydata = &(output_data[h2*output_width*channels + w2*channels]);
                    for (int c = 0; c < channels; c++)
                    {
                        Xdata[c] = h0lambda * (w0lambda * Ydata[c] + w1lambda * Ydata[w2p*channels + c]) + \
                            h1lambda * (w0lambda * Ydata[h2p*output_width*channels + c] + \
                                w1lambda * Ydata[h2p*output_width*channels + w2p * channels + c]);
                        //float w = src_mat_mask.at<float>(h1, w1);
                        //= temp;//

                    }
                }
            }
        }
    }
    else
    {
        const float rheight = (output_height > 1) ? (float)(input_height) / (output_height) : 0.f;
        const float rwidth = (output_width > 1) ? (float)(input_width) / (output_width) : 0.f;
    
        //OMP_PARALLEL_FOR_
        for (int h1 = top_bounding; h1 < bottom_bounding + 1; h1++)
        {
            for (int w1 = left_bounding; w1 < right_bounding + 1; w1++)
            {
                float h1_cut = h1 - input_roi_orig_point[1];
                float w1_cut = w1 - input_roi_orig_point[0];
                //rotate
                //float h1_ = -w1_cut * sintheta + h1_cut * costheta;
                //float w1_ = w1_cut * costheta + h1_cut * sintheta;

                float h1_ = -w1_cut*sintheta + h1_cut*costheta;
                float w1_ = w1_cut*costheta + h1_cut*sintheta;
                //scale
                float h2r = (h1_ + 0.5) / rheight - 0.5;
                float w2r = (w1_ + 0.5) / rwidth - 0.5;

                if (h2r > 0 && h2r<output_height && w2r>0 && w2r < output_width)
                {
                    int h2 = int(h2r);
                    int h2p = h2 < output_height - 1 ? 1 : 0;
                    float h1lambda = h2r - h2;
                    float h0lambda = 1. - h1lambda;

                    int w2 = int(w2r);
                    int w2p = w2 < output_width - 1 ? 1 : 0;
                    float w1lambda = w2r - w2;
                    float w0lambda = 1 - w1lambda;

                    unsigned char *Xdata = &(input_data[h1*input_orig_width*channels + w1*channels]);
                    unsigned char *Ydata = &(output_data[h2*output_width*channels + w2*channels]);
                    for (int c = 0; c < channels; c++)
                    {
                        //float w = src_mat_mask.at<float>(h1, w1);
                        Xdata[c] = \
                            h0lambda * (w0lambda * Ydata[c] + w1lambda * Ydata[w2p*channels + c]) + \
                            h1lambda * (w0lambda * Ydata[h2p*output_width*channels + c] + \
                            w1lambda * Ydata[h2p*output_width*channels + w2p*channels + c]);

                    }

                }
            }
        }
    }
    return 0;
}




// nearest interpolate function
int upsample_nearest2d(unsigned char *output_data, const unsigned char *input_data, int input_height, int input_width,
    int output_height, int output_width, int channels) {
    // special case: just copy
    if (input_height == output_height && input_width == output_width) {
        if (output_data != input_data) {
            memcpy(output_data, input_data, channels * input_height * input_width * sizeof(unsigned char));
        }
        return 0;
    }

    const float height_scale = (float)input_height / (float)output_height;
    const float width_scale = (float)input_width / (float)output_width;

    //OMP_PARALLEL_FOR_
    for (int i = 0; i < channels; ++i) {
        int output_index = i * output_height * output_width;
        int input_index_i = i * input_height * input_width;
        for (int j = 0; j < output_height; ++j) {
            int scaled_j = static_cast<int>(j * height_scale);
            int input_index_j = input_index_i + scaled_j * input_width;
            for (int u = 0; u < output_width; ++u) {
                int scaled_u = static_cast<int>(u * width_scale);
                output_data[output_index++] = input_data[input_index_j + scaled_u];
            }
        }
    }

    return 0;
}

int upsample_bilinear2d(float *output_data, const float *input_data, int input_height, int input_width,
    int output_height, int output_width, int channels, bool align_corners) {
    // special case: just copy
    if (input_height == output_height && input_width == output_width) {
        if (output_data != input_data) {
            memcpy(output_data, input_data, channels * input_height * input_width * sizeof(float));
        }
        return 0;
    }

    // align corners option from pytorch
    if (align_corners) {
        const float rheight = (output_height > 1) ? (float)(input_height - 1) / (output_height - 1) : 0.f;
        const float rwidth = (output_width > 1) ? (float)(input_width - 1) / (output_width - 1) : 0.f;
        //OMP_PARALLEL_FOR_
        for (int h2 = 0; h2 < output_height; ++h2) {
            const float h1r = rheight * h2;

            const int h1 = static_cast<int>(h1r);
            const int h1p = (h1 < input_height - 1) ? 1 : 0;
            const float h1lambda = h1r - h1;
            const float h0lambda = (float)1. - h1lambda;
            for (int w2 = 0; w2 < output_width; ++w2) {
                const float w1r = rwidth * w2;
                const int w1 = static_cast<int>(w1r);
                const int w1p = (w1 < input_width - 1) ? 1 : 0;
                const float w1lambda = w1r - w1;
                const float w0lambda = (float)1. - w1lambda;
                const float *Xdata = &(input_data[h1 * input_width + w1]);
                float *Ydata = &(output_data[h2 * output_width + w2]);
                for (int c = 0; c < channels; ++c) {
                    Ydata[0] =
                        h0lambda * (w0lambda * Xdata[0] + w1lambda * Xdata[w1p]) +
                        h1lambda * (w0lambda * Xdata[h1p * input_width] + w1lambda * Xdata[h1p * input_width + w1p]);
                    Xdata += input_width * input_height;
                    Ydata += output_width * output_height;
                }
            }
        }
    }
    else {
        const float rheight = (output_height > 1) ? (float)(input_height) / (output_height) : 0.f;
        const float rwidth = (output_width > 1) ? (float)(input_width) / (output_width) : 0.f;

        //OMP_PARALLEL_FOR_
        for (int h2 = 0; h2 < output_height; ++h2) {
            float h1r = static_cast<float>(rheight * (h2 + 0.5) - 0.5);
            h1r = h1r >= 0 ? h1r : 0;
            const int h1 = static_cast<int>(h1r);
            const int h1p = (h1 < input_height - 1) ? 1 : 0;

            const float h1lambda = h1r - h1;
            const float h0lambda = (float)1. - h1lambda;

            for (int w2 = 0; w2 < output_width; ++w2) {
                float w1r = static_cast<float>(rwidth * (w2 + 0.5) - 0.5);
                w1r = w1r >= 0 ? w1r : 0;

                const int w1 = static_cast<int>(w1r);
                const int w1p = (w1 < input_width - 1) ? 1 : 0;
                const float w1lambda = w1r - w1;
                const float w0lambda = (float)1. - w1lambda;
                const float *x_data_ptr = &(input_data[h1 * input_width + w1]);
                float *y_data_ptr = &(output_data[h2 * output_width + w2]);
                for (int c = 0; c < channels; ++c) {
                    y_data_ptr[0] = h0lambda * (w0lambda * x_data_ptr[0] + w1lambda * x_data_ptr[w1p]) +
                        h1lambda * (w0lambda * x_data_ptr[h1p * input_width] +
                            w1lambda * x_data_ptr[h1p * input_width + w1p]);
                    x_data_ptr += input_width * input_height;
                    y_data_ptr += output_width * output_height;
                }
            }
        }
    }

    return 0;
}