#include <stdio.h>
#include <stdlib.h>

const int N = 4;

int g_next = 938;
int prng()
{
    g_next += 3827;
    g_next %= 97;
    return g_next;
}

int main()
{
    int *buf = (int*)malloc(sizeof(int) * N);
    for (int i=0; i<N; i++)
        buf[i] = prng();

    printf("negative: ");
    for (int i=0; i<N; i++)
        printf("%d  ", -buf[i]);
    puts("");

    printf("logical negate: ");
    for (int i=0; i<N; i++)
        printf("%d  ", !buf[i]);
    puts("");

    printf("bitwise negate: ");
    for (int i=0; i<N; i++)
        printf("%d  ", ~buf[i]);
    puts("");

    printf("addrof-diff: ");
    for (int i=0; i<N; i++)
        printf("%d  ", (int)(&buf[i]) - (int)buf);
    puts("");

    short sh = 59;
    short *shPtr = &sh;
    printf("sizeof: %d\n", sizeof(shPtr[0]));

    free(buf);
    return 0;
}
