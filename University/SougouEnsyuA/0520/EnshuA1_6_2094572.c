#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N_DATA 1000 /* Maximum Number of Data */
#define ITERATION 1000 /* Number of Iteration 反復回数 */
#define ETA 0.005 /* Learning coefficeint 学習係数 */

/* 各層のノード数の設定 */
#define INPUT_DIM 1 /* 入力層 */
#define HIDDEN_DIM 1 /* 隠れ層 */
#define OUTPUT_DIM 1 /* 出力層 */


int load_data(float x[][INPUT_DIM], float y[][OUTPUT_DIM]) {
    // Data is loaded from "data.dat".
    int n_data_loaded, n_data = 0; // How many data is loaded
    int i;
    FILE *ifp;

    if ((ifp = fopen("kadai6_data_500.dat", "r")) == NULL) { /* データファイル名 */
        printf("File cannot be opened.\n");
    }

    while (1) {
    // A pair of measures is loaded.
        n_data_loaded = 0;

        for (i = 0; i < INPUT_DIM; i++) {
                n_data_loaded += fscanf(ifp, "%f", &x[n_data][i]);

        }
        for (i = 0; i < OUTPUT_DIM; i++) {
            n_data_loaded += fscanf(ifp, "%f", &y[n_data][i]);
        }
        // When no pair is loaded, break.
        if (n_data_loaded < INPUT_DIM + OUTPUT_DIM) {
            break;
        }
        // The number of loaded pairs is incremented.
        n_data++;
    }

    // Return the number of data
    return n_data;
}




/* 活性化関数 */
float act_func_1st(float a){
    return tanh(a);
}

float act_func_grad_1st(float a){
    return 1.0 / pow(cosh(a), 2);
}

float act_func_2nd(float a){
    return /*1.0 / (1 + exp((-1) * a))*/ a;
}

float act_func_grad_2nd(float a){
    return /*1.0 / (exp(a) + 2 + exp((-1) * a))*/ 1.0;
}








