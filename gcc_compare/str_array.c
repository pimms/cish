#include <stdio.h>
#include <stdlib.h>

int main()
{
    const char **strarr = (const char**)malloc(sizeof(void*) * 5);

    strarr[0] = "first string";
    strarr[1] = "second string";
    strarr[2] = "third string";
    strarr[3] = "fourth string";
    strarr[4] = "fifth string";

    for (int i=0; i<5; i++) {
        printf("str%d: %s\n", i, strarr[i]);
    }

    return 0;
}
