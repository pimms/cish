#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *buf = (char*)malloc(100);
    memset(buf, 0, 100);

    buf = strncpy(buf, "hi mom", 2);
    puts(buf);

    buf = strncpy(buf, "hi mom", 0);
    puts(buf);

    buf = strncpy(buf, "hi mom", 100);
    puts(buf);

    buf = strncpy(buf, "hi mom", 5);
    puts(buf);

    buf = strncpy(buf, "jada mamma", 2);
    puts(buf);

    free(buf);
    return 0;
}