int main(void) {
    float x[MAX_N_DATA][INPUT_DIM];        /* 入力データ用 */
    float y_tilde[MAX_N_DATA][OUTPUT_DIM]; /* true output 観測データ */
    int n_data;
    int i,j,n,t;
    FILE *ofp,*ofp2;

    // parameter
    float w0[HIDDEN_DIM][INPUT_DIM];
    float b0[HIDDEN_DIM];
    float w1[OUTPUT_DIM][HIDDEN_DIM];
    float b1[OUTPUT_DIM];

    // loss
    float loss_total;
    float *loss;

    // variables
    float a0[MAX_N_DATA][HIDDEN_DIM];
    float h[MAX_N_DATA][HIDDEN_DIM];
    float a1[MAX_N_DATA][OUTPUT_DIM];
    float y[MAX_N_DATA][OUTPUT_DIM];

    // gradients
    float dw0[HIDDEN_DIM][INPUT_DIM];
    float db0[HIDDEN_DIM];
    float dw1[OUTPUT_DIM][HIDDEN_DIM];
    float db1[OUTPUT_DIM];

    // for 2nd layer
    float dLdy[MAX_N_DATA][OUTPUT_DIM];
    float dyda1[MAX_N_DATA][OUTPUT_DIM];
    float dLda1[MAX_N_DATA][OUTPUT_DIM];
    float da1dw1[MAX_N_DATA][HIDDEN_DIM];
    // for 1st layer
    float dLdh[MAX_N_DATA][HIDDEN_DIM];
    float dhda0[MAX_N_DATA][HIDDEN_DIM];
    float dLda0[MAX_N_DATA][HIDDEN_DIM];
    float da0dw0[MAX_N_DATA][INPUT_DIM];









    // data loading
    n_data = load_data(x, y_tilde);
    printf("%d pairs are loaded.\n", n_data);

    loss = (float *)malloc(sizeof(float)*n_data);

    // initialize parameter
    srand(time(NULL));
    for (j = 0; j < HIDDEN_DIM; j++) {
        for (i = 0; i < INPUT_DIM; i++) {
            w0[j][i] = (float)rand() / RAND_MAX - 0.5;
        }
        b0[j] = (float)rand() / RAND_MAX - 0.5;
    }
    for (j = 0; j < OUTPUT_DIM; j++) {
        for (i = 0; i < HIDDEN_DIM; i++) {
            w1[j][i] = (float)rand() / RAND_MAX - 0.5;
        }
        b1[j] = (float)rand() / RAND_MAX - 0.5;
    }

    // output of loss function
    if ((ofp = fopen("result1.txt", "w")) == NULL) {
        printf("File cannot be opened.\n");
        exit(0);
    }
















    // main loop
    for (t = 0; t < ITERATION; t++) {
        loss_total = 0.0;

        // forward path
        for (n = 0; n < n_data; n++) {
            loss[n] = 0;




            // 1st layer: input -> hidden
            for (j = 0; j < HIDDEN_DIM; j++) {
                a0[n][j] = 0;
                for (i = 0; i < INPUT_DIM; i++) {
                    a0[n][j] += w0[j][i] * x[n][i];
                }
                a0[n][j] += b0[j];
                h[n][j] = act_func_1st(a0[n][j]); /* 入力→隠れ層の活性化関数 */
            }





            // 2nd layer: hidden -> output
            for (j = 0; j < OUTPUT_DIM; j++) {
                a1[n][j] = 0;
                for (i = 0; i < HIDDEN_DIM; i++) {
                    a1[n][j] += w1[j][i] * h[n][i];
                }
                a1[n][j] += b1[j];
                y[n][j]=act_func_2nd(a1[n][j]); /* 隠れ層→出力層の活性化関数 */
                loss[n] += powf(y_tilde[n][j] - y[n][j], 2); /* 誤差の2乗 */
            }

            loss_total += loss[n] / n_data;
        }



        // output the prediction
        if (t == ITERATION - 1) {

            if ((ofp2 = fopen("prediction.dat", "w")) == NULL) {
                printf("File cannot be opened.\n");
                exit(0);
            }

            for (n = 0; n < n_data; n++) {
                for (i = 0; i < INPUT_DIM; i++) {
                    fprintf(ofp2, "%f\t", x[n][i]);
                }

                for (j = 0; j < OUTPUT_DIM; j++) {
                    fprintf(ofp2, "%f\t", y[n][j]);
                }

                fprintf(ofp2, "\n");
            }
            fclose(ofp2);
        }





        // reset gradient
        for (j = 0; j < HIDDEN_DIM; j++) {
            for (i = 0; i < INPUT_DIM; i++) {
                dw0[j][i] = 0.0;
            }
            db0[j] = 0.0;
        }

        for (j = 0; j < OUTPUT_DIM; j++) {
            for (i = 0; i < HIDDEN_DIM; i++) {
                dw1[j][i] = 0.0;
            }
            db1[j] = 0.0;
        }






        /* backward path 誤差伝播 */
        for (n = 0; n < n_data; n++) {
            // error for 2nd layer
            for (j = 0; j < OUTPUT_DIM; j++) {
                dLdy[n][j] = 2 * (y[n][j] - y_tilde[n][j]);
            }

            // for 2nd layer
            for (j = 0; j < OUTPUT_DIM; j++) {
                dyda1[n][j] = act_func_grad_2nd(a1[n][j]); /* 出力層→隠れ層 */
                dLda1[n][j] = dLdy[n][j] * dyda1[n][j];
            }

            for (i = 0; i < HIDDEN_DIM; i++) {
                da1dw1[n][i] = h[n][i];
            }
            for (j = 0; j < OUTPUT_DIM; j++) {
                for (i = 0; i < HIDDEN_DIM; i++) {
                    dw1[j][i] += dLda1[n][j] * da1dw1[n][i] / n_data;
                }
                db1[j] += dLda1[n][j] / n_data;
            }

            // back propagation = error for 1st layer
            for (i = 0; i < HIDDEN_DIM; i++) {
                dLdh[n][i] = 0.0;
                for (j = 0; j < OUTPUT_DIM; j++) {
                    dLdh[n][i] += w1[j][i] * dLda1[n][j];
                }
            }

            // for 1st layer
            for (j = 0; j < HIDDEN_DIM; j++) {
                dhda0[n][j] = act_func_grad_1st(a0[n][j]); /* 隠れ層→入力層 */
                dLda0[n][j] = dLdh[n][j] * dhda0[n][j];
            }

            for (i = 0; i < INPUT_DIM; i++) {
                da0dw0[n][i] = x[n][i];
            }

            for (j = 0; j < HIDDEN_DIM; j++) {
                for (i = 0; i < INPUT_DIM; i++) {
                    dw0[j][i] += dLda0[n][j] * da0dw0[n][i] / n_data;
                }
                db0[j] += dLda0[n][j] / n_data;
            }
        }







        // update
        for (j = 0; j < OUTPUT_DIM; j++) {
            for (i = 0; i < HIDDEN_DIM; i++) {
                w1[j][i] = w1[j][i] - ETA * dw1[j][i];
            }
            b1[j] = b1[j] - ETA * db1[j];
        }

        for (j = 0; j < HIDDEN_DIM; j++) {
            for (i = 0; i < INPUT_DIM; i++) {
                w0[j][i] = w0[j][i] - ETA * dw0[j][i];
            }
            b0[j] = b0[j] - ETA * db0[j];
        }



        /* パラメータの表示 */
        printf("%d L=%1.5f", t, loss_total);



        // 1st layer
        printf(" w0=(");
        for (j = 0; j < HIDDEN_DIM; j++) {
            for (i = 0; i < INPUT_DIM; i++) {
                printf("%f", w0[j][i]);
                if (i < INPUT_DIM - 1) {
                    printf(",");
                }
            }

            if (j < HIDDEN_DIM - 1) {
                printf(";");
            }
        }
        printf(")");
        printf(" b0=(");

        for (j = 0; j < HIDDEN_DIM; j++) {
            printf("%f", b0[j]);
            if (j < HIDDEN_DIM - 1) {
                printf(",");
            }
        }
        printf(")\n");

        // 2nd layer
        printf(" w1=(");
        for (j = 0; j < OUTPUT_DIM; j++) {
            for (i = 0; i < HIDDEN_DIM; i++) {
                printf("%f", w1[j][i]);
                if (i < HIDDEN_DIM - 1) {
                    printf(",");
                }
            }
            if (j < OUTPUT_DIM - 1) {
                printf(";");
            }
        }
        printf(")");
        printf(" b1=(");

        for (j = 0; j < OUTPUT_DIM; j++) {
            printf("%f", b1[j]);
            if (j < OUTPUT_DIM - 1) {
                printf(",");
            }
        }
        printf(")\n");
        fprintf(ofp, "%d\t%1.5f\n", t, loss_total);
    }


    fclose(ofp);
    return 0;
}
