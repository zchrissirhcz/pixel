#include <stdio.h>
#include "naive_cnn.h"
#include "px_cnn.h"
#include <stdlib.h>

px_size_t nc_get_conv_output_size(px_size_t input_size, px_size_t kernel_size, px_size_t stride)
{
    px_size_t output_size;
    output_size.height = (input_size.height - kernel_size.height) / stride.height + 1;
    output_size.width = (input_size.width - kernel_size.width) / stride.width + 1;
    return output_size;
}


void nc_convolution_test_nchw()
{
    NcBlob* input = nc_create_empty_blob(1, 5, 5, 2);
    input->order = NCHW;
    input->data = (float[]){
        //C1
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,
        1, 1, 1, 1, 1,

        //C2
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
        2, 2, 2, 2, 2,
    };

    NcBlob* kernel = nc_create_empty_blob(2, 2, 2, 2);
    kernel->data = (float[]){
        //C1
        1, 1,
        1, 1,

        //C2
        0.5, 0.5,
        0.5, 0.5,

        //C1
        1, 1,
        1, 1,

        //C2
        0.5, 0.5,
        0.5, 0.5,
    };

    px_stride_t stride = px_create_stride(1, 1);

    int output_h = (input->height - kernel->height) / stride.height + 1;
    int output_w = (input->width - kernel->width) / stride.width + 1;
    NcBlob* output = nc_create_blob(1, output_h, output_w, kernel->batch);

    int map_size = kernel->height;
    int in_channels = input->channel;
    int out_channels = kernel->batch;
    NcPaddingType pad_type = NC_PADDING_CONV_CAFFE;
    NcConvolutionParam* param = nc_infer_create_convolution_param(map_size, in_channels, out_channels, pad_type);
    free(param->weight);
    param->weight = kernel;

    nc_convolution_forward_nchw(param, input, output);
}

void nc_convolution_test_nhwc()
{
    NcBlob* input = nc_create_empty_blob(1, 5, 5, 2);
    input->order = NHWC;
    input->data = (float[]){
        ////C1
        //1, 1, 1, 1, 1,
        //1, 1, 1, 1, 1,
        //1, 1, 1, 1, 1,
        //1, 1, 1, 1, 1,
        //1, 1, 1, 1, 1,

        ////C2
        //2, 2, 2, 2, 2,
        //2, 2, 2, 2, 2,
        //2, 2, 2, 2, 2,
        //2, 2, 2, 2, 2,
        //2, 2, 2, 2, 2,

        1, 2, 1, 2, 1,
        2, 1, 2, 1, 2,
        1, 2, 1, 2, 1,
        2, 1, 2, 1, 2,
        1, 2, 1, 2, 1,

        2, 1, 2, 1, 2,
        1, 2, 1, 2, 1,
        2, 1, 2, 1, 2,
        1, 2, 1, 2, 1,
        2, 1, 2, 1, 2,
    };

    NcBlob* kernel = nc_create_empty_blob(2, 2, 2, 2);
    kernel->data = (float[]){
        ////C1
        //1, 1,
        //1, 1,

        ////C2
        //0.5, 0.5,
        //0.5, 0.5,

        1, 0.5,
        1, 0.5,

        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,

        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
        
        1, 0.5,
        1, 0.5,
    };

    px_stride_t stride = px_create_stride(1, 1);

    int map_size = kernel->height;
    int in_channels = input->channel;
    int out_channels = kernel->batch;
    NcPaddingType pad_type = NC_PADDING_CONV_CAFFE;
    NcConvolutionParam* param = nc_infer_create_convolution_param(map_size, in_channels, out_channels, pad_type);
    free(param->weight);
    param->weight = kernel;

    int output_h = (input->height - kernel->height) / stride.height + 1;
    int output_w = (input->width - kernel->width) / stride.width + 1;
    NcBlob* output = nc_create_blob(1, output_h, output_w, kernel->batch);

    nc_convolution_forward_nhwc(param, input, output);
}

