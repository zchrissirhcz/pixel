#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    ~Array();

public:
    float* data;
    int len;
};

Array::Array():
    len(0), data(NULL)
{

}

Array::Array(uint32_t _len):
    len(_len)
{
    data = (float*)malloc(sizeof(float) * len);
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data)
{

}

Array::~Array()
{
    //free(data);
}

int main()
{
    {
        int len = 10;
        Array arr1(len);
        for (int i = 0; i < len; i++) {
            arr1.data[i] = i;
        }

        Array arr2 = arr1;

        // 当程序运行到大括号外，arr1和arr2的生命周期结束，会分别调用析构函数
        // 由于Array类没有实现拷贝构造函数，且成员都是平凡类型，则共享data和len成员
        // 如果Array的析构函数里释放data字段，则造成二次释放触发crash
        // 如果Array的析构函数里不释放data字段，则造成内存泄露
    }
    

    return 0;
}