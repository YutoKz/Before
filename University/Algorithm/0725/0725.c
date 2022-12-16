#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000000
#define K 100000.0

typedef struct _node {
    int data;
    struct _node *left;
    struct _node *right;
} NODE;

NODE *root = NULL;

clock_t start, end;




// 補助的な関数
NODE *create_node(int d)
{
    NODE *p;
    p = (NODE*)malloc(sizeof(NODE));
    p -> data = d;
    p -> left = NULL;
    p -> right = NULL;
    return p;
}

void inorder(NODE* n)
{
    if(n == NULL)
    {
        return;
    }
    inorder(n -> left);
    printf("%d ", n -> data);
    inorder(n -> right);
}




// 問題の関数
int insert(int x)
{
    NODE* p = root;
    int count = 0;

    if(p == NULL)
    {
        root = create_node(x);
        return count;
    }

    while(p != NULL)
    {
        count++;
        if(x == (p -> data))
        {
            return -1;
        }
        else if(x < (p -> data))
        {
            if((p -> left) == NULL)
            {
                p -> left = create_node(x);
                return count;
            }
            else
            {
                p = p -> left;
            }
        }
        else
        {
            if((p -> right) == NULL)
            {
                p -> right = create_node(x);
                return count;
            }
            else
            {
                p = p -> right;
            }
        }
    }
}






int main(void)
{
    // int r[N];   // 重複がないことが保証された乱数列
    // int c[N];   // 各データの挿入時の比較回数

    int* r = malloc(N * sizeof(int));
    int* c = malloc(N * sizeof(int));

    int n;
    int i, tmp;

    for(i = 0; i < N; i++)   // N=10^6 とかにすると、明らかにここで果てしない時間がかかっている
    {
        do                                                                                    // ここ人力でその都度修正必要あり
        {
            tmp = (double)rand() / ((double)RAND_MAX + 1.0) * 1000;    // 0以上999以下の乱数
            tmp = tmp * 1000 + (double)rand() / ((double)RAND_MAX + 1.0) * 1000;       //  0以上999以下を足す
            tmp++;   // これで tmp は 1 以上 N 以下の乱数

            r[i] = tmp;
            c[i] = insert(tmp);   
        } while(c[i] < 0);
    }

    printf("      n, T       , C\n");

    for(/*n = 0*/n = 20000; n <= (N - K); /*n += 100000*/ n += 20000)   // ここ調整してみる
    {
        int j;
        double c_ave = 0.0;

        root = NULL;
        for(j = 0; j < n; j++)   // n個のデータの挿入
        {
            insert(r[j]);
        }

        start = clock();

        for(j = n; j <= (n + K - 1); j++)   // K個のデータの挿入
        {
            insert(r[j]);
        }

        end = clock();

        for(j = n; j <= (n + K - 1); j++)
        {
            c_ave += c[j];
        }

        c_ave /= K;

        printf("%7d, %f, %f\n", n, (double)(end - start)/CLOCKS_PER_SEC, c_ave);
    }




    /*
    for(i = 0; i < N; i++)   // N=10^6 とかにすると、明らかにここで果てしない時間がかかっている
    {
        do                                                                                    // ここ人力でその都度修正必要あり
        {
            r = (double)rand() / ((double)RAND_MAX + 1.0) * 1000;    // 0以上999以下の乱数
            r = r * 1000 + (double)rand() / ((double)RAND_MAX + 1.0) * 1000;       //  0以上999以下を足す
            r++;
            // printf("%7d %d\n", i, r);
            start = clock();
            c[i] = insert(r);   // rは1以上1000000以下の乱数
            end = clock();
        } while(c[i] < 0);
        
        t[i] = (double)(end - start)/CLOCKS_PER_SEC;

        if(i % 10000 == 0)
            printf("%d, %f, %d\n", i, t[i], c[i]);
    }
    */



/*
    printf("n, T, C\n");

    int n, j;
    for(n = 1; n <= (N - K); n += 1000)
    {
        double T = 0.0;
        double C = 0.0;
        for(j = n; j <= n + K - 1; j++)
        {
            T += t[j];
            C += c[j];
        }
        C /= K;
        printf("%7d, %f, %d\n", n, T, C);
    }

    return 0;
*/





    // test
    /*
    printf("%d\n", insert(4));
    insert(8);
    insert(2);
    insert(10);
    insert(6);
    insert(1);
    insert(7);
    insert(3);
    insert(9);
    insert(5);
    
    printf("%d\n", insert(11));
    printf("%d\n\n", insert(10));
    printf("%d\n", insert(11));

    inorder(root);
    */
}
