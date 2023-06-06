// 2jigen tikuji

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include "cntary.h"

#define N 100

const int ITRmax=100000;
const int INTV=1000;
int nds;
double dt, dr, kappa, alpha, beta, s, sdt, diff, t_max;
double **T, **T_new;

void output(FILE* fp, double** data, int isize, int jsize);
void gpscript();





int main(int argc, char **argv)
{
    int  i, j, tstep;
    FILE* fp;





+++++
    // Define parameters
    dr = 1.0/N;
    kappa = 1.0;
    dt = dr*dr*0.25/kappa;
    s  = 50.0;

    beta  = kappa*dt/dr/dr; // !write here!
    alpha = 1.0 - 4.0*beta; // !write here!
    sdt   = s*dt;

    // Allocation
    T = carray2d(N+1, N+1, 0, 0); // !write here!
    T_new = carray2d(N+1, N+1, 0, 0); // !write here!

    // Initialization
    // !write here!
    for(i=0; i<=N; i++)
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
    output(fp, T, N+1, N+1);

    // Time advancement
    tstep = 0;
    do
    {
        tstep++;




+++++++
        // solve Heat Equation
        // *new value T(t+Dt): T_new[][], old value T(t): T[][]
        // *boundary condition: T[0][0] = T_new[0][0] = T[N][N] = T_new[N][N] = 0.0
        // !write here!
        for(i=1; i<N; i++)
        {
            for(j=1; j<N; j++)
            {
                T_new[i][j] = alpha*T[i][j] + beta*(T[i+1][j] + T[i-1][j] + T[i][j+1] + T[i][j-1]) + sdt;
            }
        }
        





        // check a difference between T(t+Dt) & T(t)
        // !write here!
        diff = 0.0;
        for(i=1; i<N; i++)
        {
            for(j=1; j<N; j++)
            {
                diff += pow((T_new[i][j]-T[i][j]), 2);
            }
        }
        diff = sqrt(diff);

        // Store T(t+Dt) into T[]
        // !write here!
        for(i=1; i<N; i++)
        {
            for(j=1; j<N; j++)
            {
                T[i][j] = T_new[i][j];
            }
        }

        // Data output during iteration
        if( tstep%INTV == 1 ) output(fp, T, N+1, N+1);

    } while ( (diff > 1.0e-10) && (tstep < ITRmax) ) ;

    if ( tstep < ITRmax )
        printf("Converged at: tstep = %d\n", tstep );
    else
        printf("Reach ITRmax before converged: diff = %9.3e\n", diff );

    // Final data output
    output(fp, T, N+1, N+1);
    fclose(fp);
    gpscript();

    return 0;

}






/* write T[], calculate max value of T[] */
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

/* generate gnuplot script file for data visulalization */
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

