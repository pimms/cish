#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *buf = (int*)malloc(sizeof(int) * 5);

    for (int i=0; i<5; i++)
        buf[i] = 0;

    int idx = 0;
    buf[idx++]++;
    buf[++idx] += 100;
    buf[--idx] += buf[0];
    ++buf[idx];

    for (int i=0; i<5; i++)
        printf("buf[%d]: %d\n", i, buf[i]);

    return 0;
}
