#include <stdio.h>
#include <math.h>

int main(void)
{
    FILE *fp;
    fp = fopen("runge_taiiki.txt","w");

    int i, j, nsteps;
    double m, k, dt;
    double x, v, f, e;
    double k_1v, k_2v, k_3v, k_4v;
    double k_1x, k_2x, k_3x, k_4x;

    double e_start, e_end;


    /* シミュレーション設定 */
    nsteps = 500;
    dt = 0.1;
    
    fprintf(fp, "NSTEPS, dt, error\n");

    for(j = 0; j < 7; j++)
    {
        m = 1.0;
        k = 1.0;

        x = 1.0;
        v = 0.0;

        for(i=0; i<nsteps; i++)
        {
            k_1v = (-1.0) * k * x * dt/ m;
            k_1x = v * dt;

            k_2v = (-1.0) * k * (x + k_1x / 2.0) * dt / m;
            k_2x = (v + k_1v / 2.0) * dt;

            k_3v = (-1.0) * k * (x + k_2x / 2.0) * dt / m;
            k_3x = (v + k_2v / 2.0) * dt;

            k_4v = (-1.0) * k * (x + k_3x) * dt / m;
            k_4x = (v + k_3v) * dt;


            v += (k_1v + 2.0 * k_2v + 2.0 * k_3v + k_4v) / 6.0;
            x += (k_1x + 2.0 * k_2x + 2.0 * k_3x + k_4x) / 6.0;


            e = 0.5 * (m*v*v + k*x*x);

            if(i == 0)
            {
                e_start = e;
            }
            if(i == nsteps-1)
            {
                e_end = e;
            }
        }

        fprintf(fp, "%11d, %11.8f, %7.7f\n" ,nsteps , dt, log10(fabs(e_end - e_start)));

        nsteps *= 10;
        dt *= 0.1;
    }

    fclose(fp);
    return 0;
    
    
}