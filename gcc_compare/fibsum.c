#include <stdio.h>
#include <stdlib.h>

const int NUM = 100;

int main()
{
    int *buf = (int*)malloc(sizeof(int) * NUM);

    buf[0] = 1;
    buf[1] = 1;

    for (int i=2; i<NUM; i++) {
        buf[i] = buf[i-1] + buf[i-2];
    }

    int sum = 0;
    for (int i=0; i<NUM; i++) {
        sum += buf[i];
    }

    printf("Sum of the first %d fibonacci numbers: %d\n", NUM, sum);
    return 0;
}
