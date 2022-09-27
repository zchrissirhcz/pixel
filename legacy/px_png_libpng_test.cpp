#include <stdio.h>
#include <stdlib.h>

#if __ARM_NEON
#include <arm_neon.h>
#include "arm_neon_helper.hpp"
#else
#include "arm_neon_sim.hpp"
#endif

#include <png.h>
#include "image_util.hpp"

//8-bit 灰度PNG图片读取与写入
int main()
{
#if __ANDROID__
    std::string load_prefix = "/data/local/tmp";
#elif __linux__
    std::string load_prefix = "/home/zz/data";
#elif _MSC_VER
    std::string load_prefix = "d:/data";
#else
#pragma error
#endif


    size_t gx, gy;		//图像尺寸  size_t = unsigned char
    png_bytep gbild_;	//图像数据首地址 unsigned char *
    std::string image_path = load_prefix + "/ggray.png";
    px_read_png(image_path.c_str(), &gbild_,&gx,&gy);	//Bild lesen 读图

    unsigned char **gbild = (unsigned char**)malloc(sizeof(char *) * gy);	//定义二维数据
    for(int i = 0 ; i < gy ; i++)
    {
        gbild[i] = gbild_ + i * gx; 
    }

    //二维数组的使用
    unsigned char *gbild_out_ = (unsigned char *)malloc(sizeof(char) * (gx * gy + 16)); //a[5 + 列数 * 6]
    unsigned char **gbild_out = (unsigned char **)malloc(sizeof(char * ) * gy);
    for(int i = 0 ; i < gy ; i++)
    {
        gbild_out[i] = gbild_out_ + i * gx;		//gbild_out[1][3]
    }


    uint8x16_t regin, mitt;	//定义NEON向量
    for(int i = 0 ; i < gy ; i++)
    {
        for(int ii = 0 ; ii < gx ; ii += 16)
        {
            regin = vld1q_u8(&gbild[i][ii]);	//数据读取

            mitt = vdupq_n_u8(50);
            regin = vqsubq_u8(regin, mitt);
            
            vst1q_u8(&gbild_out[i][ii], regin);	//数据回写
        }
    }
    px_write_png("out.png", gbild_out_, gx, gy, 8);	//制作PNG图
}
