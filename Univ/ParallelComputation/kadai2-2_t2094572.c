/*
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define N 16000

double y[N];


int main( int argc, char **argv )
{
    int i, j;
    int nprocs, myrank, start, end;
    MPI_Status status;
    double error, ans, nd = N, t0, t1;
    double **a, *x;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    start = (nd / nprocs) * myrank;
    end = (nd / nprocs) * (myrank + 1) - 1;
    
    int n = end - start + 1;
    
    a = malloc(N * sizeof(double*));
    for(i = 0; i < N; i++)
        a[i] = malloc(n * sizeof(double));
    x = malloc(n * sizeof(double));

    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();
    
    // 行列の初期化
    for(i = 0; i < n; i++)
    {
        x[i] = (double)(i + start + 1);
    }
    
    for(i = 0; i < N; i++)
    {
        for(j = 0; j < n; j++)
        {
            a[i][j] = (double)(i + 1) + (double)(j + start + 1);
        }
    }
    
    // 部分和ベクトルの計算
    double yp[N];

    for(i = 0; i < N; i++)
    {
        yp[i] = 0.0;
        for(j = 0; j < n; j++)
        {
            yp[i] += a[i][j] * x[j];
        }
    }
    

    // Rreduction
    MPI_Reduce(&yp[0], &y[0], N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();
    
    
    error = 0.0;
    
    for(i = 0; i < N; i++)
    {
        ans = ((double)i+1)*nd*(nd+1)/2 + nd*(nd+1)*(2*nd+1)/6;
        error = error + fabs( y[i] - ans );
    }
    if(myrank == 0)
    {
        printf("error = %10.5f\n", error);
        printf("Elapsed time: %lf\n", t1 - t0);
        printf("%f\n", y[5]);
    }
    
   
    
    
    for(i = 0; i < N; i++)
        free(a[i]);
    free(x);
    
    MPI_Finalize();
    return 0;
}

*/
