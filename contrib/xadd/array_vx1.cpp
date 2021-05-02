#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

class Array{
public:
    Array(uint32_t _len=0);
    uint32_t len;
    float* data;

    // 通过将拷贝构造函数、复制赋值函数声明为private、并且不实现它们
    // 避免了编译器自动生成它们，并且一旦客户调用，则编译阶段触发报错
    // 如果是 public 则无法通过链接，改 private 就可以通过
    // 对应 Effective C++ 条款06
    // C++11 则可以用 = delete 来做到
private:
    Array(const Array&);
    Array& operator=(const Array&);
};

Array::Array(uint32_t _len):
    len(_len), data(NULL)
{
    if (len>0) {
        data = (float*)malloc(sizeof(float)*len);
    }
}

int main()
{
    Array arr(10);
    Array arr2;
    arr2 = arr; // 编译报错
    Array arr3(arr); // 编译报错

    return 0;
}
