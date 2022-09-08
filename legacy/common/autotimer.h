/**
 * 基于 P-Impl 惯用法实现的自动计时器
 * 在构造函数中开始计时，在析构函数中打印累计耗时
 * 析构函数是在生命周期结束时调用
 */

#pragma once

#include <string>

class AutoTimer
{
public:
    explicit AutoTimer(const std::string& name);
    ~AutoTimer();
private:
    AutoTimer(const AutoTimer&);
    AutoTimer& operator=(const AutoTimer&);
    class Impl;
    Impl* mImpl;
};
