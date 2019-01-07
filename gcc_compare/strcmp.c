#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int wrap(const char *str1, const char *str2)
{
    const int res = strcmp(str1, str2);
    if (res < 0)
        return -1;
    if (res > 0)
        return 1;
    return 0;
}

int main()
{
    printf("%d\n", wrap("a", "b"));
    printf("%d\n", wrap("eplefjes", "b"));
    printf("%d\n", wrap("b", "eplefjes"));
    printf("%d\n", wrap("ananas", "ananas"));
    return 0;
}