//#define LOCAL_DEBUG
//NCHW convolution
void nc_convolution_forward_nchw(NcConvolutionParam* param, NcBlob* bottom, NcBlob* top)
{
#ifdef LOCAL_DEBUG
    char fout_pth[NC_MAX_PATH];
    sprintf(fout_pth, "%s/debug/conv_out_nchw.txt", project_dir);
    FILE* fout = fopen(fout_pth, "w");
    fprintf(fout, "\n--- nc_convolution_forward_nhwc\n");
#endif

    const px_stride_t stride = param->stride;
    NcBlob* kernel = param->weight;

    //per kernel
    for (int n = 0; n < kernel->batch; n++)
    {
        for (int c = 0; c < bottom->channel; c++)
        {
            int out_h = 0;
            for (int h = 0; h < bottom->height - kernel->height + 1; h += stride.height, out_h++)
            {
                int out_w = 0;
                for (int w = 0; w < bottom->width - kernel->width + 1; w += stride.width, out_w++)
                {
                    float sum = 0.f;
                    int top_idx = n * top->height * top->width + out_h * top->width + out_w;
                    //top->data[0, n, out_h, out_w] += sum;
#ifdef LOCAL_DEBUG
                    fprintf(fout, "top(%d,%d,%d,%d)=top[%d]=sigma(", n, c, out_h, out_w, top_idx);
#endif
                    for (int kh = 0; kh < kernel->height; kh++)
                    {
                        for (int kw = 0; kw < kernel->width; kw++)
                        {
                            //sum+=bottom->data[c, h+kh, w+kw] * kernel->data[n, c, kh, kw];
                            int bottom_idx = c * bottom->height * bottom->width + (h + kh) * bottom->width + w + kw;
                            int kernel_idx = n * kernel->nstep + c * kernel->height * kernel->width + kh * kernel->width + kw;
                            sum += bottom->data[bottom_idx] * kernel->data[kernel_idx];
#ifdef LOCAL_DEBUG
                            fprintf(fout, "I(%d,%d,%d,%d)*K(%d,%d,%d,%d)+",
                                    n, c, h + kh, w + kw,
                                    n, c, kh, kw);
#endif
                        }
                    }
                    top->data[top_idx] += sum;
#ifdef LOCAL_DEBUG
                    fprintf(fout, "\n");
#endif
                }
            }
        }
    }

#ifdef LOCAL_DEBUG
    for (int c = 0; c < top->c; c++)
    {
        for (int h = 0; h < top->h; h++)
        {
            for (int w = 0; w < top->w; w++)
            {
                //int top_idx = h * top->w*top->c + w * top->c + c;
                int top_idx = c * top->h * top->w + h * top->w + w;
                fprintf(fout, "%6f, ", top->data[top_idx]);
            }
            fprintf(fout, "\n");
        }
        fprintf(fout, "\n");
    }
#endif

#ifdef LOCAL_DEBUG
    fclose(fout);
#undef LOCAL_DEBUG
#endif
}

//#define LOCAL_DEBUG
//NHWC convolution forward
//@param bottom: n=1,3d blob
//@param top: n=1,3d blob
void nc_convolution_forward_nhwc(NcConvolutionParam* param, NcBlob* bottom, NcBlob* top)
{
    const px_stride_t stride = param->stride;

    //kernel: n=1, 3d blob
    //NcBlob* kernel = nc_create_empty_blob(1, param->weight->h, param->weight->w, param->weight->c);
    NcBlob* kernel = param->weight;

#ifdef LOCAL_DEBUG
    char fout_pth[NC_MAX_PATH];
    sprintf(fout_pth, "%s/debug/conv_out_nhwc.txt", project_dir);
    FILE* fout = fopen(fout_pth, "w");
    fprintf(fout, "\n--- nc_convolution_forward_nhwc\n");
#endif

    //per kernel
    //PX_LOGE("top->n=%d\n", top->n);
    for (int n = 0; n < kernel->batch; n++)
    {
        //PX_LOGE("\t\t%d-th kernel\n", n);
        //fprintf(fout, "NHWC mode\n");

        int out_h = 0;
        int out_w = 0;

        int bottom_idx, kernel_idx, top_idx;
        for (int h = 0; h < bottom->height - kernel->height + 1; h += stride.height, out_h += 1)
        {
            out_w = 0;
            for (int w = 0; w < bottom->width - kernel->width + 1; w += stride.width, out_w += 1)
            {
                float sum = 0.f;
#ifdef LOCAL_DEBUG
                fprintf(fout, "output[%d,%d]=sigma(", out_h, out_w);
#endif
                for (int kh = 0; kh < kernel->height; kh++)
                {
                    for (int kw = 0; kw < kernel->width; kw++)
                    {
                        for (int c = 0; c < bottom->channel; c++)
                        {
                            //sum += bottom[h + kh, w + kw, c] * kernel[kh, kw, c];
                            bottom_idx = (h + kh) * bottom->width * bottom->channel + (w + kw) * bottom->channel + c;
                            kernel_idx = kh * kernel->width * kernel->channel + kw * kernel->channel + c;
                            sum += bottom->data[bottom_idx] * kernel->data[kernel_idx];

#ifdef LOCAL_DEBUG
                            fprintf(fout, "I[%d,%d,%d]*K[%d,%d,%d]+",
                                    h + kh, w + kw, c,
                                    kh, kw, c);
#endif
                        }
                    }
                }

#ifdef LOCAL_DEBUG
                fprintf(fout, "\n");
#endif

                //output[out_h, out_w, n] = sum;
                top_idx = out_h * top->width * top->channel + out_w * top->channel + n;
                top->data[top_idx] = sum;
            }
        }
    }

#ifdef LOCAL_DEBUG
    PX_LOGE("top's dimension: h=%d, w=%d, c=%d\n", top->h, top->w, top->c);
    for (int h = 0; h < top->h; h++)
    {
        for (int w = 0; w < top->w; w++)
        {
            for (int c = 0; c < top->c; c++)
            {
                int top_idx = h * top->w * top->c + w * top->c + c;
                fprintf(fout, "%6f, ", top->data[top_idx]);
            }
        }
        fprintf(fout, "\n");
    }
#endif

#ifdef LOCAL_DEBUG
    fclose(fout);
#undef LOCAL_DEBUG
#endif
}
