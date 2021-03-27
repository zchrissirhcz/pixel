# matcalc

matrix calculation related stuffs, including basic blas-like operations.

## u8 matrix column max (u8 矩阵逐列求最大值)

image info: height=4032, width=3024 (river_bank2.png)

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a  |
| -- | ----------- | ---------- | ------- |
| 1  | opencv      |  166 ms    |  86 ms  |
| 2  | naive       |  6.8 ms    |  75 ms  |
| 3  | 缓存行       |  6.4 ms    | 5.3 ms  |
| 4  | 缓存行 asimd |  -         | 0.45 ms |

android-armv8加速比：time(opencv)/time(缓存行asimd)=**191倍**


## u8 matrix column max and idx (u8 矩阵逐列求最大值和对应索引)

image info: height=4032, width=3024 (river_bank2.png)

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a  |
| -- | ----------- | ---------- | ------- |
| 1  | opencv      |  237 ms    |   88 ms  |
| 2  | naive       |  167 ms    |   78 ms  |
| 3  | 缓存行       |  10 ms    |  5.3 ms  |
| 4  | 缓存行 asimd |  -         | 1.46 ms |

android-armv8加速比：time(opencv)/time(缓存行asimd)=**60倍**


## mean std dev, input u8 (uchar输入类型，算均值和方差)

image info: name=river_bank2.png, height=3024, width=4032

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a   |
| -- | ----------- | ---------- | -------- |
| 1  | opencv      |  5.6 ms    |  2 ms    |
| 2  | naive       |  11.2 ms   |  15 ms   |
| 3  | asimd1      |   -        |  3.5 ms  |
| 4  | 等价公式     |    9 ms    |  11 ms   |
| 5  | 等价公式 asimd2 | -       |  2.2 ms  |

## array mean u8 (uchar输入类型，计算均值)

image info: name=river_bank2.png, height=3024, width=4032

vs2019-x64: i7-4790
armv8a: XiaoMi 11 (QCOM888)

| id | method      | vs2019-x64 | armv8a   | armv7a    |
| -- | ----------- | ---------- | -------- | --------- |
| 1  | opencv      |  4.21 ms   |0.6/0.8 ms|  1.29 ms  |
| 2  | naive       |  2.53 ms   |  3.5 ms  |  8.64 ms  |
| 3  | asimd1      |  -         |  1.95 ms |  3.28 ms  |
| 4  | asimd2      |  -         |  1.42 ms |  4.62 ms  |
| 5  | asimd3      |  -         |  1.08 ms |  2.69 ms  |
| 6  | asimd4      |  -         |  1.04 ms |  3.79 ms  |
| 7  | asimd5      |  -         |  1.07 ms |  1.79 ms  |
| 8  | asimd6      |  -         |  0.57 ms |  -        |
| 8  | asimd7      |  -         |  0.65 ms |  1.28 ms  |

**References**

https://zhuanlan.zhihu.com/p/128640600

## f32 matrix add / subtract

image info: name=river_bank2.png, height=3024, width=4032

| id | method     | armv8      | armv7      |
| -- | ---------- | ---------- | ---------- |
| 1  | eigen      | 14.68 ms   | 18.84 ms   |
| 2  | opencv     | 14.35 ms   | 17.21 ms   |
| 3  | naive      | 14.08 ms   | 21.44 ms   |
| 4  | asimd      | 14.03 ms   | 14.83 ms   |

neon优化在armv7时有明显提升，armv8时几乎没提升。

## f32 GEMM

image info: name=colorhouse.png, height=512, width=512


| id | method     | armv8      | armv7      |
| -- | ---------- | ---------- | ---------- |
| 1  | eigen      | 6.22 ms    | 26.92 ms   |
| 2  | opencv     | 32.14 ms   | 78.93 ms   |
| 3  | naive      | 202.98 ms  | 204.07 ms  |
| 4  | order opt  | 16.77 ms   | 137.21 ms  |
| 5  | order opt2 | 17.21 ms   | 157.08 ms  |
| 6  | asimd      | 24.30 ms   | 107.93 ms  |

neon优化在armv7时有明显提升，armv8时反而最慢。（TODO继续优化）

## Eigen Notes

### 1. 快速入门
0. Eigen是header-only library，直接包含头文件然后使用
1. 最常用类型：`Eigen::MatrixXd`，表示任意尺寸(`X`)的矩阵，每个元素是`double`类型(`d`)。
2. 对于`MatrixXd`类型，对应的头文件是`#include <Eigen/Dense>`
3. `VectorXd`类型，表示任意行(`X`)的列向量
4. 用带尺寸的类型，如Matrix3d，Vector3d，好处是速度快，坏处是编译慢并且生成二进制更大。
5. 当矩阵小于等于4x4尺寸时，建议用带尺寸的类型。

### 2. `Matrix`类：
1. 类型定义

