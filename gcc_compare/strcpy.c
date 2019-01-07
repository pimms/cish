#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *buf1 = (char*)malloc(100);
    char *buf2 = (char*)malloc(100);

    buf1 = strcpy(buf1, "hei sveis");
    puts(buf1);

    strcpy(buf1 + 4, "mamma");
    puts(buf1);

    strcpy(buf2, buf1);
    printf("%s - %s\n", buf1, buf2);

    strcpy(buf1, "ops");
    printf("%s - %s\n", buf1, buf2);

    free(buf1);
    free(buf2);
    return 0;
}
