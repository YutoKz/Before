// Code by Yuto Kizawa 2022/11/09
#include <stdio.h>
#include <stdlib.h>

#define N 100        // 格子数 N+1 xy共通
#define KAPPA 0.001   // 熱拡散率
#define C_x 0.05       // 移流項の係数
#define C_y -0.08
#define L 1.0       // 計算領域 [0, L]
#define T_END 15   // 最終計算時刻 単位[sec]に注意

/*
    KAPPA dt (2/dr^2) <= 1/2
*/

// GIF生成用のファイル書き込み関数
void write_for_gif(double dr, double time, FILE* fp, double** theta)
{
    int i, j;

    for(i = 0; i <= N; i++)
    {
        for(j = 0; j <= N; j++)
        {
            fprintf(fp, "%f %f %f\n", i*dr, j*dr, theta[i][j]);
        }
    }
    fprintf(fp, "\n\n");
}



int main(void)
{
    double dt = 0.0005;                // 時間間隔
    double dr = L / N;                  // 格子間隔 xy共通
    double **theta_current, **theta_next; // 現在時刻分布, 次ステップ分布
    int i, j;
  

    // データ用配列
    theta_current = malloc((N+1) * sizeof(double*));
    for(i = 0; i <= N; i++)
        theta_current[i] = malloc((N+1) * sizeof(double));
    theta_next = malloc((N+1) * sizeof(double*));
    for(i = 0; i <= N; i++)
        theta_next[i] = malloc((N+1) * sizeof(double));
  

    // 書き込み用ファイル準備
    FILE *fp1;
    fp1 = fopen("2d_temperature_for_gif_ad.dat", "w");


    // 初期条件
    for(i = 1; i < N; i++)
    {
        for(j = 1; j < N; j++)
        {
            theta_current[i][j] = 1.0;
            theta_next[i][j] = 1.0;
        }
    }


    // 境界条件 熱源あり
    for(i = 0; i <= N; i++)
    {
        theta_current[i][N] = 1.0;
        theta_next[i][N] = 1.0;

        theta_current[i][0] = 1.0;
        theta_next[i][0] = 1.0;

        theta_current[N][i] = 1.0;
        theta_next[N][i] = 1.0;

        theta_current[0][i] = 1.0;
        theta_next[0][i] = 1.0;
    }

    // 熱源
    for(i = N * 4 / 5; i < N; i++)
    {
        theta_current[0][i] = 2.0;
        theta_next[0][i] = 2.0;
    }


    double time = 0.0;


    // t = 0 での分布を記録
    write_for_gif(dr, time, fp1, theta_current);


    // 計算用
    double ktr = KAPPA * dt / dr / dr;
    double ctr_x = C_x * dt / dr;
    double ctr_y = C_y * dt / dr;
    int steps_per_sec = 1.0 / dt;      // 1s ごとのステップ数
  
    // メインループ
    int step = 0;
    while(time <= T_END)
    {
        time += dt;
        step++;
        for(i = 1; i <= N-1; i++)
        {
            if(i <= N * 3 / 5)
            {
                for(j = N * 3 / 5; j <= N-1; j++)
                {
                    // 更新式
                    theta_next[i][j] = theta_current[i][j] 
                                    + ktr * (theta_current[i+1][j] + theta_current[i-1][j] - 4.0 * theta_current[i][j] + theta_current[i][j+1] + theta_current[i][j-1]) 
                                    - ctr_x * (theta_current[i][j] - theta_current[i-1][j]);
                }
            }
            else
            {
                for(j = 1; j <= N-1; j++)
                {
                    // 更新式
                    theta_next[i][j] = theta_current[i][j] 
                                    + ktr * (theta_current[i+1][j] + theta_current[i-1][j] - 4.0 * theta_current[i][j] + theta_current[i][j+1] + theta_current[i][j-1]) 
                                    - ctr_x * (theta_current[i][j] - theta_current[i-1][j])
                                    - ctr_y * (theta_current[i][j] - theta_current[i][j-1]);
                }
            }
        }

        for(i = 1; i <= N-1; i++)
        {
            for(j = 1; j <= N-1; j++)
            {
                theta_current[i][j] = theta_next[i][j];
            }
        }

        if(step % (steps_per_sec/5) == 0)
        {
            write_for_gif(dr, time, fp1, theta_current);
            printf("Now: t = %f\n", time);
        }
    }

    fclose(fp1);

    return 0;
}