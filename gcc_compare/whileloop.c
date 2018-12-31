#include <stdio.h>
#include <stdlib.h>


int g_num = 10;
int foo()
{
    return --g_num;
}


int main()
{
    int n = 0;

    printf("while (0)\n");
    while (0) {
        printf("no no no ");
    }

    printf("while (foo())\n");
    while (foo()) {
        printf("%d\n", n);
    }

    printf("while (n >= 0)\n");
    while (n >= 0) {
        n -= 1;
        printf("%d\n", --n);
    }

    printf("while (*ptr)\n");
    const char *str = "Hello, mother! How are you this blessed evening?";
    const char *ptr = str;
    while (*ptr) {
        printf("%c", *ptr++);
    }

    return 0;
}

