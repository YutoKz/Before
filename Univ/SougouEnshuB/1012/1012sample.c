#include <stdio.h>

int main(void)
{
    FILE *fp, *fpxt, *fpet;
    fp = fopen("AllData.txt","w");
    fpxt = fopen("xt.txt","w");
    fpet = fopen("et.txt","w");

    int i, nsteps;
    double m, k, dt;
    double x, v, f, e;
    


    /* シミュレーション設定 */
    nsteps = 2000;
    dt = 0.01;

    m = 1.0;
    k = 1.0;

    x = 1.0;
    v = 1.0;
    



    for(i=0; i<nsteps; i++)
    {
        f = (-1) * k * x;
        x += v * dt;
        v += (f / m) * dt;
        
        e = 0.5 * (m*v*v + k*x*x);
        
        fprintf(fp, "%7.3f %7.3f %7.3f %7.3f\n" ,dt*(i+1) ,x , v, e);
        fprintf(fpxt, "%7.3f %7.3f\n" ,dt*i ,x);
        fprintf(fpet, "%7.3f %7.3f\n" ,dt*i, e);
    }

    fclose(fp);
    fclose(fpxt);
    fclose(fpet);
    return 0;
    
    
}