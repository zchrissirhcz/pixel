# ncnn::Mat 类

## 0x2 Mat类的 from_pixels 函数
`from_pixels()`的实现，是根据FLAG，调用不同的函数，其中最常用的是:
- `from_rgb()`
- `from_rgb2bgr()`

### 0x201 from_rgb 函数
```c++
//mat_pixel.cpp

/**
 * 从raw buffer创建Mat对象
 * 维度顺序：HWC => CHW
 * 数据类型：uchar => float
 * ARMv8平台使用了neon intrinsics实现加速
 * @param rgb 图像raw buffer，HWC顺序
 * @param w 图像宽度
 * @param h 图像高度
 * @return Mat对象，CHW顺序
 */
static Mat from_rgb(const unsigned char* rgb, int w, int h);
```

ncnn的实现，是把pure C和arm intrinsics以及arm assembly放在同一个函数中，用宏控制不同的代码分支。为了便于理解，按pure C, arm intrinsics分别写为函数，并增加注释。其中“C版本实现”指的是naive C/C++实现。

```c++
// C版本的from_rgb实现
ncnn::Mat from_rgb_c(const unsigned char* rgb, int w, int h) {
    ncnn::Mat m(w, h, 3); // 创建3通道图像
    if (m.empty()) {
        return m;
    }

    // 创建3个指针，分别指向刚创建图像的3个通道
    // 注意：源buffer的每个bgr是连续的，而目标buffer的bgr是分开的
    // 也就是：ncnn::Mat是CHW顺序，而cv::Mat是HWC顺序
    // 因此，from_rgb函数，不能简单的用memcpy实现
    // 但如果用NHWC顺序做CNN推理，则from_rgb可用memcpy甚至浅拷贝实现
    float* ptr0 = m.channel(0);
    float* ptr1 = m.channel(1);
    float* ptr2 = m.channel(2);

    int size = w * h;
    int remain = size;
    for (int i=0; i<remain; i++) {
        *ptr0 = rgb[0];
        *ptr1 = rgb[1];
        *ptr2 = rgb[2];

        rgb += 3;
        ptr0++;
        ptr1++;
        ptr2++;
    }
    return m;
}
```

```c++
// neon intrinsics版本的from_rgb
ncnn::Mat from_rgb_neon_intrinsics(const unsigned char* rgb, int w, int h)
{
    ncnn::Mat m(w, h, 3);
    if (m.empty()) {
        return m;
    }

    float* ptr0 = m.channel(0);
    float* ptr1 = m.channel(1);
    float* ptr2 = m.channel(2);

    int size = w * h;

    int nn = size >> 3; // size / 8
    int remain = size - (nn<<3); // size - ((size/8)*8)

    for (; nn>0; nn--) {
        // vld3_u8: 按交叉顺序将内存的数据装入3个neon寄存器(d寄存器）
        // 其中每个d寄存器的每个通道(lane)是8个bit，每个d寄存器是64位，也就是8个lane
        // 通道(lane)的意思，是把寄存器看做是基本长度(size)的向量，每个向量元素是一个lane
        // 内存第1个数据放入第1个d寄存器的第1个通道，
        // 内存第2个数据放入第2个d寄存器的第1个通道,
        // 内存第3个数据放入第3个d寄存器的第1个通道,
        // 内存第4个数据放入第1个d寄存器的第2个通道,
        // 内存第5个数据放入第2个d寄存器的第2个通道,
        // 内存第6个数据放入第3个d寄存器的第2个通道,
        // ...
        // 内存第22个数据放入第1个d寄存器的第8个通道,
        // 内存第23个数据放入第2个d寄存器的第8个通道,
        // 内存第24个数据放入第3个d寄存器的第8个通道,
        uint8x8x3_t _rgb = vld3_u8(rgb);

        // 将vector的元素bit位扩大到原来的两倍，元素值不变。
        // 目的是什么？防止溢出吗？其实是为了u8=>float32的类型转换
        // 通常，cnn的输入是float类型，手动u8=>float效率并不高
        // 这里是把u8=>float用neon intrinsic实现了
        uint16x8_t _r16 = vmovl_u8(_rgb.val[0]);
        uint16x8_t _g16 = vmovl_u8(_rgb.val[1]);
        uint16x8_t _b16 = vmovl_u8(_rgb.val[2]);

        // vget_low_u16(_r16)是取_r16的低一半（64位)
        float32x4_t _rlow = vcvtq_f32_u32(vmovl_u16(vget_low_u16(_r16)));
        // vget_high_u16(_r16)是取_r16的高一半（64位）
        float32x4_t _rhigh = vcvtq_f32_u32(vmovl_u16(vget_high_u16(_r16)));

        float32x4_t _glow = vcvtq_f32_u32(vmovl_u16(vget_low_u16(_g16)));
        float32x4_t _ghigh = vcvtq_f32_u32(vmovl_u16(vget_high_u16(_g16)));

        float32x4_t _blow = vcvtq_f32_u32(vmovl_u16(vget_low_u16(_b16)));
        float32x4_t _bhigh = vcvtq_f32_u32(vmovl_u16(vget_high_u16(_b16)));

        vst1q_f32(ptr0, _rlow);
        vst1q_f32(ptr0+4, _rhigh);

        vst1q_f32(ptr1, _glow);
        vst1q_f32(ptr1+4, _ghigh);

        vst1q_f32(ptr2, _blow);
        vst1q_f32(ptr2+4, _bhigh);

        rgb += 3*8;
        ptr0 += 8;
        ptr1 += 8;
        ptr2 += 8;
    }

    for (; remain>0; remain--) {
        *ptr0 = rgb[0];
        *ptr1 = rgb[1];
        *ptr2 = rgb[2];

        rgb += 3;
        ptr0++;
        ptr1++;
        ptr2++;
    }

    return m;
}
```

