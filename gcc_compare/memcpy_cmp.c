#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char *str1 = "abcdefghijklmnopqrstuvwxyz0";
    const char *str2 = "abcdefghijklmnopqrstuvwxyz1";

    printf("alphabet: %d\n", memcmp(str1, str2, 26));
    printf("full, 1: %d\n", memcmp(str1, str2, 27));
    printf("full, 2: %d\n", memcmp(str2, str1, 27));


    void *buf = malloc(28);
    memcpy(buf, str1, 28);

    printf("copied buf: %s\n", (const char*)buf);
    printf("equal to src: %d\n", memcmp(buf, str1, 27));
    printf("equal to other, 1: %d\n", memcmp(buf, str2, 27));
    printf("equal to other, 1: %d\n", memcmp(str2, buf, 27));

    free(buf);

    return 0;
}

