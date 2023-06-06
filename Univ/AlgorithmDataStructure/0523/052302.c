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

int search_data(int d)
{
    int count = 0;
    CELL *p = head;
    while(p != NULL)
    {
        if(p -> data == d)
        {
            return count;
        }
        count++;
        p = p -> next;
    }
    return (-1)*count; 
}








int main(int argc, char const *argv[])
{
    int n_max = atoi(argv[1]);
    int i_max = atoi(argv[2]);
    int i, n, d;
    clock_t start1, end1, start2, end2;
    printf("Number of cells = %d\n\n", n_max);
    

    // まずセル数 n_max の連結リストを用意
    //int r[n_max];
    int *r = malloc(n_max * sizeof(int));
    for(n = 0 ; n < n_max; n++)
    {
        r[n] = (double)rand() / ((double)RAND_MAX + 1.0) * n_max;
    }

    for(n = 0; n < n_max; n++)
    {
        insert_data(r[n]);
    }
/*
    CELL *tmp = head;
    while(tmp != NULL)
    {
        printf("%d  ", tmp -> data);
        tmp = tmp -> next;
    }
    printf("\n\n");
*/

    printf("d\tresult\ttime\n");
    
    start1 = clock();
    for(i = 0; i < i_max; i++)
    {
        d = (double)rand() / ((double)RAND_MAX + 1.0) * n_max;
        start2 = clock();
        printf("%d\t%d\t", d, search_data(d));
        end2 = clock();
        printf("%f\n", (double)(end2-start2)/CLOCKS_PER_SEC);
    }
    end1 = clock();
    printf("\n%f\n", (double)(end1-start1)/CLOCKS_PER_SEC);




    

        /*
        // 正常に格納できているか確認
        CELL *tmp = head;
        while(tmp != NULL)
        {
            printf("%d  ", tmp -> data);
            // printf("ok \n");
            tmp = tmp -> next;
        }
            printf("\n\n\n");
        */

    /*
    CELL *now = head;
    head = NULL;
    CELL *nex;
    while(now != NULL)
    {
        nex = now -> next;
        free(now);
        now = nex;
    }
    */
    
    // }
    

}