实测性能：
```
// 输入图像尺寸 width=3880, size=5184
// 重复10次
// from_rgb_c耗时 1320 ms
// from_rgb_neon_intrinsics耗时 1230 ms
```

顺带，其他几个函数，我感觉cnn里几乎用不到，就不分析了：
```c++
        if (type == PIXEL_RGB || type == PIXEL_BGR)
            return from_rgb(pixels, w, h); //分析了

        if (type == PIXEL_GRAY)
            return from_gray(pixels, w, h); //感觉用不到

        if (type == PIXEL_RGBA)
            return from_rgba(pixels, w, h); //感觉用不到
```

### 0x202 from_rgb2bgr 函数

和`from_rgb`函数的功能与作用几乎一样（读取uchar类型数据，逐像素转为float并赋值给Mat的data，并且维度顺序从HWC转为CHW），唯一的区别是b和r通道的指针交换一下，也就是：
```c++
        vst1q_f32(ptr0, _rlow);
        vst1q_f32(ptr0+4, _rhigh);

        vst1q_f32(ptr1, _glow);
        vst1q_f32(ptr1+4, _ghigh);

        vst1q_f32(ptr2, _blow);
        vst1q_f32(ptr2+4, _bhigh);
```
改成：
```c++
        vst1q_f32(ptr2, _rlow);
        vst1q_f32(ptr2+4, _rhigh);

        vst1q_f32(ptr1, _glow);
        vst1q_f32(ptr1+4, _ghigh);

        vst1q_f32(ptr0, _blow);
        vst1q_f32(ptr0+4, _bhigh);
```

以及把：
```c++
        *ptr0 = rgb[0];
        *ptr1 = rgb[1];
        *ptr2 = rgb[2];
```
改成：
```c++
        *ptr0 = rgb[2];
        *ptr1 = rgb[1];
        *ptr2 = rgb[0];
```

## 0x21 Mat类的 to_pixels 函数
`to_pixels()`的实现，是根据FLAG，调用不同的函数，其中最常用的是:
- `to_rgb()`
- `to_bgr2rgb()`

### 0x211 to_rgb 函数
纯C naive实现；暂时没考虑SATURATE问题（为什么以及什么时候会导致溢出？）
```c++
/**
 * Mat转buffer
 * 维度：HWC => CHW
 * 类型：float => uchar
 * @param m
 * @param rgb
 */
static void to_rgb_c(const ncnn::Mat& m, unsigned char* rgb)
{
    const float* ptr0 = m.channel(0);
    const float* ptr1 = m.channel(1);
    const float* ptr2 = m.channel(2);

    int size = m.w * m.h;

//#define SATURATE_CAST_UCHAR(X) (unsigned char)std::min(std::max((int)(X), 0), 255);
#define SATURATE_CAST_UCHAR(X) (unsigned char)(X)
    int remain = size;

    for (; remain>0; remain--)
    {
        rgb[0] = SATURATE_CAST_UCHAR(*ptr0);
        rgb[1] = SATURATE_CAST_UCHAR(*ptr1);
        rgb[2] = SATURATE_CAST_UCHAR(*ptr2);

        rgb += 3;
        ptr0++;
        ptr1++;
        ptr2++;
    }

#undef SATURATE_CAST_UCHAR
}
```

