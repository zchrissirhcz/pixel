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
    Array& operator=(const Array& arr); // 复制赋值构造函数
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

Array& Array::operator=(const Array& arr)
{
    // 当前array对象有两种状态：refcount不为空，或为空
    // 传入的参数对象arr也有两种状态：refcount不为空，或为空
    // 因此一共有4种状态
    if (this != &arr)
    {
        //0 0
        if (this->refcount == NULL && arr.refcount == NULL) {
            data = arr.data;
            len = arr.len;
        }

        //0 1
        else if (this->refcount == NULL && arr.refcount != NULL) {
            data = arr.data;
            len = arr.len;
            refcount = arr.refcount;
            *refcount = *refcount + 1;
        }

        //1 0
        else if (this->refcount != NULL && arr.refcount == NULL) {
            // 首先让this->refcount减少一次引用
            *refcount = *refcount - 1;
            // 如果refcount=0了，那应该释放data和refcount
            if (*refcount == 0) {
                free(data);
                free(refcount);
                data = NULL;
                refcount = NULL;
            }

            // 然后从arr 赋值
            data = arr.data;
            len = arr.len;
        }

        //1 1
        else if (this->refcount != NULL && arr.refcount != NULL) {
            *refcount = *refcount - 1;
            if (*refcount == 0) {
                free(data);
                free(refcount);
                data = NULL;
                refcount = NULL;
            }

            data = arr.data;
            len = arr.len;
            refcount = arr.refcount;
            *refcount = *refcount + 1;
        }
    }

    // 如果传入的arr就是自己，那什么都不应该做
    // 如果前面的refcount和其他成员的赋值弄好了，也在这里返回
    return *this; 
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


// 引用计数测试 - 拷贝构造函数
void test_copy_ctor()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);
        Array arr2 = arr1;
        Array arr3 = arr1;
    }

    {
        Array arr1;
        Array arr2 = arr1;
        Array arr3 = arr2;
    }

    float* data = (float*)malloc(sizeof(float) * 10);
    {
        Array arr1(10, data);
        Array arr2 = arr1;
    }
    free(data);
}

// 引用计数测试 - 复制赋值函数,case1
// 当前计数为0，参数计数也为0
void test_assign_op_case1()
{
    {
        int len = 10;
        float* data = (float*)malloc(sizeof(float) * len);
        {
            Array arr1(len, data);
            Array arr2;

            arr2 = arr1;
        }
        free(data);
    }

    {
        Array arr1;
        Array arr2;
        arr2 = arr1;
    }

    {
        Array arr1;
        arr1 = arr1;
    }
}

// 引用计数测试 - 复制赋值函数,case2
// 当前计数为0，参数计数不为0
void test_assign_op_case2()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2;

        arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);

        Array arr2;
        Array arr3;

        arr2 = arr1;
        arr3 = arr1;
    }
}

// 引用计数测试 - 复制赋值函数,case2
// 当前计数不为0，参数计数为0
void test_assign_op_case3()
{
    {
        Array arr1;

        int len = 10;
        Array arr2(len);

        arr2 = arr1;
    }

    {
        Array arr1;

        int len = 10;
        Array arr2(len);

        Array arr3 = arr2;

        arr2 = arr1;
    }
}

// 引用计数测试 - 复制赋值函数,case2
// 当前计数不为0，参数计数不为0
void test_assign_op_case4()
{
    {
        int len = 10;
        Array arr1(len);
        Array arr2(len);

        arr2 = arr1;
    }

    {
        int len = 10;
        Array arr1(len);

        Array arr2 = arr1;

        Array arr3 = Array(len);

        arr2 = arr3;
    }

    {
        Array arr1(10);
        arr1 = arr1;
    }
}


int main()
{

    test_copy_ctor();
    test_assign_op_case1();
    test_assign_op_case2();
    test_assign_op_case3();
    test_assign_op_case4();

    return 0;
}
