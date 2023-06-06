#include <stdio.h>
#include <math.h>

int main(void)
{
    FILE *fp;
    fp = fopen("verlet_taiiki.txt","w");

    int i, j, nsteps;
    double m, k, dt;
    double x, v, v_ast, f, e;

    double e_start, e_end;

    nsteps = 500;
    dt = 0.1;
    

    fprintf(fp, "NSTEPS, dt, error\n");

    for(j = 0; j < 7; j++)
    {
        /* シミュレーション設定 */
        m = 1.0;   
        k = 1.0;

        x = 1.0;
        v = 0.0;

        f = (-1) * k * x;  // f(0)を計算

        for(i=0; i<nsteps; i++)
        {
            v_ast = v + (f * dt) / (2 * m);
            x += v_ast * dt;
            f = (-1) * k * x;
            v = v_ast + (f * dt) / (2 * m);
        
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