#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N -0.00005

int d_min_index(double d[5])
{
    int i, ans = 0;

    for(i = 1; i < 5; i++)
    {
        if(d[i] < d[ans])
            ans = i;
    }
    return ans;
}

int a_min_row_index(double a[][5], double b[3], int column)
{
    int i;
    double temp[3];
    for(i = 0; i < 3; i++)
        temp[i] = b[i] / a[i][column];
        
    int ans = 0;                      
    for(i = 1; i < 3; i++)
        if(temp[i] < temp[ans])
            ans = i;

    return ans;
}

int main(void)
{
    FILE *fp;

    fp = fopen("simplex_pro_2022_ex.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open file\n");
        return 1;
    }

    int i, j, k;
    double a[3][5], b[3], d[5], basis_index[3];
    
    /* 初期設定 */
    for(i = 0; i < 2; i++)
        fscanf(fp, "%lf ", &(d[i]));
    
    for(i = 2; i < 4; i++)
        d[i] = 0;
    
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 5; j++)
            fscanf(fp, "%lf ", &(a[i][j]));
        fscanf(fp, "%lf ", &(b[i]));
    }

    for(i = 0; i < 3; i++)
        fscanf(fp, "%lf ", &(basis_index[i]));

    fclose(fp);
    /* 以上　初期設定完了 */


    double obj_func = 0;
    int ans_index[2];


    /* ループ */
    while(d[d_min_index(d)] < N)
    {
        int d_min = d_min_index(d);
        int a_min_row = a_min_row_index(a, b, d_min);        

        for(i = 0; i < 5; i++)
        {
            if(i != d_min)
                a[a_min_row][i] /= a[a_min_row][d_min];
        }
        b[a_min_row] /= a[a_min_row][d_min];
        a[a_min_row][d_min] = 1;

        for(i = 0; i < 3; i++)
        {
            if(i != a_min_row)
            {
                double temp = a[i][d_min];
                for(j = 0; j < 5; j++)
                    a[i][j] -= a[a_min_row][j] * temp;
                    
                b[i] -= b[a_min_row] * temp;
            }
        }

        double temp = d[d_min];
        for(j = 0; j < 5; j++)
            d[j] -= a[a_min_row][j] * temp;
        
        basis_index[a_min_row] = d_min;

    }  /* ループ終了 */  

    for(i = 0; i < 3; i++)
    {
        if(basis_index[i] == 0)
        {
            obj_func += 150.0 * b[i];
            ans_index[0] = i;
        }
        else
        if(basis_index[i] == 1)
        {
            obj_func += 180.0 * b[i];
           ans_index[1] = i;
        }
    }

    
    printf("\nTotal Sales = %.1f yen\n\n", obj_func);
    printf("Production Volume:\nCurry and Rice = %.1f gram, Nikudon = %.1f gram\n\n", 100.0 * b[ans_index[0]], 100.0 * b[ans_index[1]]);
    printf("Surplus Ingredients:\nMeat = %.1f gram, Onion = %.1f gram, Rice = %.1f gram\n\n", 
            5000.0 - (25.0 * b[ans_index[0]] + 50.0 * b[ans_index[1]]), 5300 - (40.0 * b[ans_index[0]] + 20.0 * b[ans_index[1]]), 4000 - (25.0 * b[ans_index[0]] + 20.0 * b[ans_index[1]]));

    return 0;
}


