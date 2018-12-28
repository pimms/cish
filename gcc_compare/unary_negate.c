#include <stdio.h>

int foo()
{
    return 10;
}

int main()
{
    printf("%d\n", -foo());
    return 0;
}
