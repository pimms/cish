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

    int *e0 = &buf[0];
    int *e1 = &buf[1];

    printf("e0 <-> e1: %d\n", ptrdiff(e0, e1));
    printf("e0 offset: %d\n", ptrdiff(buf, e0));
    printf("e1 offset: %d\n", ptrdiff(buf, e1));
    return 0;
}
