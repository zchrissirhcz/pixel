#include "naive_cnn.h"
#include <stdio.h>

// nhwc
//#define LOCAL_DEBUG
void nc_pooling_forward_nhwc(NcPoolingParam* param, NcBlob* bottom, NcBlob* top) {
#ifdef LOCAL_DEBUG
    char fout_pth[NC_MAX_PATH];
    sprintf(fout_pth, "%s/debug/pool_out_nhwc.txt", project_dir);
    FILE* fout = fopen(fout_pth, "w");
#endif

    px_stride_t stride = param->stride;

    int kernel_h = param->map_size;
    int kernel_w = param->map_size;

    int out_h = 0;
    for (int h = 0; h < bottom->height-kernel_h+1; h += stride.height, out_h++)
    {
        int out_w = 0;
        for (int w = 0; w < bottom->width-kernel_w+1; w += stride.width, out_w++)
        {
            for (int c = 0; c < bottom->channel; c++)
            {
                float sum = 0.f;
                for (int kh = 0; kh < kernel_h; kh++)
                {
                    for (int kw = 0; kw < kernel_w; kw++)
                    {
                        //sum += bottom[n, h+kh, w+kw, c]
                        int bottom_idx = (h + kh)*bottom->width*top->channel + (w + kw)*bottom->channel + c;
                        sum += bottom->data[bottom_idx];
                    }
                }
                //top->data[out_h, out_w, c] = sum/(kernel_h*kernel_w)
                int top_idx = out_h * top->width*top->channel + out_w * top->channel + c;
                top->data[top_idx] = sum / (kernel_h*kernel_w);
            }
        }
    }

    //ignore n dimension since the output is 3-dimension
#ifdef LOCAL_DEBUG
    for (int h = 0; h < top->h; h++) {
        for (int w = 0; w < top->w; w++) {
            for (int c = 0; c < top->c; c++) {
                int top_idx = h * top->w*top->c + w * top->c + c;
                fprintf(fout, "top[%d,%d,%d]=%6f, ", h, w, c, top->data[top_idx]);
                //fprintf(fout, "top[%d]=%6f, ", top_idx, top->data[top_idx]);
            }
            fprintf(fout, "\n");
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
#undef LOCAL_DEBUG
#endif
}

//#define LOCAL_DEBUG
void nc_pooling_forward_nchw(NcPoolingParam* param, NcBlob* bottom, NcBlob* top) {
#ifdef LOCAL_DEBUG
    char fout_pth[NC_MAX_PATH];
    sprintf(fout_pth, "%s/debug/pool_out_nchw.txt", project_dir);
    FILE* fout = fopen(fout_pth, "w");
#endif

    px_stride_t stride = param->stride;
    int kernel_h = param->map_size;
    int kernel_w = param->map_size;

    for (int c = 0; c < bottom->channel; c++)
    {
        int out_h = 0;
        for (int h = 0; h < bottom->height-kernel_h+1; h += stride.height, out_h++)
        {
            int out_w = 0;
            for (int w = 0; w < bottom->width-kernel_w+1; w += stride.width, out_w++)
            {
                int top_idx = c * top->height*top->width + h * top->width + w;

                //sum += bottom[n,c,h+kh, w+kw]
                //fprintf(fout, "top[%d,%d,%d]=sigma(", c, out_h, out_w);
                //fprintf(fout, "top[%d,%d,%d]=top[%d]=sigma(", c, h, w, top_idx);
                float sum = 0.f;
                for (int kh = 0; kh < kernel_h; kh++)
                {
                    for (int kw = 0; kw < kernel_w; kw++)
                    {
                        int bottom_idx = c * bottom->height*bottom->width + (h + kh)*bottom->width + (w + kw);
                        sum += bottom->data[bottom_idx];
                        //fprintf(fout, "I[%d,%d,%d]+", c, h + kh, w + kw);
                        //fprintf(fout, "%f+", bottom->data[bottom_idx]);
                    }
                }
                //top->data[n, c, out_h, out_w] = sum
                top->data[top_idx] = sum / (kernel_h*kernel_w);
                //fprintf(fout, "\n");
            }
        }
    }

#ifdef LOCAL_DEBUG
    //ignore n dimension since the output is 3-dimension
    for (int c = 0; c < top->c; c++) {
        for (int h = 0; h < top->h; h++) {
            for (int w = 0; w < top->w; w++) {
                int top_idx = c * top->h*top->w + h * top->w + w;
                //fprintf(fout, "top[c=%d,h=%d,w=%d]=%6f, ", c, h, w, top->data[top_idx]);
                fprintf(fout, "top[%d]=%6f, ", top_idx, top->data[top_idx]);
            }
            fprintf(fout, "\n");
        }
        fprintf(fout, "\n");
    }
    fclose(fout);
#undef LOCAL_DEBUG
#endif
}


void nc_pooling_test_nchw() {
    NcBlob* input = nc_create_empty_blob(1, 5, 5, 2);
    input->order = NCHW;
    input->data = (float[]) {
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

    NcBlob* kernel = nc_create_empty_blob(1, 3, 3, 2);

    int map_size = kernel->height;
    int in_channels = input->channel;
    NcPoolingType pool_type = NC_POOLING_AVERAGE;
    NcPaddingType pad_type = NC_PADDING_POOL_CAFFE;
    NcPoolingParam* param = nc_infer_create_pooling_param(map_size, in_channels, pool_type, pad_type);

    param->out_height = (input->height-kernel->height)/param->stride.height+1;
    param->out_width = (input->width-kernel->width)/param->stride.width+1;
    
    NcBlob* top = nc_create_blob(1, param->out_height, param->out_width, input->channel);

    nc_pooling_forward_nchw(param, input, top);
}


void nc_pooling_test_nhwc() {
    NcBlob* input = nc_create_empty_blob(1, 5, 5, 2);
    input->order = NHWC;
    input->data = (float[]) {
        //C1
        1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 2,
    };

    NcBlob* kernel = nc_create_empty_blob(1, 3, 3, input->channel);

    int map_size = kernel->height;
    int in_channels = input->channel;
    NcPoolingType pool_type = NC_POOLING_AVERAGE;
    NcPaddingType pad_type = NC_PADDING_POOL_CAFFE;
    NcPoolingParam* param = nc_infer_create_pooling_param(map_size, in_channels, pool_type, pad_type);

    param->out_height = (input->height-kernel->height)/param->stride.height+1;
    param->out_width = (input->width-kernel->width)/param->stride.width+1;
    
    NcBlob* top = nc_create_blob(1, param->out_height, param->out_width, input->channel);

    nc_pooling_forward_nhwc(param, input, top);


}
