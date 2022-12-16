#include<stdio.h>
#include<stdlib.h>
#include<math.h>

// 考える状態数
#define n  5
// 数値実験のステップ数・状態の遷移回数
#define LOOP 1000000


int main(int argc, char *argv[]){

    // 乱数の種．ここを(他の自然数に)変えると結果も変化する
    int seed = 0;
    // 遷移行列．A[i][j] 状態jにあるものが次のタイミングで状態iに移る確率
    // 状態1の次は，それぞれ確率0.333で2,3,4のどれかに移る
    double A[n][n]={
        {0,     0,     0,     0,   1},
        {1.0/4, 0,     0,     0,   0},
        {1.0/4, 1.0/3, 0,     0,   0},
        {1.0/4, 1.0/3, 1.0/2, 0,   0},
        {1.0/4, 1.0/3, 1.0/2, 1,   0}
    };
    // 遷移行列の別表現(計算するときに便利)
    double thr[n][n];
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            thr[i][j]=0;
        }
    }
    for(int j=0;j<n;j++) {
        thr[0][j]=A[0][j];
    }
    for(int i=1;i<n;i++) {
        for(int j=0;j<n;j++) {
            thr[i][j] = thr[i-1][j] + A[i][j];
        }
    }
/*
A=
0.00 0.00 0.00 0.00 1.00
0.25 0.00 0.00 0.00 0.00
0.25 0.33 0.00 0.00 0.00
0.25 0.33 0.50 0.00 0.00
0.25 0.33 0.50 1.00 0.00

thr=
0.00 0.00 0.00 0.00 1.00
0.25 0.00 0.00 0.00 1.00
0.50 0.33 0.00 0.00 1.00
0.75 0.67 0.50 0.00 1.00
1.00 1.00 1.00 1.00 1.00
*/

    // 使う行列の確認
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) {
            printf("%.2f ",A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) {
            printf("%.2f ",thr[i][j]);
        }
        printf("\n");
    }
    printf("\n");





    // 乱数の初期化
    srand(seed);
    // 現在どの場所にいるかの初期化
    // 0からn-1の整数の一様乱数
    int index = rand()%n;
    int counter[n];  // 今までに何回状態 i にいたか
    for(int i=0;i<n;i++) {
        counter[i]=0;
    }
    int loop=0;
    double r;
    while( loop < LOOP ){
        // ほぼ，0〜1の実数一様乱数
        r =  (double)rand()/RAND_MAX;

        // このループn回で一回の遷移
        for(int i=0;i<n;i++){
            // Aで表される確率に応じた状態遷移．
            // thrを用いて計算の簡略化
            if( r < thr[i][index] ){
                index = i;  // ここに移動する
                counter[i]++;
                break;
            }
        }
        loop++;
    }
    // 1ノルムを1に規格化する準備
    double norm = 0.0;
    for(int i=0;i<n;i++) {
        norm = norm + counter[i];
    }
    // 表示．どれくらいの割合で状態 i にいたか
    for(int i=0;i<n;i++) {
        printf("%.3f  ", counter[i]/norm);
    }
    printf("\n");
    return 0;
}
