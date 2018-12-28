#include <stdio.h>

int main()
{
    int value = 100;
    int cool = *&*&*&*&value;
    if (cool == 100) {
        puts("that's swell");
    } else {
        puts("that's not swell");
    }

    return 0;
}
