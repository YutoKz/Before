
// おそらくこれでいけた
// 0620 cooy 2.c とまったく同じ


#include <stdio.h>
#include <stdlib.h>

#define B 17117
#define occupied 0
#define empty 1
#define deleted 2
#define N 15000              // ここを変えてみる
#define I 1000 // (3)繰り返し回数


typedef struct _tri
{
    int a;
    int b;
    int c;
} TRI;

typedef struct _harray
{
    TRI *tri;
    int state;
} HARRAY;



HARRAY H[B];
int v[I][N];



void init_hash()
{
    int i;
    for(i = 0; i < B; i++)
        H[i].state = empty;
}



int correspond(TRI *p1, TRI *p2)   // 同一三角形なら 1 を返す
{
    if(((p1->a) == (p2->a)) && ((p1->b) == (p2->b)) && ((p1->c) == (p2->c)))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



int h(TRI *p, int count)   // count はリハッシュの回数　添え字みたいなもん　　　あとから中身変えて試すことになりそう
{
    
    int x = p -> a;
    return (x*x*x + count*count) % B;

}   // 返り値はハッシュ値 (index)



int insert_hash(TRI *p)   // 返り値は基本衝突回数
{
    int i, count = 0;   // 衝突回数であり、かつハッシュ関数の添え字
    int hp = h(p, count);

    for(i = 0; i < B; i++)
    {
        if(H[hp].state != occupied)
        {
            H[hp].tri = p;
            H[hp].state = occupied;
            return count;
        }
        else if(correspond(p, H[hp].tri))
        {
            return -1;
        }
        else
        {
            count++;
            hp = h(p, count);
        }
    }

    return -2;
}



int search_hash(TRI *p)   // p と同一の三角形を探索する  返り値は基本 発見までに状態を確認した配列要素数
{
    int i, count = 0;
    int hp = h(p, count);
    int s;

    for(i = 0; i < B; i++)
    {
        s = H[hp].state;
        count++;
        if(s == empty)
        {
            return (-1)*count;
        }
        else if(s == occupied)
        {
            if(correspond(p, H[hp].tri) == 1)
                return count;
        }

        hp = h(p, count);
    }
}



int create_tri()
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

    return insert_hash(p);
}



int search_tri()
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

    return search_hash(p);
}












int main()
{
    FILE *ofp, *ofp1, *ofp2;
    if ((ofp = fopen("result.txt", "w")) == NULL) {
        printf("File cannot be opened.\n");
        exit(0);
    }
    if ((ofp1 = fopen("result1.txt", "w")) == NULL) {
        printf("File cannot be opened.\n");
        exit(0);
    }
    if ((ofp2 = fopen("result2.txt", "w")) == NULL) {
        printf("File cannot be opened.\n");
        exit(0);
    }


    init_hash();

    int n, c;
    int i;

    
    for(n = 0; n < N; n++)
    {
        fprintf(ofp, "%f\t%d\n", (double)n / (double)B, create_tri());

        if(n % 100 == 0)
            for(i = 0; i < I; i++)
                v[i][n] = search_tri();   // 1個からN    
    
    }

    for(n = 0; n < N; n++)
    {
        if(n % 100 == 0)
        {
            fprintf(ofp1, "%f, ", (double)n / (double)B);
            fprintf(ofp2, "%f, ", (double)n /*/ (double)B*/);

            for(i = 0; i < I; i++)
            {
                if(v[i][n] < 0)
                {
                    fprintf(ofp1, "%d, ", v[i][n]);
                    // fprintf(ofp2, ", ");
                }
                else
                {
                    // fprintf(ofp1, ", ");
                    fprintf(ofp2, "%d, ", v[i][n]);
                }
            }
            fprintf(ofp1, "\n");
            fprintf(ofp2, "\n");
        }
    }

    fclose(ofp1);
    fclose(ofp2);
    
    return 0;
}
