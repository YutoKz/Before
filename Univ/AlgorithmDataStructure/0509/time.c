#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int abc(int n)
{
    int count = 0;
    int a, b, c;
    for(a = 1; a <= n; a++)
    {
        for(b = 1; b*b <= (a*a*a - 3); b++)
        {
            for(c = 1; c <= b; c++)
            {
                if(a*a*a == (b*b + 3*c))
                    count++;
            }
        }
    }

    return count;
}


int main(int argc, char const *argv[])
{
    // printf("\n%s\t%3d\n", argv[1], abc(atoi(argv[1])));
    
    int s = atoi(argv[1]);
    int e = atoi(argv[2]);
    int k = atoi(argv[3]);

    int i;
    
    for(i = s; i <= e; i += k)
    {
        clock_t start, end;
        start = clock();
        abc(i);
        end = clock();
        printf("%d, %f\n", i, (double)(end-start)/CLOCKS_PER_SEC);
    }

    return 0;
}
