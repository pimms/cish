
#include <stdlib.h>
#include <stdio.h>

int main()
{
    const char *n0 = "0.53";
    const char *n1 = "10.99";
    const char *n2 = "10054.31";
    const char *n3 = "-100";
    const char *n4 = "-100.594";
    const char *n5 = "";

    printf("n1: %g\n", atof(n1));
    printf("n2: %g\n", atof(n2));
    printf("n3: %g\n", atof(n3));
    printf("n4: %g\n", atof(n4));
    printf("n5: %g\n", atof(n5));

    return (int)atoi(n0);
}
