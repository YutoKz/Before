
// おそらくこれでいけた
// 0620 cooy 2.c とまったく同じ


#include <stdio.h>
#include <stdlib.h>

#define B 1093
#define occupied 0
#define empty 1
#define deleted 2
#define N 1000              // ここを変えてみる



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
int v[N][100];



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
    return (x*x*x + count) % B;

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
        else if(s == occupied)    // 問題あり
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
    FILE *ofp;
    if ((ofp = fopen("result1.txt", "w")) == NULL) {
        printf("File cannot be opened.\n");
        exit(0);
    }


    init_hash();

    int n, c;
    int i;


    printf("n\tc\n");
    for(n = 0; n < N; n++)
    {
        printf("%d\t%d\n", n, create_tri());

        for(i = 0; i < 100; i++)
        {
            v[n][i] = search_tri();   // 1個からN個
        }    
    }
    printf("\n\n");

    for(n = 0; n < N; n++)
    {
        for(i = 0; i < 100; i++)
        {
            fprintf(ofp, "%d\t", v[n][i]);
        }
        fprintf(ofp, "\n");
    }

    fclose(ofp);
    
    return 0;
}
