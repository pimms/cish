#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const int N = 100;

int main()
{
    char *str = (char*)malloc(N);
    memset(str, 0, N);

    puts(str);

    str = strcat(str, "hi");
    puts(str);
    str = strcat(str, " ");
    puts(str);
    str = strcat(str, "mom");
    puts(str);
    str = strcat(str, ", how u doin");
    puts(str);

    free(str);
    return 0;
}
