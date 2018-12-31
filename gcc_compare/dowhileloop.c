#include <stdio.h>
#include <stdlib.h>


int main()
{
    printf("while (0)\n");
    do {
        printf("should be printed once\n");
    } while (0);

    printf("while (n++ < 10)\n");
    int n = 0;
    do {
        printf("i: %d\n", n);
    } while (n++ < 10);

    printf("while (n++ <= 10)\n");
    n = 0;
    do {
        printf("i: %d\n", n);
    } while (n++ <= 10);

    printf("while (++n <= 10)\n");
    n = 0;
    do {
        printf("i: %d\n", n);
    } while (++n <= 10);

    printf("while (*ptr++)\n");
    const char *str = "hi mom";
    const char *ptr = str;
    do {
        printf("%c", *ptr);
    } while (*ptr++);

    return 0;
}

