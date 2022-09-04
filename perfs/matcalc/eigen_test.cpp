#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

using std::cout;
using std::cin;
using std::endl;

using Eigen::MatrixXd;
using Eigen::MatrixXf;
using Eigen::Matrix;
using Eigen::VectorXd;
using Eigen::Matrix3d;
using Eigen::Matrix3f;
using Eigen::Vector3d;

#define LOGD(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)

float dotproduct_eigen(size_t len, float* va, float* vb)
{
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> vva(va, len);
    Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> vvb(vb, len);
    float res = vva.dot(vvb);
    return res;
}

static void matrix_add_f32_eigen(float* mA, float* mB, float* mC, const size_t M, const size_t N)
{
    using namespace Eigen;
    Map<Matrix<float, Dynamic, Dynamic, RowMajor>> eA(mA, M, N);
    Map<Matrix<float, Dynamic, Dynamic, RowMajor>> eB(mB, M, N);
    Map<Matrix<float, Dynamic, Dynamic, RowMajor>> eC(mC, M, N);
    eC = eA + eB;
}

// 例子1：创建2x2矩阵，逐元素赋值，然后输出
static void eigen_example1()
{
    LOGD("--- %s ---\n", __FUNCTION__);
    MatrixXd m(2,2); // MatrixXd是最常用的Eigen数据类型，X表示任意尺寸，d表示double。这里创建的是2x2规格的矩阵。
    m(0,0) = 3; // 追元素赋值，注意是用小括号，这应该是重载了括号操作符
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    cout << m << endl; // 输出矩阵，直接std::cout即可，说明有重载<<操作符
}

// 矩阵乘以向量的例子。
static void eigen_example2()
{
    LOGD("--- %s ---\n", __FUNCTION__);
    MatrixXd m = MatrixXd::Random(3,3); //创建3x3矩阵，元素为[-1,1]之间的随机浮点数

    m = (m+MatrixXd::Constant(3, 3, 1.2)) * 50;//[-1,1] + 1.2 = [0.2, 2.2]; [0.2, 2.2]*50=[10, 110]
    // 也就是把原本在[-1,1]之间的各个元素，映射到[10, 110]之间

    cout << "m=" << endl << m << endl;

    VectorXd v(3); // 创建一个3行的列向量v

    v << 1, 2, 3; // 给列向量赋值，分别为1,2,3

    cout << "m*v=" << endl << m*v << endl; // 计算 m*v，也就是矩阵乘以向量，并输出结果
}

// eigen_example2的另一个版本：使用具体尺寸的类型，而不是MatrixXd和VectorXd
static void eigen_example2_2()
{
    LOGD("--- %s ---\n", __FUNCTION__);
    Matrix3d m = Matrix3d::Random();
    m = (m+Matrix3d::Constant(1.2)) * 50;
    cout << "m=" << endl << m << endl;
    Vector3d v(1,2,3);
    cout << "m*v=" << endl << m*v << endl;
}

// 访问系数（元素）
static void eigen_example3()
{
    LOGD("--- %s ---\n", __FUNCTION__);
    MatrixXd m(2,2);
    m(0, 0) = 3;
    m(1, 0) = 2.5;
    m(0, 1) = -1;
    m(1, 1) = m(1, 0) + m(0, 1);
    cout << "Here is the matrix m:\n" << m << endl;

    VectorXd v(2);
    v(0) = 4;
    v(1) = v(0) - 1;
    cout << "Here is the vector v:\n" << v << endl;
}

// 用逗号表达式初始化
static void eigen_example4()
{
    LOGD("--- %s ---\n", __FUNCTION__);
    Matrix3f m; // 声明

    // 初始化。注意：必须先声明，才能用逗号表达式初始化；定义时直接用逗号表达式初始化会报错
    // 因为这个初始化其实是通过重载<<操作符实现的
    m << 1, 2, 3,
       4, 5, 6,
       7, 8, 9;
    cout << m << endl;
}

// 输出维度信息、元素个数；动态尺寸矩阵/向量的resize
static void eigen_example5()
{
    LOGD("--- %s ---\n", __FUNCTION__);
    MatrixXd m(2, 5);
    m.resize(4, 3);
    printf("The matrix m is of size: rows=%d, cols=%d, num elements=%d\n",
        m.rows(), m.cols(), m.size());
    
    VectorXd v(2);
    v.resize(5);
    std::cout << "The vector v is of size " << v.size() << endl;
    printf("As a matrix, v is of size: rows=%d, cols=%d\n", v.rows(), v.cols());
}

// 动态尺寸矩阵赋值，如果尺寸不同，则自动把等号左边的矩阵resize
static void eigen_example5_2()
{
    LOGD("--- %s ---\n", __FUNCTION__);
    MatrixXf a(2, 2);
    printf("originally, a's size: row=%d, col=%d\n", a.rows(), a.cols());
    MatrixXf b(3, 3);
    a = b;
    printf("after assign, a's size: row=%d, col=%d\n", a.rows(), a.cols());
}

// eigen_example5_2的修改，从固定尺寸(3x3)的矩阵，赋值给动态维度的2x2矩阵，隐式resize
static void eigen_example5_3()
{
    LOGD("--- %s ---\n", __FUNCTION__);
    MatrixXf a(2, 2);
    printf("originally, a's size: row=%d, col=%d\n", a.rows(), a.cols());
    Matrix3f b;
    b << 1, 2, 3,
       4, 5, 6,
       7, 8, 9;
    a = b;
    printf("after assign, a's size: row=%d, col=%d\n", a.rows(), a.cols());
}

int main() {
    eigen_example1();
    eigen_example2();
    eigen_example2_2();
    eigen_example3();
    eigen_example4();
    eigen_example5();
    eigen_example5_2();
    eigen_example5_3();

    return 0;
}