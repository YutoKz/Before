#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 1000000
#define I1 10   // (1) 一つのnに関する繰り返し数
#define I2 0   // (2) 一つのnに関する繰り返し数


typedef struct _tri 
{
    int a;
    int b;
    int c;
} TRI;



typedef struct _node2
{
    TRI *tri;
    struct _node2 *left;
    struct _node2 *right;
} NODE2;

typedef struct _node10
{
    TRI *tri;
    struct _node10 *n0;
    struct _node10 *n1;
    struct _node10 *n2;
    struct _node10 *n3;
    struct _node10 *n4;
    struct _node10 *n5;
    struct _node10 *n6;
    struct _node10 *n7;
    struct _node10 *n8;
    struct _node10 *n9;
} NODE10;

NODE2 *root2;
NODE10 *root10;



TRI *A2[N]; /* 2 分ヒープ用配列*/
TRI *A10[N]; /* 10 分ヒープ用配列*/
int n2; /* 2 分ヒープに格納済みのデータ数*/
int n10; /* 10 分ヒープに格納済みのデータ数*/



double triarea(TRI *p)   // 面積計算
{
    double s = (double)(p->a + p->b + p->c) / 2.0;
    return sqrt(s * (s - p->a) * (s - p->b) * (s - p->c));
}


int tricmp(TRI *p1, TRI *p2)   // 面積　> 1   < -1   = 0
{
    if(triarea(p1) > triarea(p2))
    {
        return 1;
    }
    else if(triarea(p1) < triarea(p2))
    {
        return -1;
    }
    else
    {
        return 0;
    }
}



int insert2heap(TRI *p) /* 2 分ヒープへの挿入*/   // 返り値は tricmp の呼び出し回数
{
    n2++;
    A2[n2 - 1] = p;
    int count = 0;

    int child = n2 - 1, parent;
    while(child != 0)
    {
        count++;
        parent = (child - 1) / 2;
        if(tricmp(A2[parent], A2[child]) > 0)
        {
            A2[child] = A2[parent];
            A2[parent] = p;
            child = parent;
        }
        else
        {
            return count;
        }
    }
    return count;
}



int insert10heap(TRI *p){ /* 10 分ヒープへの挿入*/
    n10++;
    A10[n10 - 1] = p;
    int count = 0;

    int child = n10 - 1, parent;
    while(child != 0)
    {
        count++;
        parent = (child - 1) / 10;
        if(tricmp(A10[parent], A10[child]) > 0)
        {
            A10[child] = A10[parent];
            A10[parent] = p;
            child = parent;
        }
        else
        {
            return count;
        }
    }
    return count;
}









int deletemin2heap(){ /* 2 分ヒープからの取り出し*/
    if(n2 == 0)
        return -1;
    
    if(n2 == 1)
    {
        n2 = 0;
        return 0;
    }

    A2[0] = A2[n2 - 1];
    n2--;

    int count = 0;
    int parent = 0, min_index;
    TRI* min_value;
    while((2 * parent + 1) <= (n2 - 1))
    {
        if((2 * parent + 1) == (n2 - 1))   // 左の子しか存在せず
        {
            min_index = 2 * parent + 1;
            min_value = A2[min_index];
        }
        else   // 左右の子存在
        {
            count++;
            if(tricmp(A2[2 * parent + 1], A2[2 * parent + 2]) > 0)   // 面積　左子 > 右子
            {
                min_index = 2 * parent + 2;
                min_value = A2[2 * parent + 2];
            }
            else   // 面積　左子 <= 右子
            {
                min_index = 2 * parent + 1;
                min_value = A2[2 * parent + 1];
            }
        }

        count++;
        if(tricmp(A2[parent], min_value) > 0)   // 親の方が子の最小より面積大きいならば
        {
            A2[min_index] = A2[parent];
            A2[parent] = min_value;
            parent = min_index;
        }
        else
        {
            return count;
        }
    }
    return count;
}


int deletemin10heap(){ /* 10 分ヒープからの取り出し*/
    if(n10 == 0)
        return -1;
    
    if(n10 == 1)
    {
        n10 = 0;
        return 0;
    }

    A10[0] = A10[n10 - 1];
    n10--;
    
    int count = 0;
    int parent = 0, min_index;
    TRI* min_value;
    while((10 * parent + 1) <= (n10 - 1))
    {
        if((10 * parent + 1) == (n10 - 1))   // 子が n1 しか存在せず
        {
            min_index = 10 * parent + 1;
            min_value = A10[min_index];
        }
        else   // 子が複数存在
        {
            min_index = 10 * parent + 1;
            int i;
            for(i = 2; i <= 10; i++)
            {
                count++;
                if(tricmp(A10[10 * parent + i], A10[min_index]) < 0)
                {
                    min_index = 10 * parent + i;
                }
            }

            min_value = A10[min_index];
        }

        count++;
        if(tricmp(A10[parent], min_value) > 0)   // 親の方が子の最小より面積大きいならば
        {
            A10[min_index] = A10[parent];
            A10[parent] = min_value;
            parent = min_index;
        }
        else
        {
            return count;
        }
    }
    return count;
}


TRI* create_tri()
{
    TRI *p;
    p = (TRI*)malloc(sizeof(TRI));

    int a, b, c;
    while(1)
    {
        a = (rand() % 99) + 1;
        b = (rand() % 99) + 1;
        c = (rand() % 99) + 1;

        if((a < b + c) && (b < a + c) && (c < a + b))
            break;
    }

    p->a = a;
    p->b = b;
    p->c = c;

    return p;
}















int main(int argc, char const *argv[])
{
    int n, i, j;
    TRI* p;
    int count = 0;

    printf("(1)\ninsert2heap\n");
    for(n = 100; n <= 10000; n += 100)
    {
        printf("%d, ", n);
        for(i = 0; i < I1; i++)
        {
            n2 = 0;
            count = 0;
            for(j = 0; j < n; j++)
            {
                p = create_tri();
                count += insert2heap(p);
            }
            printf("%d, ", count);
        }
        printf("\n");
    }

    printf("\ninsert10heap\n");
    for(n = 100; n <= 10000; n += 100)
    {
        printf("%d, ", n);
        for(i = 0; i < I1; i++)
        {
            n10 = 0;
            count = 0;
            for(j = 0; j < n; j++)
            {
                p = create_tri();
                count += insert10heap(p);
            }
            printf("%d, ", count);
        }
        printf("\n");
    }






    printf("\n(2)\ndeletemin2heap\n");
    for(n = 10000; n <= N; n += 50000)
    {
        printf("%d, ", n);
        for(i = 0; i < I2; i++)
        {
            n2 = 0;
            count = 0;
            for(j = 0; j < n; j++)
            {
                p = create_tri();
                insert2heap(p);
            }

            for(j = 0; j < 10000; j++)
            {
                count += deletemin2heap();
            }
            printf("%d, ", count);
        }
        printf("\n");
    }

    printf("\ndeletemin10heap\n");
    for(n = 10000; n <= N; n += 50000)
    {
        printf("%d, ", n);
        for(i = 0; i < I2; i++)
        {
            n10 = 0;
            count = 0;
            for(j = 0; j < n; j++)
            {
                p = create_tri();
                insert10heap(p);
            }

            for(j = 0; j < 10000; j++)
            {
                count += deletemin10heap();
            }
            printf("%d, ", count);
        }
        printf("\n");
    }

    return 0;
}
