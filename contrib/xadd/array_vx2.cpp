#include <iostream>
#include <memory>

class Array
{
public:
    Array(uint32_t _len=0);

public:
    uint32_t len;
    std::shared_ptr<float> data;
};

Array::Array(uint32_t _len):
    len(_len), data(nullptr)
{
    if (len>0) {
        data.reset(new float[len], [](float* p) {
            delete[] p;
        });
    }
}

int main()
{
    Array arr(10);

    return 0;
}
