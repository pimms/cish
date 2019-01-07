#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int wrap(const char *str1, const char *str2, int n)
{
    const int res = strncmp(str1, str2, n);
    if (res < 0)
        return -1;
    if (res > 0)
        return 1;
    return 0;
}

int main()
{
    printf("%d\n", wrap("a", "b", 100));
    printf("%d\n", wrap("eplefjes", "e", 5));
    printf("%d\n", wrap("eplefjes", "e", 1));
    printf("%d\n", wrap("eplefjes", "e", 0));

    printf("%d\n", wrap("www.google.com", "www.facebook.com", 4));
    printf("%d\n", wrap("www.google.com", "www.facebook.com", 5));
    printf("%d\n", wrap("www.google.com", "www.facebook.com", 1024));

    return 0;
}
