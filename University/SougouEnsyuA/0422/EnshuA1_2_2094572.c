#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pivot(int k, int n, double **x, double **y)
{
    double max_value = x[k][k];
    int max_line = k;

    int i,j;
    for(i=k+1; i<n; i++)
    {
        if(fabs(x[i][k]) > fabs(max_value))
        {
            max_value = x[i][k];
            max_line = i;
        }
    }
    if(max_line != k)
    {
        double temp;
        for(j=k; j<n; j++)
        {
            temp = x[k][j];
            x[k][j] = x[max_line][j];
            x[max_line][j] = temp;
        }
        for(j=0; j<n; j++)
        {
            temp = y[k][j];
            y[k][j] = y[max_line][j];
            y[max_line][j] = temp;
        }
    }
}





int main(int argc, char *argv[])
{
    FILE *fp;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Cannot open file\n");
        return 1;
    }

    /* 変数の数を数える */
    int lines = 0;  /* linesに行数すなわち変数の数が格納 */
    char line[64];
    while(fgets(line, 64, fp) != NULL)
        lines++;
    rewind(fp);

    /* メモリの動的確保 */
    /* 元の行列a及びその逆行列c */
    int i, j, k;
    double **a, **c;
    a = malloc(lines * sizeof(double*));
    c = malloc(lines * sizeof(double*));
    for(i=0; i<lines; i++)
    {
        a[i] = malloc(lines * sizeof(double));
        c[i] = malloc(lines * sizeof(double));
    }
    
    /* 動的に確保した配列に行列のデータ格納 */
    for(i=0; i<lines; i++)
    {
        for(j=0; j<lines; j++)
            fscanf(fp, "%lf ", &(a[i][j]));
        for(j=0; j<lines; j++)
            if(i==j)
                c[i][j] = 1;
            else
                c[i][j] = 0;
    }

    fclose(fp);  /* 以上、配列a及びc 初期設定完了 */



    /* 消去 */
    for(k=0; k<=(lines-1); k++)
    {
        /* 対角要素が０に近ければpivot操作 */
        if((a[k][k] > -0.00005) && (a[k][k] < 0.00005))
        {
            pivot(k, lines, a, c);
            if((a[k][k] > -0.00005) && (a[k][k] < 0.00005))   /* rankが落ちたと判定し、プログラム終了 */
            {
                printf("No inverse Matrix!\n");
                return 1;
            }
        }
        
        /* 対角成分[k,k]を1にするためk行をa[k][k]で割る */
        for(j=k+1; j<lines; j++)
            a[k][j] /= a[k][k];
        for(j=0; j<lines; j++)
            c[k][j] /= a[k][k];
        a[k][k] = 1;
        
        for(i=0; i<lines; i++)
        {
            if(i != k)
            {
                for(j=k+1; j<lines; j++)
                    a[i][j] = a[i][j] - a[k][j]*a[i][k];
                for(j=0; j<lines; j++)
                    c[i][j] = c[i][j] - c[k][j]*a[i][k];
                a[i][k] = 0;
            }
        }
    }



    /* 答えの出力 */
    printf("inverse A =\n");
    for(i=0; i<lines; i++)
    {
        for(j=0; j<lines; j++)
            printf("%10.4f   ", c[i][j]);
        printf("\n");
    }

    for(i=0; i<lines; i++)
    {
        free(a[i]);
        free(c[i]);
    }
    return 0;
}
