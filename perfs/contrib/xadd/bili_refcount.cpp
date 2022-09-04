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

    CStudent& operator=(CStudent& obj);

    void release();

    void Show()
    {
        cout << hex << (int&)m_pszName << m_pszName << endl; // 槽点2：使用了delete后的内存
    }

private:
    char* m_pszName;
};

CStudent::CStudent(const char* pszName)
{
    m_pszName = new char[256];
    strcpy(m_pszName, pszName);
}

CStudent::CStudent(CStudent& obj)
{
    m_pszName = obj.m_pszName;

    //strcpy(m_pszName, obj.m_pszName);
}

CStudent& CStudent::operator=(CStudent& obj)
{
    m_pszName = obj.m_pszName;
    return *this;
}

void CStudent::release()
{
    if (m_pszName!=NULL)
    {
        delete m_pszName; // 槽点1：没用 delete[]
        m_pszName = NULL;
    }
}

int main()
{
    CStudent stu1("zhang san");
    CStudent stu2("li si");
    CStudent stu3 = stu2;

    stu1.Show();
    stu2.Show();
    stu3.Show();

    stu2.release();
    stu3.Show();

    return 0;
}
