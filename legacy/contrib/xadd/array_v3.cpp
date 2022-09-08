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
    Array(const Array& arr); // 拷贝构造函数
    ~Array();

public:
    float* data;
    int len;

private:
    int* refcount; // 增加了 int 类型的引用计数器
};

Array::Array():
    len(0), data(NULL), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

Array::Array(uint32_t _len):
    len(_len), refcount(new int(0))
{
    data = (float*)malloc(sizeof(float) * len);
    *refcount = *refcount + 1; // 构造函数，分配内存，refcount 分配内存，然后+1
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

// 拷贝构造函数
Array::Array(const Array& arr):
    refcount(arr.refcount) // 用 arr.refcount赋值给refcount
{
    if(refcount) {
        // 如果 arr.refcount 不等于 NULL，说明 arr 是自己管理 data 数据
        // 那么现在要和 arr 对象共享 data 从而构造一个新的 Array 对象，就需要计数器+1
        *refcount = *refcount + 1;
    }
    len = arr.len;
    data = arr.data;
}

Array::~Array()
{
    // 如果 refcount 不为 NULL，说明 data 字段是需要管理的堆内存
    if (refcount && (*refcount)>0 ) { // 如果 refcount 不为 NULL，并且 *refcount > 0，说明至少有1个Array对象，绑定着data字段
        *refcount = *refcount - 1; // 那么就让计数器减1
        
        if (*refcount == 0) { // 计数器减去1后，值为0，说明没有人再指向data这块内存了
            free(data); // 则释放这块内存
            data = NULL;
            free(refcount); // 并且引用计数器也要释放掉
            refcount = NULL;
        }
    }
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

        Array arr3;
        arr3 = arr1;

        Array arr4(len);
        arr4 = arr1;

        // arr1, arr2
        // 当程序运行到大括号外，arr1和arr2的生命周期结束，会分别调用析构函数
        // 由于增加了引用计数器，并且拷贝构造函数、析构函数中，根据引用计数器来释放内存
        // 因此，看起来不会内存泄露或二次释放了（对于单线程的拷贝构造函数，确实如此）

        // arr1, arr3
        // 由于arr3创建时没指定参数，因此不用自己申请和管理内存
        // 把arr1赋值给arr3时，由于没定义复制赋值函数，调用编译器生成的默认的复制赋值函数
        // 也就是 len 和 data 字段，完全相同
        // 这其实不合理：因为明明有 arr1 和 arr3 两个对象在共享 data 字段，计数器的值却是1
        // 虽然析构函数里不会二次释放或内存泄露，但如果arr1的声明周期结束、arr3没结束，
        // 则后续arr3访问data对象，就是非法访问了

        // arr1, arr4
        // 这里的问题在于，arr4创建的时候指定了len参数，因此需要自行创建和管理自己的data，也就是堆内存
        // 而由于没有实现Array类的复制赋值函数，调用了编译器生成的默认复制赋值函数
        // 因此，arr1的len和data字段赋值给arr4对象，而arr4对象原来的data则无人管理
        // 这就造成了内存泄露
    }
    

    return 0;
}