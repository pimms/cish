#include <stdio.h>
#include <stdbool.h>

int main()
{
    int negTrue = -true;
    int negFalse = -false;
    printf("negTrue: %d\n", negTrue);
    printf("negFalse: %d\n", negFalse);
    return 0;
}
