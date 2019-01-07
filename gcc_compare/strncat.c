#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    const int N = 200;

    char *str = (char*)malloc(N);
    memset(str, 0, N);

    char *buf = (char*)malloc(5);
    buf[4] = 0;

    for (int i=0; i<20; i++) {
        memset(buf, 'a' + i, 4);
        strncat(str, buf, (i % 4) + 1);
        puts(str);
    }


    free(str);
    free(buf);
    return 0;
}
