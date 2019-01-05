#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main()
{
    const char *str = "www.google.com";
    const int len = 14; // TODO: implement strlen... :)

    printf("%s\n", (const char*)memchr(str, '.', len));
    printf("%s\n", (const char*)memchr(str, 'o', len));
    printf("%s\n", (const char*)memchr(str, 'g', len));
    printf("%s\n", (const char*)memchr(str + 5, '.', len - 5));
    printf("%s\n", (const char*)memchr(str, 'o', len));
    printf("%s\n", (const char*)memchr(str, 'g', len));
    printf("is null: %d\n", (memchr(str, 'p', len) == NULL));
    printf("is null: %d\n", (memchr(str, 'w', 0) == NULL));
    printf("is null: %d\n", (memchr(str, 'w', 1) == NULL));
    printf("is null: %d\n", (memchr(str, 'g', 4) == NULL));
    printf("is null: %d\n", (memchr(str, 'w', -1) == NULL));

    return 0;
}

