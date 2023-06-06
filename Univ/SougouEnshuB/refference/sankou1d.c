// Code by Yuto Kizawa 2022/11/09

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>

#define N 100

const int ITRmax=100000;
const int INTV=1000;
int nds;
//double dt, dx, kappa, alpha, beta, s, sdt, diff_local, diff_sum, t_max;
//double *T, *T_new;
double T_global[N+1];

void output(FILE* fp, double* data, int size);
void gpscript();





int main(int argc, char **argv)
{
    int nprocs, myrank;
    MPI_Status  status;
    int left, right;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    
    
    
    int start, end;
    
    int  i, tstep;
    FILE* fp;

    // Define parameters
    dx = 1.0/N;
    kappa = 1.0;
    dt = dx*dx*0.5/kappa;
    s  = 80.0;

    beta  = kappa*dt/dx/dx;
    alpha = 1.0 - 2.0*beta;
    sdt   = s*dt;

    
    
    // Allocation
    // start = (N/nprocs)*myrank;
    // end   = (N/nprocs)*(myrank + 1) - 1;
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
    
    
    T = carray1d(end - start + 3, start - 1);
    T_new = carray1d(end - start + 3, start - 1);
    
    // Initialization
    for(i=start-1; i<=end+1; i++){
        T    [i] = 0.0;
        T_new[i] = 0.0;
    }
    
    // 1st-time data output
    nds = -1;
    t_max = 0.0;
    fp = fopen("T1d.dat", "w");
    


    MPI_Gatherv(&T[start], nsdom, MPI_DOUBLE, &T_global[0], counts, disps, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(myrank == 0)
        output(fp, &T_global[0], N+1);
    
    
    
    // Time advancement
    tstep = 0;
    do
    {
        tstep++;
        
        // solve Heat Equation
        // *new value T(t+Dt): T_new[], old value T(t): T[]
        // *boundary condition: T[0] = T_new[0] = T[N] = T_new[N] = 0.0
        left = myrank - 1;
        if(myrank == 0)
            left = MPI_PROC_NULL;
        
        right = myrank + 1;
        if(myrank == nprocs - 1)
            right = MPI_PROC_NULL;
        
        MPI_Sendrecv(&T[end], 1, MPI_DOUBLE, right, (myrank+1)*100, &T[start-1], 1, MPI_DOUBLE, left, (left+1)*100, MPI_COMM_WORLD, &status);
        MPI_Sendrecv(&T[start], 1, MPI_DOUBLE, left, (myrank+1)*(-100), &T[end+1], 1, MPI_DOUBLE, right, (right+1)*(-100), MPI_COMM_WORLD, &status);
        
        
        for(i=start; i<=end; i++){
            T_new[i] = alpha*T[i] + beta*(T[i+1]+T[i-1]) + sdt; // !write here!
        } // beta=0.5 alpha=0

        // check a difference between T(t+Dt) & T(t)
        diff_local = 0.0;
        for(i=start; i<=end; i++){
            diff_local += pow((T_new[i]-T[i]), 2); // !write here!
        }
        
        MPI_Allreduce(&diff_local, &diff_sum, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        
        diff_sum = sqrt(diff_sum); // !write here!

        // Store T(t+Dt) into T[]
        for(i=start; i<=end; i++){
            T[i] = T_new[i];
        }

        // Data output during iteration
        if( tstep%INTV == 1 )
        {
            MPI_Gatherv(&T[start], nsdom, MPI_DOUBLE, &T_global[0], counts, disps, MPI_DOUBLE, 0, MPI_COMM_WORLD);
            if(myrank == 0)
                output(fp, &T_global[0], N+1);
        }
        
        } while ( (diff_sum > 1.0e-10) && (tstep < ITRmax) ) ;

    if ( tstep < ITRmax )
        printf("Converged at: tstep = %d\n", tstep );
    else
        printf("Reach ITRmax before converged: diff_sum = %9.3e\n", diff_sum);

    // Final data output
    MPI_Gatherv(&T[start], nsdom, MPI_DOUBLE, &T_global[0], counts, disps, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(myrank == 0)
        output(fp, &T_global[0], N+1);
    fclose(fp);
    gpscript(); // generate gnuplot script file
    
    
    
    
    
    MPI_Finalize();
    return 0;
    
}





/* write T[], calculate max value of T[] */
void output(FILE* fp, double* data, int size)
{
  int i;

  for(i=0; i<size; i++){
    t_max = (t_max > data[i]) ? t_max : data[i];
    fprintf(fp, "%f  %f\n", i*dx, data[i]);
  }
  fprintf(fp, "\n\n");

  nds += 1;
}

/* generate gnuplot script file for data visulalization */
void gpscript()
{
  FILE* gps;

  gps = fopen("Tplot1d.gp", "w");
  fprintf(gps, "set xlabel 'x'\n");
  fprintf(gps, "set xrange[%f:%f]\n",0.0,1.0);
  fprintf(gps, "set yrange[%f:%f]\n",-t_max*0.1,t_max*1.1);
  fprintf(gps, "set title 'T distribution'\n");
  fprintf(gps, "set terminal png\n");
  fprintf(gps, "set output \"T1dinitial.png\"\n");
  fprintf(gps, "plot './T1d.dat' index %d using 1:2 w l\n", 0);
  fprintf(gps, "set output \"T1dinterim.png\"\n");
  fprintf(gps, "plot './T1d.dat' index %d using 1:2 w l\n", nds/16);
  fprintf(gps, "set output \"T1dfinal.png\"\n");
  fprintf(gps, "plot './T1d.dat' index %d using 1:2 w l\n", nds);
  fprintf(gps, "set terminal x11 size 1000,750\n");
  fprintf(gps, "do for [ind = 0:%d] {\n", nds);
  fprintf(gps, "plot './T1d.dat' index ind using 1:2 w l\n");
  fprintf(gps, "pause 0.5\n");
  fprintf(gps, "}\n");

  fclose(gps);

  printf("To visualize data, run \"gnuplot -persist Tplot1d.gp\".\n");
}

