#include <stdio.h>
#include <stdlib.h>

int main()
{
    if (0 && 1) puts("no");
    if (0 || 1) puts("yes");
    if (1 || 0) puts("yes");
    if (0 || 0) puts("no");

    if (0 || (1 || 0)) puts("yes");
    if (0 || (0 || 1)) puts("yes");
    if (0 || (0 || 0)) puts("no");

    if (1 && (1 || 0)) puts("yes");
    if (1 && (1 || 0)) puts("yes");
    if (1 && (0 || 1)) puts("yes");
    if (1 && (0 || 0)) puts("no");

    return 0;
}
