#include <stdio.h>
#include <stdlib.h>

int main()
{
    if (0 && 1) puts("1");
    if (0 || 1) puts("2");
    if (1 || 0) puts("3");
    if (0 || 0) puts("4");

    if (0 || (1 || 0)) puts("5");
    if (0 || (0 || 1)) puts("6");
    if (0 || (0 || 0)) puts("7");

    if (1 && (1 || 0)) puts("8");
    if (1 && (1 || 0)) puts("9");
    if (1 && (0 || 1)) puts("10");
    if (1 && (0 || 0)) puts("11");

    return 0;
}
