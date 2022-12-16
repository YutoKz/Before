#include <stdio.h>

int main(void)
{
    FILE *fp, *fpxt, *fpet;
    fp = fopen("AllData.txt","w");
    fpxt = fopen("xt.txt","w");
    fpet = fopen("et.txt","w");

    int i, nsteps;
    double m, k, dt;
    double x, v, v_ast, f, e;
    


    /* シミュレーション設定 */
    nsteps = 5000;
    dt = 0.01;

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
        
        fprintf(fp, "%7.3f %7.3f %7.3f %7.7f\n" ,dt*(i+1) ,x , v, e);
        fprintf(fpxt, "%7.3f %7.3f\n" ,dt*i ,x);
        fprintf(fpet, "%7.3f %7.7f\n" ,dt*i, e);
    }

    fclose(fp);
    fclose(fpxt);
    fclose(fpet);
    return 0;
    
    
}