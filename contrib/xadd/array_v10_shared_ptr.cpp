#ifdef _MSC_VER
#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <memory>
#include <thread>

class Array {
public:
    Array();
    explicit Array(uint32_t _len);
    Array(uint32_t _len, float* data);
    Array(const Array& arr); // 拷贝构造函数
    Array& operator=(const Array& arr); // 复制赋值构造函数
    ~Array();

public:
    std::shared_ptr<float> data;
    int len;
};

Array::Array() :
    len(0), data(NULL)
{

}

Array::Array(uint32_t _len) :
    len(_len)
{
    if (len>0) {
        data.reset(new float[len], [](float* p) {
            delete[] p;
        });
    }
}

Array::Array(uint32_t _len, float* _data) :
    len(_len), data(_data) // 用外部内存 _data 给 data 赋值，bad!
{
    // 外部的内存应该外部申请和释放，不应该交给 shared_ptr 来管理
}

// 拷贝构造函数
Array::Array(const Array& arr):
    len(arr.len),
    data(arr.data)
{
}

Array& Array::operator=(const Array& arr)
{
    if (this != &arr)
    {
        data = arr.data;
        len = arr.len;
    }
    return *this;
}

Array::~Array()
{
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

Array g_arr;

void do_work()
{
    int loop_counter = 2000000; // 循环次数；过少则不容易触发线程不安全问题，可以比1000再大一些
    // 触发的问题有三种（均在VS2019 x64 Debug模式下测试发现）：
    // 第一种：内存泄露，vld会报告出来(loop_counter=1000或2000时出现)；非必现，但概率很高
    // 第二种：运行表现为死循环，长时间不出结果 (loop_counter等于2000或更大值时出现），中等概率
    // 第三种：非法内存访问，表现为VS弹窗 Debug Assertion Failed. (loop_counter=2000时出现）
    //
    // 在Release 模式下，相对更难复现问题，需要增加loop_counter到200000，或更大，才能发现“异常结果”
    //   (返回值不等于0，等于-1073740940)
    while (loop_counter--) {
        Array tmp_arr = g_arr;
    }
}

void test_multithread()
{
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();//开始时间

    std::thread th1(do_work);
    std::thread th2(do_work);
    std::thread th3(do_work);
    std::thread th4(do_work);
    std::thread th5(do_work);
    std::thread th6(do_work);
    std::thread th7(do_work);
    std::thread th8(do_work);
    std::thread th9(do_work);
    std::thread th10(do_work);

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();

    std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();//结束时间
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    std::cout << "共耗时：" << time_span.count() << " s" << std::endl; // 耗时
}



int main()
{

    //test_copy_ctor();
    //test_assign_op_case1();
    //test_assign_op_case2();
    //test_assign_op_case3();
    //test_assign_op_case4();

    g_arr = Array(10);
    test_multithread();

    // float* data = (float*)malloc(10*sizeof(float));
    // Array some(10, data);
    // free(data); // 第一次释放
    // 第二次释放是 some 的析构函数释放

    //test_thread_safe();

    return 0;
}
