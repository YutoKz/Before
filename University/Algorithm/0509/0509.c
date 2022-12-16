#include <stdio.h>
#include <stdlib.h>

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
                {
                    count++;
                    printf("(%3d,%3d,%3d)\n",a ,b ,c);
                }
            }
        }
    }

    return count;
}


int main(int argc, char const *argv[])
{
    printf("\n%s\t%3d\n", argv[1], abc(atoi(argv[1])));

    return 0;
}
