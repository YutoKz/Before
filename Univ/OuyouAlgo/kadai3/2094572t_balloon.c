#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>


#define MAX_BALLOONS 40
#define MAX_T 50000

/**********************
 * board の情報
 */

typedef struct balloon {
	int time;
	int pos;
} balloon_t, * balloon_tp;
balloon_t balloons[MAX_BALLOONS];

void setupBalloons(FILE * in, int n) {
    int i=0;
    for(i=0; i< n; i++) {
    	int time, pos;
    	fscanf(in, "%d %d", &pos, &time);
    	balloons[i].time = time;
    	balloons[i].pos = pos;
    }
}

typedef struct result {
	int isOK;
	int num;
} result_t, *result_tp;

result_t solve(int n) {
    int i, j, k;
    int distance[n+1][4];
    int current_time = 0, current_pos = 0;
    int next_time, next_pos;

    // 初期化
    for(i = 0; i < n+1; i++)
    {
        for(j = 0; j < 4; j++)
        {
            distance[i][j] = -1;
        }
    }
    distance[0][0] = 0;

    for(i = 0; i < n; i++)
    {
        next_time = balloons[i].time;
        next_pos = balloons[i].pos;

        // i+1個回収時点 1個持ち
        // 最小距離比較用配列
        int distance_tmp[4];
        for(j = 0; j < 4; j++)
        {
            distance_tmp[j] = -1;
        }
        // i個回収時点で0個持ちで直接ルート
        if((distance[i][0] >= 0) && ((next_time - current_time) >= abs(next_pos - current_pos)))
        {
            distance_tmp[0] = distance[i][0] + abs(next_pos - current_pos);
        }
        // i個回収時点でj個持ちで一旦家寄るルート
        for(j = 1; j <= 3; j++)
        {
            if((distance[i][j] >= 0) && (next_time - current_time - current_pos * (j+1) >= next_pos))
            {
                distance_tmp[j] = distance[i][j] + current_pos + next_pos;
            }
        }
        // distance_tmp[4]の正かつ最小を求める
        int distance_min = -1;
        for(j = 0; j < 4; j++)
        {
            if(distance_min < 0)
            {
                distance_min = distance_tmp[j];
            }
            else if(distance_min > distance_tmp[j] && distance_tmp[j] >= 0)
            {
                distance_min = distance_tmp[j];
            }
        }
        distance[i+1][1] = distance_min;
        // 以上、i+1個回収時点 1個持ち


        // i+1個回収時点 2個持ち
        // i個回収時点で1個持ちで直接ルート
        if((distance[i][1] >= 0) && ((next_time - current_time) >= abs(next_pos - current_pos) * 2))
        {
            distance[i+1][2] = distance[i][1] + abs(next_pos - current_pos);
        }


        // i+1個回収時点 3個持ち
        // i個回収時点で2個持ちで直接ルート
        if((distance[i][2] >= 0) && ((next_time - current_time) >= abs(next_pos - current_pos) * 3))
        {
            distance[i+1][3] = distance[i][2] + abs(next_pos - current_pos);
        }
    

        // 回収できるか確認
        if((distance[i+1][0] < 0) && (distance[i+1][1] < 0) && (distance[i+1][2] < 0) && (distance[i+1][3] < 0))
        {
            result_t result = {false, i+1};
            return result;
        }


        // 次ループの準備
        current_time = balloons[i].time;
        current_pos = balloons[i].pos;
    }

    // n個全て回収した時点での最小距離を求める
    int min;
    for(i = 0; i < 4; i++)
    {
        if(distance[n][i] >= 0)
        {
            min = distance[n][i];
            for(j = i+1; j < 4; j++)
            {
                if((min > distance[n][j]) && (distance[n][j] >= 0))
                {
                    min = distance[n][j];
                }
            }
            break;
        }
    }

    // 求めた最小距離に、最後に家に帰る分の距離を足して出力
	result_t result = { true, min + current_pos };
	return result;
}

/*******
 * こちらで用意したmain 関数。
 * 問題準備してから、solve() をよび、正解比較もおこなう。
 */
int main(int argc, char* argv[]) {
    struct {
        char * in, * ans;
    } filepairs[] = { {"sample.in", "sample.ans"},
                      { "B.in", "B.ans"}};

    int i;
    for(i=0; i<2; i++) {
        char* inFile = filepairs[i].in;
        char* ansFile = filepairs[i].ans;
        FILE * in = fopen(inFile, "r");
        FILE * ansIn = fopen(ansFile, "r");
        if(in==NULL) {
            printf("Can't open file: %s.\n", inFile);
            return 0;
        }
        if(ansIn==NULL) {
            printf("Can't open file: %s.\n", ansFile);
            return 0;
        }
        int failCount = 0;
        int totalCount = 0;
        printf("Processing input: %s\n", inFile);
        while(1) {
            int n, ans; char isOK[10];
            result_t result;
            fscanf(in, "%d", &n);
            if(n==0) break;
            setupBalloons(in, n);
            result = solve(n);
            fscanf(ansIn, "%s %d", isOK, &ans);
            if((strcmp("OK", isOK) ==0) != result.isOK ||
            		ans != result.num) {
                failCount++;
                printf("You failed problem No. %d (result: %s %10d, ans: %s, %d)\n",
                		totalCount, result.isOK? "OK": "NG", result.num, isOK, ans);
            }
            totalCount++;
        }
        if(failCount==0) {
            printf("Congratulation! You passed all problems (%d) in %s!\n", totalCount, inFile);
        } else {
            printf("Im sorry you missed %d/%d in %s!\n", failCount, totalCount, inFile);
            return 0;
        }
    }
    return 0;
}

