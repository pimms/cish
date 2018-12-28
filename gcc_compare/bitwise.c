#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n = 0;
    printf("0x%x\n", n);

    n |= 0x11002200;
    printf("0x%x\n", n);

    char *ptr = (char*)&n;
    ptr[1] |= 0x51;
    printf("0x%x\n", n);

    ptr[3] <<= 1;
    printf("0x%x\n", n);

    ptr[2] = ptr[2]^ptr[1];
    printf("0x%x\n", n);

    n &= n;
    printf("0x%x\n", n);

    return 0;
}
