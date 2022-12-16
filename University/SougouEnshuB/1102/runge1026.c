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
    double k_1v, k_2v, k_3v, k_4v;
    double k_1x, k_2x, k_3x, k_4x;


    /* シミュレーション設定 */
    nsteps = 50000;
    dt = 0.1;

    m = 1.0;
    k = 1.0;

    x = 1.0;
    v = 0.0;
    
    


    f = (-1) * k * x;  // f(0)を計算

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
        
        fprintf(fp, "%7.3f %7.3f %7.3f %7.7f\n" ,dt*(i+1) ,x , v, e);
        fprintf(fpxt, "%7.3f %7.3f\n" ,dt*i ,x);
        fprintf(fpet, "%7.3f %7.7f\n" ,dt*i, e);
    }

    fclose(fp);
    fclose(fpxt);
    fclose(fpet);
    return 0;
    
    
}