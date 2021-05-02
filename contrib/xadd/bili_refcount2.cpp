#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <string.h>

#include <iostream>

using namespace std;

class CStudent
{
public:
    CStudent(const char* pszName);
    CStudent(CStudent& obj);

    ~CStudent();
    CStudent& operator=(CStudent& obj);
    void release();
    void Show()
    {
        //if (*m_pCount>0) { // 槽点7：应该先判断 m_pCount 是否为 NULL
        if (m_pCount && (*m_pCount)>0) {
            cout << hex << (int&)m_pszName << " " << m_pszName << endl;
        }
    }

private:
    char* m_pszName;
    int* m_pCount;
};

CStudent::CStudent(const char* pszName)
{
    m_pszName = new char[256];
    m_pCount = new int;

    strcpy(m_pszName, pszName);
    *m_pCount = 1;
}

CStudent::CStudent(CStudent& obj)
{
    printf("copy constructor\n");
    // m_pszName = obj.m_pszName;

    // m_pCount = obj.m_pCount;
    //(*m_pCount) ++; // 槽点6：应该先release，再从obj赋值
    
    //release();
    m_pszName = obj.m_pszName;
    m_pCount = obj.m_pCount;
    *m_pCount = *m_pCount + 1;
}

CStudent::~CStudent()
{
    release();
}

CStudent& CStudent::operator=(CStudent& obj)
{
    printf("--- copy assign operator\n");
    if (obj.m_pszName == m_pszName)
    {
        return *this;
    }

    //if (m_pCount && --(*m_pCount)==0)
    if (m_pCount && *m_pCount>0)
    {
        *m_pCount = *m_pCount - 1;

        if (*m_pCount == 0) {
            //delete m_pszName; // 槽点5：应该使用 delete[]
            delete[] m_pszName;
            m_pszName = NULL;
            delete m_pCount;
            m_pCount = NULL; // 槽点3：没有赋值为NULL
        }
    }

    m_pszName = obj.m_pszName;
    m_pCount = obj.m_pCount;

    *m_pCount = *m_pCount + 1;
    return *this;
}

void CStudent::release()
{
    //if (m_pszName!=NULL && --*m_pCount==0) // 槽点4：运算符优先级弄错
    //printf("--- m_pCount is %d\n", *m_pCount);
    if (m_pCount && (*m_pCount)>0)
    {
        *m_pCount = *m_pCount - 1;

        if (*m_pCount==0) {
            //delete m_pszName; // 槽点1：没用 delete[]
            if (m_pszName!=NULL) {
                delete[] m_pszName;
            }
            m_pszName = NULL;

            delete m_pCount;
            m_pCount = NULL;
        }
    }
}

int main()
{
    {
        //CStudent stu1("zhang san");
        CStudent stu2("li si");
        CStudent stu3 = stu2;

        //stu1.Show();
        stu2.Show();
        stu3.Show();

        stu2.release();
        stu3.Show();
    }

    return 0;
}
