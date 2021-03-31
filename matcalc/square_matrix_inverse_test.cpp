#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 全排列的递归(dfs)函数
void permute_dfs(int t, int n, int x, int* path, int* visit, int** perm_lst, int* cnt)
{
    if(t==n) {
        // for(int i=1; i<=n; i++) {
        //     if(i>1) printf(" ");
        //     printf("%d", path[i]);
        // }
        // printf("\n");

        int* perm_item = (int*)malloc(n*sizeof(int));
        memcpy(perm_item, path+1, n*sizeof(int));
        perm_lst[*cnt] = perm_item;

        *cnt = (*cnt) + 1;
    }

    for(int i=1; i<=n; i++) {
        if(!visit[i]) {
            visit[i] = 1;
            path[t+1] = i;
            permute_dfs(t+1, n, i, path, visit, perm_lst, cnt);
        }
    }

    visit[x] = 0;
}

static void print_2d_array(int** data, int total_len, int each_len)
{
    for (int i=0; i<total_len; i++) {
        for (int j=0; j<each_len; j++) {
            printf("%d ", data[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

static void free_2d_array(int** data, int n)
{
    if (data) {
        for (int i=0; i<n; i++) {
            if(data[i]) {
                free(data[i]);
            }
        }
        free(data);
    }
}

// 阶乘
static int factorial(int n) {
    int res = 1;
    for (int i=1; i<=n; i++) {
        res = res * i;
    }
    return res;
}

// 输入数字n，算出1~n的全排列
static void permute_test()
{
    int n;
    scanf("%d", &n);
    size_t buf_size = (n+1)*sizeof(int);
    int* path = (int*)malloc(buf_size);
    memset(path, 0, buf_size);

    int* visit = (int*)malloc(buf_size);
    memset(visit, 0, buf_size);
    
    int cnt = 0;
    int perm_len = factorial(n);
    //printf("factorial(%d)=%d\n", n, perm_len);
    int** perm_lst = (int**)malloc(sizeof(int*)*perm_len);
    memset(perm_lst, 0, sizeof(int*)*n);
    permute_dfs(0, n, 0, path, visit, perm_lst, &cnt);

    print_2d_array(perm_lst, perm_len, n);

    free(path);
    free(visit);
    free_2d_array(perm_lst, perm_len);
}

// 按照定义，求行列式的值
// @param data: nxn 尺寸的矩阵
static float eval_determinant(float* data, int n)
{
    size_t buf_size = (n+1)*sizeof(float);
    int* path = (int*)malloc(buf_size);
    memset(path, 0, buf_size);

    int* visit = (int*)malloc(buf_size);
    memset(visit, 0, buf_size);

    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            //TODO: 使用每一个全排列的值作为下标，访问矩阵里的元素，并累乘
        }
    }

    return 0;
}

// static void list_node_test()
// {
//     int* data = (int*)malloc(sizeof(int)*3);
//     data[0] = 1;
//     data[1] = 2;
//     data[2] = 3;
//     PermNode* node = create_perm_node(data, 3);

//     data[0] = 2;
//     data[1] = 1;
//     data[2] = 3;
//     PermNode* node2 = create_perm_node(data, 3);

//     PermList* list = create_perm_list();
//     perm_list_append_node(list, node);
//     perm_list_append_node(list, node2);

//     print_perm_list(list);

//     free(data);
// }

int main () {

    permute_test();
    //list_node_test();

    return 0;
}


/*
// 按定理计算det(A)：按第一行展开。需要知道代数余子式的含义

#define N 10

double getA(double arcs[N][N],int n);
bool GetMatrixInverse(double src[N][N],int n,double des[N][N]);
void  getAStart(double arcs[N][N],int n,double ans[N][N]);

//主要函数如下：
//得到给定矩阵src的逆矩阵保存到des中。
bool GetMatrixInverse(double src[N][N],int n,double des[N][N])
{
    double flag=getA(src,n);
    double t[N][N];
    if(flag==0)
    {
        return false;
    }
    else
    {
        getAStart(src,n,t);
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                des[i][j]=t[i][j]/flag;
            }

        }
    }
    return true;
}

//计算|A|:
//按第一行展开计算|A|
double getA(double arcs[N][N],int n)
{
    if(n==1)
    {
        return arcs[0][0];
    }
    double ans = 0;
    double temp[N][N]={0.0};
    int i,j,k;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-1;j++)
        {
            for(k=0;k<n-1;k++)
            {
                // 构造 (n-1)x(n-1)规格的子行列式，它由A[0][i]所在行列之外的元素组成
                temp[j][k] = arcs[j+1][(k>=i)?k+1:k];
            }
        }
        double t = getA(temp,n-1);
        if(i%2==0)
        {
            ans += arcs[0][i]*t;
        }
        else
        {
            ans -= arcs[0][i]*t;
        }
    }
    return ans;
}

//计算伴随矩阵：
//计算每一行每一列的每个元素所对应的余子式，组成A*
void  getAStart(double arcs[N][N],int n,double ans[N][N])
{
    if(n==1)
    {
        ans[0][0] = 1;
        return;
    }
    int i,j,k,t;
    double temp[N][N];
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            for(k=0;k<n-1;k++)
            {
                for(t=0;t<n-1;t++)
                {
                    temp[k][t] = arcs[k>=i?k+1:k][t>=j?t+1:t];
                }
            }
            ans[j][i]  =  getA(temp,n-1);
            if((i+j)%2 == 1)
            {
                ans[j][i] = - ans[j][i];
            }
        }
    }
}

int main() {
    // double src[N][N] = {
    //     {1, 2, 3, 4, 5},
    //     {0, 1, 4, 1, 2},
    //     {5, 6, 0, 1, 0},
    //     {2, 4, 1, 1, 2},
    //     {1, 3, 5, 2, 4}
    // };

    double src[N][N] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {0, 1, 4, 1, 2, 1, 1, 1, 1, 1},
        {5, 6, 0, 1, 0, 2, 2, 2, 2, 2},
        {2, 4, 1, 1, 2, 3, 3, 3, 3, 3},
        {1, 3, 5, 2, 4, 4, 4, 4, 4, 4},
        {6, 2, 3, 2, 5, 5, 5, 5, 1, 5},
        {7, 8, 7, 7, 8, 1, 2, 3, 1, 2},
        {0, 1, 2, 1, 0, 3, 4, 5, 4, 3},
        {3, 3, 3, 1, 2, 6, 8,-6,-3, 1},
        {-2,2, 3, 3, 4, 1, 0, 0, 2, 0}
    };

    int n = N;
    double des[N][N];
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            des[i][j] = 0;
        }
    }
    GetMatrixInverse(src, n, des);

    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            printf("%3.2f ", des[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}
*/