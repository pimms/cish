#include <stdio.h>

int main()
{
    const char *str = "This is my string!";
    const char *ptr = str;

    int sum = 0;
    while (*ptr) {
        sum += *ptr++;
    }

    printf("The sum of the chars of '%s' is %d\n", str, sum);
    return 0;
}
