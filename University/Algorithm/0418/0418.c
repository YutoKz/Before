#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int length(int n)
{
    int len = 0;
    int i = 1;
    while(n >= i)
    {
        len++;
        i *= 10;
    }
    return len;
}

int func(int n)
{
    int i, j;
    int ans = 0;
    int temp;

    for(i = 1; i <= n; i++)
    {
        int sum = 0;
        int k = i;

        for(j = 0; j < length(i); j++)
        {
            temp = k;
            k /= 10;
            sum += (temp - (k * 10));
        }

        if(sum % 2 == 0)
            ans += i;
    }

    return ans;
}







int main(void)
{
    printf("100   %d\n",func(100));    
    printf("1000  %d\n",func(1000));
    printf("10000 %d\n",func(10000));

    int i;
    for(i = 1000000; i <= 30000000; i += 1000000)
    {
        clock_t start, end;
        start = clock();
        func(i);
        end = clock();
        printf("%f\n", (double)(end-start)/CLOCKS_PER_SEC);
    }

    return 0;
}
