#include <stdlib.h>
#include <stdio.h>

int main()
{
    const char *n0 = "0";
    const char *n1 = "10";
    const char *n2 = "10054";
    const char *n3 = "-100";
    const char *n4 = "-100.594";
    const char *n5 = "";

    printf("n1: %d\n", atoi(n1));
    printf("n2: %d\n", atoi(n2));
    printf("n3: %d\n", atoi(n3));
    printf("n4: %d\n", atoi(n4));
    printf("n5: %d\n", atoi(n5));

    return atoi(n0);
}
