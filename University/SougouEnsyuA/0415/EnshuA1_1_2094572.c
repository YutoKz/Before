#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void pivot(int k, int n, double **x, double *y)
{
	double max_value = x[k][k];
	int max_line = k;

    int i,j;
	for(i=k+1; i<n; i++)
	{
		if(fabs(x[i][k]) > fabs(max_value))
		{
			max_value = x[i][k];
			max_line = i;
		}
	}
    if(max_line != k)
	{
		double temp;
		for(j=k; j<n; j++)
		{
			temp = x[k][j];
			x[k][j] = x[max_line][j];
			x[max_line][j] = temp;
		}
			temp = y[k];
			y[k] = y[max_line];
			y[max_line] = temp;
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;

	fp = fopen(argv[1], "r");

	if (fp == NULL)
	{
		printf("Cannot open file\n");
		return 1;
	}

	/* 変数の数を数える */
	int lines = 0;  /* linesに行数すなわち変数の数が格納 */
	char line[64];
	while(fgets(line, 64, fp) != NULL)
		lines++;
	rewind(fp);

	/* メモリの動的確保 */
    int i, j, k;
	double **a, *b;
	a = malloc(lines * sizeof(double*));
	for(i=0; i<lines; i++)
		a[i] = malloc(lines * sizeof(double));
	b = malloc(lines * sizeof(double));

	/* 動的に確保した配列に行列のデータ格納 */
	for(i=0; i<lines; i++)
	{
		/*
		fread(a[i], sizeof(double), lines, fp);
		fread((b+i), sizeof(double), 1, fp);
		*/
		for(j=0; j<lines; j++)
			fscanf(fp, "%lf ", &(a[i][j]));
		fscanf(fp, "%lf\n", (b+i));
	}

	fclose(fp);  /* 以上、配列aとbに方程式の情報格納完了 */

	/* 前進消去 */
	for(k=0; k<=(lines-2); k++)
	{
		/* 対角要素が０に近ければpivot操作 */
		if((a[k][k] > -0.00005) && (a[k][k] < 0.00005))
			pivot(k, lines, a, b);

		for(i=(k+1); i<lines; i++)
		{
            for(j=(k+1); j<lines; j++)
			    a[i][j] = a[i][j] - a[k][j]*(a[i][k]/a[k][k]);
			b[i] = b[i] - b[k]*(a[i][k]/a[k][k]);
		}
	}

	/* 後退代入 */
	double x[lines];
	x[lines-1] = b[lines-1] / a[lines-1][lines-1];
	for(i=(lines-2); i>=0; i--)
	{
		double sigma = 0;
		for(k=i+1; k<=(lines-1); k++)
		{
			sigma += a[i][k]*x[k];
			x[i] = (b[i] - sigma) / a[i][i];
		}
	}

    /* 答えの出力 */
	for(i=1; i<lines; i++)
		for(j=0; j<i; j++)
			a[i][j] = 0;

	for(i=0; i<lines; i++)
	{
		for(j=0; j<lines; j++)
			printf("%9.4f ", a[i][j]);
		printf(": %9.4f\n", b[i]);
	}
	for(i=0; i<lines; i++)
		printf("x%d = %.4f ", (i+1), x[i]);
	printf("\n");

    for(i=0; i<lines; i++)
        free(a[i]);
    free(b);
	return 0;
}
