#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *test;

int main(int argc, char const *argv[])
{
    if(test != NULL)
        printf("%p\n", test);
    else
        printf("fail\n");

    
}