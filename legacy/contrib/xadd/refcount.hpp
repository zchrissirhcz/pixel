// from http://www.cs.cmu.edu/~lemur/doxygen/lemur-3.1/html/RefCount_8hpp-source.html
// seems not working

#pragma once

template<class T>
class RefCount
{
private:
    struct Ref {
        T* const ptr;
        unsigned int count;

        Ref(T* p): ptr(p), count(1) {}
        ~Ref() {delete ptr;}
        Ref* increment() {++count; return this;}
        bool decrement() { return (--count==0);}
    private:
        Ref(const Ref&);
        Ref& operator=(const Ref&);
    }* ref;
public:
    explicit RefCount(T* p=0):
        ref(p? new Ref(p): 0)
    {
    }

    RefCount(const RefCount<T>& other):
        ref(other.ref ? other.ref->increment() : 0)
    {
    }
    ~RefCount()
    {
        if (ref && ref->decrement()) {
            delete ref;
        }
    }
    RefCount<T>& operator=(const RefCount<T>& other)
    {
        Ref* tmp = other.ref ? other.ref->increment() : 0;
        if (ref &&ref->decrement()) {
            delete ref;
        }
        ref = tmp;
        return *this;
    }

    operator T*() const
    {
        return ref ? ref->ptr : 0;
    }
    T* operator->() const
    {
        return ref ? ref->ptr : 0;
    }
    T* get() const
    {
        return ref ? ref->ptr : 0;
    }
    template<class newType> operator RefCount<newType>()
    {
        return RefCount<newType>(ref);
    }
};
