/*

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "cntary.h"
#include <mpi.h>
#include <omp.h>

#define N 1000

const int ITRmax=1000;
const int INTV=1;
int nds;
double dt, dr, kappa, alpha, beta, s, sdt, diff_local, diff_sum, t_max;
double **T, **T_new, **T_global;

void output(FILE* fp, double** data, int isize, int jsize);
void gpscript();





int main(int argc, char **argv)
{
    int nprocs, myrank;
    MPI_Status  status;
    int lower, upper;
    double t0, t1;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    
    int start, end;   // i
    
    
    
    int  i, j, tstep;
    FILE* fp;

    // Define parameters
    dr = 1.0/N;
    kappa = 1.0;
    dt = dr*dr*0.25/kappa;
    s  = 50.0;
    
    beta  = kappa*dt/dr/dr; // !write here!
    alpha = 1.0 - 4.0*beta; // !write here!
    sdt   = s*dt;

    
    
    // Allocation
    // T = carray2d(N+1, N+1, 0, 0); // !write here!
    // T_new = carray2d(N+1, N+1, 0, 0); // !write here!
    if((N - 1) % nprocs == 0)
    {
        start = ((N-1) / nprocs) * myrank + 1;
        end = ((N-1) / nprocs) * (myrank + 1);
    }
    else
    {
        if(myrank != (nprocs - 1))
        {
            start = (((N - 1) / nprocs) + 1) * myrank + 1;
            end = (((N - 1) / nprocs) + 1) * (myrank + 1);
        }
        else
        {
            start = (((N - 1) / nprocs) + 1) * myrank + 1;
            end = N - 1;
        }
    }
    
    int nsdom = end - start + 1;
    int *disps = (int*)malloc(nprocs*sizeof(int));
    int *counts = (int*)malloc(nprocs*sizeof(int));
    MPI_Allgather(&start, 1, MPI_INT, &disps[0], 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Allgather(&nsdom, 1, MPI_INT, &counts[0], 1, MPI_INT, MPI_COMM_WORLD);
    
    #pragma omp parallel for
    for(i=0; i<nprocs; i++)
    {
        disps[i] = disps[i]*(N+1);
        counts[i] = counts[i]*(N+1);
    }
    
    T = carray2d(end - start + 3, N+1, start - 1, 0);
    T_new = carray2d(end - start + 3, N+1, start - 1, 0);
    
    T_global = carray2d(N+1, N+1, 0, 0);
    
    
    
    // Initialization
    // !write here!
    #pragma omp parallel for private(j)
    for(i=start-1; i<=end+1; i++)
    {
        for(j=0; j<=N; j++)
        {
            T    [i][j] = 0.0;
            T_new[i][j] = 0.0;
        }
    }

    // 1st-time data output
    nds = -1;
    t_max = 0.0;
    fp = fopen("T2d.dat", "w");
    
    MPI_Gatherv(&T[start][0], nsdom*(N+1), MPI_DOUBLE, &T_global[0][0], counts, disps, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(myrank == 0)
        output(fp, T_global, N+1, N+1);

    
    // Time advancement
    tstep = 0;
    
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();
    do
    {
        tstep++;

        // solve Heat Equation
        // *new value T(t+Dt): T_new[][], old value T(t): T[][]
        // *boundary condition: T[0][0] = T_new[0][0] = T[N][N] = T_new[N][N] = 0.0
        // !write here!
        lower = myrank - 1;
        if(myrank == 0)
            lower = MPI_PROC_NULL;
        
        upper = myrank + 1;
        if(myrank == nprocs - 1)
            upper = MPI_PROC_NULL;
        
        MPI_Sendrecv(&T[end][0], (N+1), MPI_DOUBLE, upper, (myrank+1)*100, &T[start-1][0], (N+1), MPI_DOUBLE, lower, (lower+1)*100, MPI_COMM_WORLD, &status);
        MPI_Sendrecv(&T[start][0], (N+1), MPI_DOUBLE, lower, (myrank+1)*(-100), &T[end+1][0], (N+1), MPI_DOUBLE, upper, (upper+1)*(-100), MPI_COMM_WORLD, &status);
        
        
        
        
        #pragma omp parallel for private(j)
        for(i=start; i<=end; i++)
        {
            for(j=1; j<N; j++)
            {
                T_new[i][j] = alpha*T[i][j] + beta*(T[i+1][j] + T[i-1][j] + T[i][j+1] + T[i][j-1]) + sdt;
            }
        }
        

        // check a difference between T(t+Dt) & T(t)
        // !write here!
        diff_local = 0.0;
        #pragma omp parallel
        {
            #pragma omp for reduction(+:diff_local) private(j)
            for(i=start; i<=end; i++)
            {
                for(j=1; j<N; j++)
                {
                    diff_local += pow((T_new[i][j]-T[i][j]), 2);
                }
            }
        }
        
        MPI_Allreduce(&diff_local, &diff_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        
        diff_sum = sqrt(diff_sum);

        // Store T(t+Dt) into T[]
        // !write here!
        #pragma omp parallel for private(j)
        for(i=start; i<=end; i++)
        {
            for(j=1; j<N; j++)
            {
                T[i][j] = T_new[i][j];
            }
        }

        // Data output during iteration
        if( tstep%INTV == 1 )
        {
            MPI_Gatherv(&T[start][0], nsdom*(N+1), MPI_DOUBLE, &T_global[0][0], counts, disps, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            if(myrank == 0)
                output(fp, T_global, N+1, N+1);
        }
        
        
    } while ( (diff_sum > 1.0e-10) && (tstep < ITRmax) ) ;
    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();
    

    if ( tstep < ITRmax )
        printf("Converged at: tstep = %d\n", tstep );
    else
        printf("Reach ITRmax before converged: diff = %9.3e\n", diff_sum );

    // Final data output
    MPI_Gatherv(&T[start][0], nsdom*(N+1), MPI_DOUBLE, &T_global[0][0], counts, disps, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(myrank == 0)
    {
        output(fp, T_global, N+1, N+1);
        printf("Elapsed time: %lf\n", t1 - t0);
    }
    fclose(fp);
    if(myrank == 0)
        gpscript();

    MPI_Finalize();
    return 0;

}

*/


