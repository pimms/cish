#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n = 0x3959384f;

    n += 1;
    printf("%x - %d\n", n, n);
    n -= 1;
    printf("%x - %d\n", n, n);
    n >>= 1;
    printf("%x - %d\n", n, n);
    n <<= 1;
    printf("%x - %d\n", n, n);
    n |= 0x000F; // big oof
    printf("%x - %d\n", n, n);
    n *= 2;
    printf("%x - %d\n", n, n);
    n /= 2;
    printf("%x - %d\n", n, n);
    n ^= 0x39429827;
    printf("%x - %d\n", n, n);
    n &= 0xF0F0EF;
    printf("%x - %d\n", n, n);

    return 0;
}