然后，不看ncnn源码的情况下，只参照 `from_rgb()` 和neon intrinsics的函数说明资料，写了如下实现：
```c++
void to_rgb_neon_intrinsics(const ncnn::Mat& m, unsigned char* rgb)
{
    const float* ptr0 = m.channel(0);
    const float* ptr1 = m.channel(1);
    const float* ptr2 = m.channel(2);

    int size = m.w * m.h;
    int nn = size >> 3;
    int remain = size - (nn<<3);
    for (; nn>0; nn--) {
        // float => u8

        // r
        float32x4_t r1_f32 = vld1q_dup_f32(ptr0);  // 应改为vld1q_f32
        uint32x4_t r1_u32 = vcvtq_u32_f32(r1_f32);
        uint16x4_t r1_u16 = vmovn_u32(r1_u32);

        float32x4_t r2_f32 = vld1q_dup_f32(ptr0+4);// 应改为vld1q_f32
        uint32x4_t r2_u32 = vcvtq_u32_f32(r2_f32);
        uint16x4_t r2_u16 = vmovn_u32(r2_u32);

        uint16x8_t r_16 = vcombine_u16(r1_u16, r2_u16);
        uint8x8_t r_8 = vmovn_u16(r_16);

        // g
        float32x4_t g1_f32 = vld1q_dup_f32(ptr1);// 应改为vld1q_f32
        uint32x4_t g1_u32 = vcvtq_u32_f32(g1_f32);
        uint16x4_t g1_u16 = vmovn_u32(g1_u32);

        float32x4_t g2_f32 = vld1q_dup_f32(ptr1+4);// 应改为vld1q_f32
        uint32x4_t g2_u32 = vcvtq_u32_f32(g2_f32);
        uint16x4_t g2_u16 = vmovn_u32(g2_u32);

        uint16x8_t g_16 = vcombine_u16(g1_u16, g2_u16);
        uint8x8_t g_8 = vmovn_u16(g_16);

        // b
        float32x4_t b1_f32 = vld1q_dup_f32(ptr2);// 应改为vld1q_f32
        uint32x4_t b1_u32 = vcvtq_u32_f32(b1_f32);
        uint16x4_t b1_u16 = vmovn_u32(b1_u32);

        float32x4_t b2_f32 = vld1q_dup_f32(ptr2+4);// 应改为vld1q_f32
        uint32x4_t b2_u32 = vcvtq_u32_f32(b2_f32);
        uint16x4_t b2_u16 = vmovn_u32(b2_u32);

        uint16x8_t b_16 = vcombine_u16(b1_u16, b2_u16);
        uint8x8_t b_8 = vmovn_u16(b_16);

        //
        uint8x8x3_t _rgb;
        _rgb.val[0] = r_8;
        _rgb.val[1] = g_8;
        _rgb.val[2] = b_8;
        vst3_u8(rgb, _rgb);

        rgb += 3*8;
        ptr0 += 8;
        ptr1 += 8;
        ptr2 += 8;
    }
    for (; remain>0; remain--) {
        rgb[0] = (unsigned char)(*ptr0);
        rgb[1] = (unsigned char)(*ptr1);
        rgb[2] = (unsigned char)(*ptr2);
        rgb += 3;
        ptr0++;
        ptr1++;
        ptr2++;
    }
}
```
测试图不变的情况下，结果有差异，但整图看，肉眼看不出区别。（为什么有差异？哪句导致的？）

参考ncnn最新源码后发现了bug所在，把 vld1q_dup_f32 改为 vld1q_f32 后即可。前者是无脑复制单个元素，后者是加载内存数据到寄存器。

修改后的实现：
```c++
void to_rgb_neon_intrinsics(const ncnn::Mat& m, unsigned char* rgb)
{
    const float* ptr0 = m.channel(0);
    const float* ptr1 = m.channel(1);
    const float* ptr2 = m.channel(2);

    int size = m.w * m.h;
    int nn = size >> 3;
    int remain = size - (nn<<3);
    for (; nn>0; nn--) {
        // r
        float32x4_t _rlow = vld1q_f32(ptr0);
        float32x4_t _rhigh = vld1q_f32(ptr0+4);

        float32x4_t _glow = vld1q_f32(ptr1);
        float32x4_t _ghigh = vld1q_f32(ptr1+4);

        float32x4_t _blow = vld1q_f32(ptr2);
        float32x4_t _bhigh = vld1q_f32(ptr2+4);

        uint16x8_t _r16 = vcombine_u16(vmovn_u32(vcvtq_u32_f32(_rlow)), vmovn_u32(vcvtq_u32_f32(_rhigh)));
        uint16x8_t _g16 = vcombine_u16(vmovn_u32(vcvtq_u32_f32(_glow)), vmovn_u32(vcvtq_u32_f32(_ghigh)));
        uint16x8_t _b16 = vcombine_u16(vmovn_u32(vcvtq_u32_f32(_blow)), vmovn_u32(vcvtq_u32_f32(_bhigh)));

        uint8x8x3_t _rgb;
        _rgb.val[0] = vmovn_u16(_r16);
        _rgb.val[1] = vmovn_u16(_g16);
        _rgb.val[2] = vmovn_u16(_b16);
        vst3_u8(rgb, _rgb);

        rgb += 3*8;
        ptr0 += 8;
        ptr1 += 8;
        ptr2 += 8;
    }
    for (; remain>0; remain--) {
#define SATURATE_CAST_UCHAR(X) (unsigned char)::std::min(::std::max((int)(X), 0), 255);
        rgb[0] = SATURATE_CAST_UCHAR(*ptr0);
        rgb[1] = SATURATE_CAST_UCHAR(*ptr1);
        rgb[2] = SATURATE_CAST_UCHAR(*ptr2);
        rgb += 3;
        ptr0++;
        ptr1++;
        ptr2++;
#undef SATURATE_CAST_UCHAR
    }
}
```
我这里用uchar而没有用schar，暂时没细究区别。anyway，from_rgb再to_rgb，结果一致了！


### 0x212 to_bgr2rgb 函数