所有的矩阵、向量类型，都是`Matrix`模板类的对象；`Matrix`模板类声明如下：
```C++
Matrix<typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
```
第一个参数`Scalar`表示系数类型，后两个表示尺寸。例如`Matrix4f`定义为：
```c++
typedef Matrix<float, 4, 4> Matrix4f;
```
Eigen中的向量默认为列向量，啥意思呢，比如`Vector3f`定义为：
```c++
typedef Matrix<float, 3, 1> Vector3f;
```
而行向量则是行数为1的矩阵，例如`RowVector2i`定义为：
```c++
typedef Matrix<int, 1, 2> RowVector2i;
```

对于编译时无法确定具体尺寸的矩阵、向量类型，它们的typedef定义中，第二第三个参数是Dynamic。例如最常用的`MatrixXd`类型定义为：
```c++
typedef Matrix<double, Dynamic, Dynamic> MatrixXd;
```
而对于行数不确定的列向量类型`VectorXi`，定义为：
```c++
typedef Matrix<int, Dynamic, 1> VectorXi;
```
还可以按需求自行定义，例如行数为3、列数不固定的float系数矩阵：
```C++
Matrix<float， 3, Dynamic>
```

2. 构造函数

`Matrix`类的默认构造函数，也就是“啥也不写”的情况下，不会做任何动态内存分配，也不会给系数初始化。例如：
```c++
Matrix3f a; // a是3x3矩阵，对应数组是float[9]，系数没有初始化的（怎么做到的？？）
MatrixXf b; // b是动态尺寸的矩阵，当前尺寸是0x0，并且系数也是没有初始化的
```
而对于小尺寸列向量(<=4行)，也提供了方便的系数初始化函数：
```c++
Vector2d a(5.0, 6.0);
Vector3d b(5.0, 6.0, 7.0);
Vector4d c(5.0, 6.0, 7.0, 8.0);
```

3. 访问系数（元素）

对于矩阵和向量，都可以使用`()`来访问元素;
对于向量，还可以用`[]`来访问元素。
注意：C++语言的`[i,j]`等价于`[i]`因此矩阵不能`[]`来访问元素。

4. 逗号初始化

重载`<<`操作符实现的。

5. 获取维度、元素数量；resize

对于所有类型矩阵/向量，都可以用：`.cols()`, `.rows()`, `.size()`方法。

`.resize()`方法仅适合动态尺寸的类型；如果定义时是固定尺寸的类型，则resize到不同大小时会触发assert失败。

6. `Matrix`模板类的后三个（有默认值的）模板参数

```C++
Matrix<typename Scalar,
       int RowsAtCompileTime,
       int ColsAtCompileTime,
       int Options = 0,
       int MaxRowsAtCompileTime = RowsAtCompileTime,
       int MaxColsAtCompileTime = ColsAtCompileTime>
```
Options表示“是否用RowMajor"，为0表示ColMajor，为1表示RowMajor。
实际上`RowMajor=1, ColMajor=0`是定义好了的量（枚举？）。
`MaxRowsAtCompileTime`和`MaxColsAtCompileTime`表示编译时候能确定的维度最大值，在`RowsAtCompileTime`或`ColsAtCompileTime`为Daynamic时，指定了它们两个，可以避免动态内存分配。例如：
```c++
Matrix<float, Dynamic, Dynamic, 0, 3, 4>
```
表示的类型，内部用的是12个float元素的数组。

7. typedef说明

- `MatrixNt`是`Matrix<type, N, N>`，也就是系数类型为type的NxN方阵
- `VectorNt`是`Matrix<type, N, 1>`，也就是系数类型为type的N行的列向量
- `RowVectorNt`是`Matrix<type, 1, N>`，也就是系数类型为type的N列的行向量
其中，
- N可以是2, 3, 4或X（表示动态尺寸）
- t可以是`i`(int), `f`(float), `d`(double), `cf`(`complex<float>`)或`cd`(`complex<double>`)之一（也就是说，Eigen的typedef定义的类型中，t只有i,f,d,cf,cd这5种取值）
- 对于没有被提到的其他类型，仍然可作为Scalar参数，例如uchar, short等。

### 3. map函数

经常会处理其他数据结构和Eigen的转换，比如把opencv的mat转为eigen的matrix，或者std::vector的填入matrix。在不进行拷贝的情况下可以使用eigen的map功能进行内存映射。

1维原始数组共享给Eigen矩阵（感觉用Vector更合理）：
```c++
Eigen::Map<Eigen::Matrix<float, 1, Eigen::Dynamic, Eigen::RowMajor>> eA(mA, len);
```

1维原始数组按照2维数组共享给Matrix矩阵：
```c++
Eigen::Map<Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> eA(mA, M, N);
```

注意：用`Eigen::Mat`的话，不会对矩阵系数做拷贝，仅仅是指针/引用操作，耗时可认为是0 ms。


### 疑问
1. 为什么Eigen默认用ColMajor存储？感觉会慢，而且和其他C/C++库会不一致。

2. 为什么`cols()`和`rows()`方法，返回值类型是`long`这个不友好的类型？