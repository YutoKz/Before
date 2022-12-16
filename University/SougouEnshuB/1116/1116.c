// Code by Yuto Kizawa 2022/11/09
#include <stdio.h>
#include <stdlib.h>

#define NX 100     // 格子数 NX+1
#define KAPPA 0.01   // 熱拡散率                    ←ここを変更
#define L 1.0       // 計算領域 [0, L]
#define T_END 30   // 最終計算時刻 単位[sec]に注意     ←ここを変更

// 画像生成用のファイル書き込み関数
void write_for_image(double dx, double time, FILE* fp, double* theta)
{
  int i;

  for(i = 0; i <= NX; i++)
  {
    fprintf(fp, "%f %f %f\n", i*dx, time, theta[i]);
  }
  fprintf(fp, "\n");
}

// GIF生成用のファイル書き込み関数
void write_for_gif(double dx, double time, FILE* fp, double* theta)
{
  int i;

  for(i = 0; i <= NX; i++)
  {
    fprintf(fp, "%f %f\n", i*dx, theta[i]);
  }
  fprintf(fp, "\n\n");
}



int main(void)
{
  double dt = 0.00005;                // 時間間隔
  double dx = L / NX;                 // 格子間隔
  double *theta_current, *theta_next; // 現在時刻分布, 次ステップ分布
  int i;
  
  theta_current = (double*)malloc((NX + 1)*sizeof(double));
  theta_next    = (double*)malloc((NX + 1)*sizeof(double));

  FILE *fp0, *fp1;
  fp0 = fopen("temperature_for_image.dat", "w");
  fp1 = fopen("temperature_for_gif.dat", "w");

  // 初期化
  for(i = 0; i < NX; i++)
  {
    theta_current[i] = 0.0;
    theta_next[i] = 0.0;
  }
  theta_current[NX] = 1.0;
  theta_next[NX] = 1.0;


  double time = 0.0;

  // t = 0 での分布を記録
  write_for_image(dx, time, fp0, &theta_current[0]); 
  write_for_gif(dx, time, fp1, &theta_current[0]);

  // 計算用
  double ktx = KAPPA * dt / dx / dx;
  int dt_inv = 1.0 / dt;
  
  int step = 0;
  while(time <= T_END)
  {
    time += dt;
    step++;
    for(i = 1; i <= NX-1; i++)
    {
      theta_next[i] = theta_current[i] + ktx * (theta_current[i+1] - 2.0 * theta_current[i] + theta_current[i-1]);
    }

    for(i = 1; i <= NX-1; i++)
    {
      theta_current[i] = theta_next[i];
    }

    if(step % dt_inv == 0)
    {
      write_for_image(dx, time, fp0, &theta_current[0]);
      write_for_gif(dx, time, fp1, &theta_current[0]);
      printf("now t = %f\n", time);
    }
  }

  fclose(fp0);
  fclose(fp1);

  return 0;
}