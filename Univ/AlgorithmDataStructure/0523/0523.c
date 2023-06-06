#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _cell
{
    int data;
    struct _cell *next;
} CELL;

CELL *head;


void insert_data(int d)
{
    if((d % 2) == 1 || head == NULL)
    {
        CELL *new_cell = (CELL*) malloc(sizeof(CELL));
        new_cell -> data = d;
        new_cell -> next = head;
        head = new_cell;
    }
    else
    {
        CELL *p = head;
        while(p -> next != NULL)
        {
            p = p -> next;
        }
        CELL *new_cell = (CELL*) malloc(sizeof(CELL));
        new_cell -> data = d;
        new_cell -> next = p -> next;
        p -> next = new_cell;
    }
}


int main(int argc, char const *argv[])
{
    int i_max = atoi(argv[1]);
    int n_max = atoi(argv[2]);
    int i, n;

    printf("%d\n", n_max);
    
    // 計測の繰り返し
    for(i = 0; i < i_max; i++)
    {
    


        int r[n_max];
        for(n = 0 ; n < n_max; n ++)
        {
            r[n] = (double)rand() / ((double)RAND_MAX + 1.0) * n_max;
        }



        // n_max 回の関数処理にかかる時間の計測
        clock_t start, end;
        start = clock();



        for(n = 0; n < n_max; n++)
        {
            insert_data(r[n]);
        }



        end = clock();







        // 計測結果の出力
        printf("%f\n", (double)(end-start)/CLOCKS_PER_SEC);

        
        // 正常に格納できているか確認
        CELL *tmp = head;
        while(tmp != NULL)
        {
            printf("%d\n", tmp -> data);
            // printf("ok \n");
            tmp = tmp -> next;
        }
            printf("\n\n\n");
        

        CELL *now = head;
        head = NULL;
        CELL *nex;
        while(now != NULL)
        {
            nex = now -> next;
            free(now);
            now = nex;
        }
    
    }
    

}