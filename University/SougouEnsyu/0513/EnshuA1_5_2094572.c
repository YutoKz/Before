#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define ITERATION 20000 /* 反復回数 */
#define ETA 0.00005 /* 学習係数 */
#define MAX_N_DATA 10000 /* 配列の最大データ数 */

float calc_y(float a, float b, float c, float d, float x)
{
    /* パラメータa, b, c, dを使用して yの値を計算 */
    float y_calc;

    y_calc = a * pow(x, 3) + b * pow(x, 2) + c * x + d;

    return y_calc;
}

float get_loss(int n_data, float* x, float* y, float a, float b,float c, float d)
{
    int i;
    float y_pred;
    float loss_total = 0;
    /* 必要な変数は追記のこと */

    for (i = 0; i < n_data; i++) {
        /* パラメータを用いて計算したyの値を取得 */
        y_pred = calc_y(a, b, c ,d,x[i]);

        /* 以下で2乗誤差を求め，その総和を求めるためのコードを実装 */
        /* 求めた総和はloss_totalに記憶 */

        loss_total += pow((y_pred - y[i]), 2);
    }
    return loss_total / n_data;
}

float get_grad_a(int n_data, float* x, float* y, float a, float b,float c, float d)
{
    int i;
    float grad_total = 0;
    /* 必要な変数は追記のこと */

    for (i = 0; i < n_data; i++) {
        /* 以下実装が必要 */
        grad_total += 2 * a * pow(x[i], 6) + 2 * pow(x[i], 3) * (b * pow(x[i], 2) + c * x[i] + d - y[i]);
    }
    return grad_total / n_data;
}

float get_grad_b(int n_data, float* x, float* y, float a, float b, float c, float d)
{
    /* get_grad_a関数を参考に，必要な変数を定義し，実装すること */
    int i;
    float grad_total = 0;
    /* 必要な変数は追記のこと */

    for (i = 0; i < n_data; i++) {
        /* 以下実装が必要 */
        grad_total += 2 * b * pow(x[i], 4) + 2 * pow(x[i], 2) * (a * pow(x[i], 3) + c * x[i] + d - y[i]);
    }
    return grad_total / n_data;
}

float get_grad_c(int n_data, float* x, float* y, float a, float b,float c, float d)
{
    /* get_grad_a関数を参考に，必要な変数を定義し，実装すること */
    int i;
    float grad_total = 0;
    /* 必要な変数は追記のこと */

    for (i = 0; i < n_data; i++) {
        /* 以下実装が必要 */
        grad_total += 2 * c * pow(x[i], 2) + 2 * x[i] * (a * pow(x[i], 3) + b * pow(x[i], 2) + d - y[i]);
    }
    return grad_total / n_data;
}

float get_grad_d(int n_data, float* x, float* y, float a, float b,float c, float d)
{
    /* get_grad_a関数を参考に，必要な変数を定義し，実装すること */
    int i;
    float grad_total = 0;
    /* 必要な変数は追記のこと */

    for (i = 0; i < n_data; i++) {
        /* 以下実装が必要 */
        grad_total += 2 * d + 2 * (a * pow(x[i], 3) + b * pow(x[i], 2) + c * x[i] - y[i]);
    }
    return grad_total / n_data;
}


int load_data(float* x, float* y)
{
    /* "data5_kadai_2022.dat"内のデータを取得，基本的にはファイル名以外は変更しなくても使用可能 */
    int n_data = 0;
    int n_data_loaded;
    FILE *fp;
    if ((fp = fopen("kadai5_data_2022.dat", "r")) == NULL) {
        printf("File data_kadai cannot be opened.\n");
    }
    
    while (1) {
        /* x,yの情報を取得し，配列に格納 */
        n_data_loaded = fscanf(fp, "%f %f\n", &x[n_data], &y[n_data]);

        if (n_data_loaded < 2) {
            break;
        }
        /* 取得したデータを標準出力に */
        printf("%d: %lf %lf\n",n_data,x[n_data],y[n_data]);
        n_data++;
    }
    /* データを返す */
    fclose(fp);
    return n_data;
}

void get_data(float a, float b,float c, float d, int num)
{
    /* 推定したパラメータを用いて求まった，x,yの値をファイル出力するための関数 */
    /* numは出力するデータ数 */
    FILE *fp;
    int i;
    double x, y;

    if ((fp = fopen("get_data_2022.txt", "w")) == NULL) {
        printf("File data_kadai cannot be opened.\n");
    }

    for(i=0;i<num;i++){
        x = rand()%6 + rand()%50*0.1-2.5; /* xの範囲が-2.5~7.4の間でランダムの値で取得 */
        y = a * pow(x,3.0) + b * pow(x, 2.0) + c * x + d;
        fprintf(fp,"%lf %lf\n",x,y);
    }
    fclose(fp);

}

int main(void)
{
    float x[MAX_N_DATA],y[MAX_N_DATA]; /* データ保持用配列 */
    float a,b,c,d; /* 推定する係数 */
    float loss, grad_a,grad_b,grad_c,grad_d,grad_e;
    int n_data,t;
    FILE *ofp;
 
    n_data = load_data(x,y);
    printf("%d pairs are loaded.\n", n_data);

    /* パラメータの初期化 */    
    a = 1.00000;
    b = 1.00000;
    c = 1.00000;
    d = 1.00000;
 

    /* 誤差の記録 */
    if ((ofp = fopen("results.txt", "wb")) == NULL) {
        printf("File cannot be opened.\n");
        exit(EXIT_FAILURE);
    }

    /* 主体 */
    for (t = 0; t < ITERATION; t++) {
        loss = get_loss(n_data, x, y, a, b,c,d);
        grad_a = get_grad_a(n_data, x, y, a, b,c,d);
        grad_b = get_grad_b(n_data, x, y, a, b,c,d);
        grad_c = get_grad_c(n_data, x, y, a, b,c,d);
        grad_d = get_grad_d(n_data, x, y, a, b,c,d);
        /* grad_b, c,dも同様に取得できるようにする */
        
        printf("%d a=%1.5f, b=%1.5f, c=%1.5f, d=%1.5f, L=%1.5f\n", t, a, b, c,d,loss);
        fprintf(ofp, "%d\t%1.5f\t%1.5f\t%1.5f\t%1.5f\t%1.5f\n", t, a, b, c, d, loss);

        /* 勾配の更新 */
        /* 学習係数ETAを用いる必要がある */ 
        a -= ETA * grad_a;
        b -= ETA * grad_b;
        c -= ETA * grad_c;
        d -= ETA * grad_d;
    }
    fclose(ofp);

    get_data(a,b,c,d,100); /* 推定したパラメータを用いてデータを取得, 現在の場合100個のデータを取得 */

    printf("\n\ny = %1.5f x^3 + %1.5f x^2 + %1.5f x + %1.5f\n", a, b, c, d);

    return 0;
}