/* write T[], calculate max value of T[] */
/*
void output(FILE* fp, double** data, int isize, int jsize)
{
  int i, j;

  for(i=0; i<isize; i++){
    for(j=0; j<jsize; j++){
      t_max = (t_max > data[i][j]) ? t_max : data[i][j];
      fprintf(fp, "%f  %f  %f\n", j*dr, i*dr, data[i][j]);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "\n\n");

  nds += 1;
}

*/

/* generate gnuplot script file for data visulalization */
/*
void gpscript()
{
  FILE* gps;

  gps = fopen("Tplot2d1.gp", "w");
  fprintf(gps, "set pm3d map\n");
  fprintf(gps, "set size square\n");
  fprintf(gps, "set xlabel 'x'\n");
  fprintf(gps, "set ylabel 'y'\n");
  fprintf(gps, "set xrange[%f:%f]\n",0.0,1.0);
  fprintf(gps, "set yrange[%f:%f]\n",0.0,1.0);
  fprintf(gps, "set cbrange[%f:%f]\n",-t_max*0.1,t_max*1.1);
  fprintf(gps, "set palette defined (%f 'blue', %f 'red', %f 'yellow')\n",0.0,t_max/2,t_max);
  fprintf(gps, "unset ztics\n");
  fprintf(gps, "set title 'T distribution'\n");
  fprintf(gps, "set terminal png\n");
  fprintf(gps, "set output \"T2d1initial.png\"\n");
  fprintf(gps, "splot './T2d.dat' index %d using 1:2:3\n", 0);
  fprintf(gps, "set output \"T2d1interim.png\"\n");
  fprintf(gps, "splot './T2d.dat' index %d using 1:2:3\n", nds/4);
  fprintf(gps, "set output \"T2d1final.png\"\n");
  fprintf(gps, "splot './T2d.dat' index %d using 1:2:3\n", nds);
  fprintf(gps, "set terminal x11 size 800,800\n");
  fprintf(gps, "do for [ind = 0:%d] {\n", nds);
  fprintf(gps, "splot './T2d.dat' index ind using 1:2:3\n");
  fprintf(gps, "pause 0.5\n");
  fprintf(gps, "}\n");

  fclose(gps);

  gps = fopen("Tplot2d2.gp", "w");
  fprintf(gps, "set size square\n");
  fprintf(gps, "set xlabel 'x'\n");
  fprintf(gps, "set ylabel 'y'\n");
  fprintf(gps, "set xrange[%f:%f]\n",0.0,1.0);
  fprintf(gps, "set yrange[%f:%f]\n",0.0,1.0);
  fprintf(gps, "set contour base\n");
  fprintf(gps, "set cntrparam levels 10\n");
  fprintf(gps, "set title 'T distribution'\n");
  fprintf(gps, "set terminal png\n");
  fprintf(gps, "set output \"T2d2initial.png\"\n");
  fprintf(gps, "splot './T2d.dat' index %d every 2 using 1:2:3\n", 0);
  fprintf(gps, "set output \"T2d2interim.png\"\n");
  fprintf(gps, "splot './T2d.dat' index %d every 2 using 1:2:3\n", nds/4);
  fprintf(gps, "set output \"T2d2final.png\"\n");
  fprintf(gps, "splot './T2d.dat' index %d every 2 using 1:2:3\n", nds);
  fprintf(gps, "set terminal x11 size 800,800\n");
  fprintf(gps, "splot './T2d.dat' index %d every 2 using 1:2:3\n", nds);

  fclose(gps);

  printf("To visualize data, run \"gnuplot -persist Tplot2d1.gp\" or \"gnuplot -persist Tplot2d2.gp\".\n");
}

*/
