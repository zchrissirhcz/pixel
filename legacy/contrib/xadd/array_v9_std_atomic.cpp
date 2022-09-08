#ifdef _MSC_VER
#include <vld.h> // 用于内存泄露检查。仅在MSVC Debug模式下测试用。
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>

#include <thread> // 用于验证多线程 MY_XADD 是否安全的 test 代码编写
#include <mutex> // 用于验证锁操作对 MY_XADD 是否有效的尝试
#include <atomic>

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
    //int* refcount; // 引用计数器。使用 volatile 修饰，希望能解决多线程访问冲突
    std::atomic<int>* refcount;
    void addref();
    void release();
};

static int MY_XADD(std::atomic<int>* addr, int delta) {
    int tmp(addr->load());
    *addr += delta;

    return tmp;
}

// 引用计数器增加1次
void Array::addref() {
    if (refcount) {
        MY_XADD(refcount, 1);
    }
}

// 引用计数器减少1次
// 如果计数器等于0，则释放data和refcount并且置NULL
void Array::release() {
    // 如果 refcount 不为 NULL，那么：
    //    更新计数器： *refcount = *refcount - 1
    //    判断如果原来的计数器值为1，那么现在计数器的值为0，应当释放相关内存
    if (refcount && MY_XADD(refcount, -1) == 1) {
        // end for refcount
        delete(refcount);
        refcount = NULL;

        // end for Array object's shared heap memory variable(s)
        free(data);
        data = NULL;
    }
}

Array::Array() :
    len(0), data(NULL), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

Array::Array(uint32_t _len) :
    len(_len)
{
    if (len>0) {
        refcount = new std::atomic<int>();
        std::atomic_init(refcount, 0);
        data = (float*)malloc(sizeof(float) * len);
        addref(); // 构造函数，分配内存，refcount 分配内存，然后+1
    }
}

Array::Array(uint32_t _len, float* data) :
    len(_len), data(data), refcount(NULL) // 构造函数，不分配内存，refcount初始化为NULL
{

}

// 拷贝构造函数
Array::Array(const Array& arr):
    len(arr.len),
    data(arr.data),
    refcount(arr.refcount) // 用 arr.refcount赋值给refcount
{
    addref();
}

// 复制赋值函数
Array& Array::operator=(const Array& arr)
{
    // 当前array对象有两种状态：refcount不为空，或为空
    // 传入的参数对象arr也有两种状态：refcount不为空，或为空
    // 因此一共有4种状态
    if (this != &arr)
    {
        release();
        refcount = arr.refcount;
        addref();

        data = arr.data;
        len = arr.len;
    }

    // 如果传入的arr就是自己，那什么都不应该做
    // 如果前面的refcount和其他成员的赋值弄好了，也在这里返回
    return *this;
}

Array::~Array()
{
    release();
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

// 通过这个例子说明，std::atomic<int>类型作为引用计数，不合适
// 无法保证引用同一内存的所有对象的计数器相同
// (但 std::atomic<int>* 之间，可以相互赋值)
// void atomic_test()
// {
//     std::atomic<int> a;

//     a = 3;

//     std::atomic<int> b;
//     std::atomic_init(&b, a);

//     a = 4;

//     std::cout << a << std::endl; // 输出4
//     std::cout << b << std::endl; // 输出3
// }

// void atomic_test2()
// {
//     std::atomic<int> a(3);
//     int b = atomic_fetch_add(&a, 1);
//     std::cout << a << std::endl;
//     std::cout << b << std::endl;
// }

int main()
{

    //test_copy_ctor();
    //test_assign_op_case1();
    //test_assign_op_case2();
    //test_assign_op_case3();
    //test_assign_op_case4();

    g_arr = Array(10);
    test_multithread();
    //atomic_test();
    //atomic_test2();

    //test_thread_safe();

    return 0;
}
