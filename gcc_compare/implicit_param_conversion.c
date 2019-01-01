#include <stdio.h>

int int_echo(int n)
{
    return n;
}

float float_echo(float n)
{
    return n;
}

int shrinkray(char ch)
{
    return ch;
}

int main()
{
    printf("implicitly converted float: %d\n", int_echo((float)100.385f));
    printf("implicitly converted double: %d\n", int_echo((double)198.385));
    printf("implicitly converted int: %g\n", float_echo((int)100));
    printf("shrinkray: %d\n", 0xFF00);
    printf("shrinkray: %d\n", 0x12);
    printf("shrinkray: %d\n", 0x8F);
    return 0;
}

