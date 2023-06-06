/*
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#define N 16000

double a[N][N], x[N], y[N];


int main( int argc, char **argv )
{
    
    int nprocs, myrank, start, end;
    MPI_Status status;
    double error, ans, nd = N, t0, t1;
    int i, j;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    start = (nd / nprocs) * myrank;
    end = (nd / nprocs) * (myrank + 1) - 1;
    
    
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();
    
    // 行列の初期化
    for(i = start; i <= end; i++)
    {
        x[i] = (double)(i + 1);
    }
    
    for(i = 0; i < N; i++)
    {
        for(j = start; j <= end; j++)
        {
            a[i][j] = (double)(i + 1) + (double)(j + 1);
        }
    }
    
    // 部分和ベクトルの計算
    double yp[N];

    for(i = 0; i < N; i++)
    {
        yp[i] = 0.0;
        for(j = start; j <= end; j++)
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
    }
    
    MPI_Finalize();
    return 0;
}

*/
