#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *buf = (int*)malloc(sizeof(int) * 5);
    int idx = 0;
    for (int i=0; i<5; i++)
        buf[i] = 0;

    ++buf[idx];

    for (int i=0; i<5; i++)
        printf("buf[%d]=%d  ", i, buf[i]);
    printf("\n");

    free(buf);
    return 0;
}


