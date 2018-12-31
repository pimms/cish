#include <stdio.h>
#include <stdlib.h>

const int BITS_IN_BYTE = 8;

void to_binary(int, char*);
void printn(int, char*);

int main()
{
    char *buf = (char*)malloc(100);
    printn(0xFF00FF00, buf);
    printn(0x12345678, buf);
    printn(0x80000000, buf);
    free(buf);
    return 0;
}

void printn(int n, char *buf)
{
    to_binary(n, buf);
    printf("%d in binary: %s\n", n, buf);
}

void to_binary(int n, char *buf)
{
    const int bits = sizeof(n) * BITS_IN_BYTE;
    int stridx = bits;

    buf[stridx--] = 0;

    for (int i=0; i<bits; i++) {
        const int bit = n >> i & 1;
        buf[stridx--] = '0' + bit;
    }
}

