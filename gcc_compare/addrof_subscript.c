#include <stdio.h>
#include <stdlib.h>

int my_abs(int n)
{
    if (n<0)
        return -n;
    return n;
}

int ptrdiff(int *a, int *b)
{
    return my_abs((int)a - (int)b);
}

int main()
{
    int *buf = (int*)malloc(sizeof(int) * 5);

    // Fluff, only exists to help in debugging if
    // we have a bug (which we did at one point)
    for (int i=0; i<5; i++)
        buf[i] = (i+1) * 7;

    int *e0 = &buf[0];
    int *e1 = &buf[1];

    printf("e0 == buf: %d\n", e0 == buf);
    printf("e1 == (buf+1): %d\n", e0 == (buf+1));

    printf("e0 <-> e1: 0x%x\n", ptrdiff(e0, e1));
    printf("e0 offset: 0x%x\n", ptrdiff(buf, e0));
    printf("e1 offset: 0x%x\n", ptrdiff(buf, e1));
    return 0;
}
