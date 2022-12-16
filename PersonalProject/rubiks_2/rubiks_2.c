#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define I_MAX 100000000
#define SIZE 100000000

/*
    白 -> 0
    緑 -> 1
    橙 -> 2
    青 -> 3
    赤 -> 4
    黄 -> 5
*/

// 現在の色
int color[6][4];
// 初期の色
int init_color[6][4];
// 現時点最適手順
int procedure[SIZE];
int procedure_tmp[SIZE];
// 現時点最適手順数
int num_of_procedures = -1;
int num_of_procedures_tmp = 0;

// 90deg 回転
void rotate(int axis)
{
    /*
        右ねじ正
        0 -> x軸回り
        1 -> y軸回り
        2 -> z軸回り
    */


    int i, j;
    int color_tmp_top[4], color_tmp_side[4][2];
    if(axis == 0)   // x軸回り
    {
        // 回転前の色
        for(i = 0; i < 4;i++)
        {
            color_tmp_top[i] = color[3][i];
        }
        color_tmp_side[0][0] = color[1][2];
        color_tmp_side[0][1] = color[1][3];
        color_tmp_side[1][0] = color[0][2];
        color_tmp_side[1][1] = color[0][3];
        color_tmp_side[2][0] = color[4][2];
        color_tmp_side[2][1] = color[4][3];
        color_tmp_side[3][0] = color[5][2];
        color_tmp_side[3][1] = color[5][3];

        // 回転
        for(i = 0; i < 4;i++)
        {
            color[3][i] = color_tmp_top[(i+1) % 4];
        }
        color[1][2] = color_tmp_side[3][0];
        color[1][3] = color_tmp_side[3][1];
        color[0][2] = color_tmp_side[0][0];
        color[0][3] = color_tmp_side[0][1];
        color[4][2] = color_tmp_side[1][0];
        color[4][3] = color_tmp_side[1][1];
        color[5][2] = color_tmp_side[2][0];
        color[5][3] = color_tmp_side[2][1];

        
    } 
    else if(axis == 1)  // y軸回り
    {
        // 回転前の色
        for(i = 0; i < 4;i++)
        {
            color_tmp_top[i] = color[5][i];
        }
        color_tmp_side[0][0] = color[1][1];
        color_tmp_side[0][1] = color[1][2];
        color_tmp_side[1][0] = color[3][2];
        color_tmp_side[1][1] = color[3][3];
        color_tmp_side[2][0] = color[4][3];
        color_tmp_side[2][1] = color[4][0];
        color_tmp_side[3][0] = color[2][0];
        color_tmp_side[3][1] = color[2][1];

        // 回転
        for(i = 0; i < 4;i++)
        {
            color[5][i] = color_tmp_top[(i+1) % 4];
        }
        color[1][1] = color_tmp_side[3][0];
        color[1][2] = color_tmp_side[3][1];
        color[3][2] = color_tmp_side[0][0];
        color[3][3] = color_tmp_side[0][1];
        color[4][3] = color_tmp_side[1][0];
        color[4][0] = color_tmp_side[1][1];
        color[2][0] = color_tmp_side[2][0];
        color[2][1] = color_tmp_side[2][1];
    }
    else    // z軸回り
    {
        // 回転前の色
        for(i = 0; i < 4;i++)
        {
            color_tmp_top[i] = color[1][i];
        }
        color_tmp_side[0][0] = color[2][1];
        color_tmp_side[0][1] = color[2][2];
        color_tmp_side[1][0] = color[0][1];
        color_tmp_side[1][1] = color[0][2];
        color_tmp_side[2][0] = color[3][1];
        color_tmp_side[2][1] = color[3][2];
        color_tmp_side[3][0] = color[5][3];
        color_tmp_side[3][1] = color[5][0];

        // 回転
        for(i = 0; i < 4;i++)
        {
            color[1][i] = color_tmp_top[(i+1) % 4];
        }
        color[2][1] = color_tmp_side[3][0];
        color[2][2] = color_tmp_side[3][1];
        color[0][1] = color_tmp_side[0][0];
        color[0][2] = color_tmp_side[0][1];
        color[3][1] = color_tmp_side[1][0];
        color[3][2] = color_tmp_side[1][1];
        color[5][3] = color_tmp_side[2][0];
        color[5][0] = color_tmp_side[2][1];
    }
}





// 完成したか判定
int judge()
{
    int i, j;
    for(i = 0; i < 6; i++)
    {
        int tmp = color[i][0];
        for(j = 1; j < 4; j++)
        {
            if(color[i][j] != tmp)
            {
                return 0;
            }
        }
    }
    return 1;
}

// 完成していた場合の処理
void complete()
{
    int i, j;
    // より最適な手順が見つかったかどうか判定
    if((num_of_procedures < 0) || (num_of_procedures_tmp < num_of_procedures))
    {
        num_of_procedures = num_of_procedures_tmp;
        for(i = 0; i < SIZE; i++)
        {
            procedure[i] = procedure_tmp[i];
        }
    }
    printf("current num_of_procedures = %d\n", num_of_procedures);

    // 次に向け元に戻す
    for(i = 0; i < 6; i++)
    {
        for(j = 0; j < 4; j++)
        {
            color[i][j] = init_color[i][j];
        }
    }
    num_of_procedures_tmp = 0;
    for(i = 0; i < SIZE; i++)
    {
        procedure_tmp[i] = -1;
    }
}




int main(int argc, char const *argv[])
{
    int i, j;
    int axis;
    srand((unsigned int)time(NULL));

    // 手順用配列初期化
    for(i = 0; i < SIZE; i++)
    {
        procedure[i] = -1;
        procedure_tmp[i] = -1;
    }

    // 初期の色をファイルから読み込み
    FILE *fp;
    fp = fopen("colors.txt", "r");
    for(i = 0; i < 6; i++)
    {
        for(j = 0; j < 4; j++)
        {
            fscanf(fp, "%d ", &color[i][j]);
        }
    }
    for(i = 0; i < 6; i++)
    {
        for(j = 0; j < 4; j++)
        {
            init_color[i][j] = color[i][j];
        }
    }






    // メインループ
    int step = 0;
    while(step < I_MAX)
    {
        if(judge() == 1)
        {
            complete();
            break;
        }

        axis = rand() % 3;
        rotate(axis);

        int k = 0;
        while(1)
        {
            if(procedure_tmp[k] < 0)
            {
                //printf("%d\n", k);
                procedure_tmp[k] = axis;
                num_of_procedures_tmp = k + 1;
                break;
            } 
            k++;
        }



        
        step++;
    }





    // 最終的な結果を表示
    FILE *fp1;
    fp1 = fopen("out.txt", "w");
    int tmp = procedure[0];
    //i = 0;
    // while(procedure[i] >= 0)
    for(i = 0; i < num_of_procedures; i++)
    {
        //if(procedure[i] == tmp)
        //{
            fprintf(fp1, "%d ", procedure[i]);
        //}
        //else
        //{
        //    printf("\n%d ");
        //}
        //tmp = procedure[i];
        //i++;
    }

    fclose(fp);
    fclose(fp1);
    
    return 0;
}
