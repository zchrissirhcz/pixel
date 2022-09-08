


#include <iostream>

class Student
{
public:
    Student();
    ~Student();
    float math; // 数学成绩
    float chinese; // 语文成绩
};

Student::Student()
{
    math = 0;
    chinese = 0;
}

Student::~Student()
{

}

template<typename T1, typename T2>
auto my_max(T1 t1, T2 t2)
{
    return t1>t2 ? t1 : t2;
}

auto my_max(Student stu1, Student stu2)
{
    auto stu1_sum = stu1.chinese + stu1.math;
    auto stu2_sum = stu2.chinese + stu2.math;
    return stu1_sum>stu2_sum ? stu1_sum : stu2_sum;
}



int main()
{
    std::cout << my_max(12, 34.5) << std::endl;

    std::cout << my_max(12, 9.6) << std::endl;

    std::cout << my_max("34", "56") << std::endl;

    //-----------
    Student stu1, stu2;

    stu1.chinese = 109;
    stu1.math = 89;

    stu2.chinese = 119;
    stu2.math = 106;

    std::cout << my_max(stu1, stu2) << std::endl;

    return 0;

}
