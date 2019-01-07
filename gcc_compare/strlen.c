#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int intVar = 0;

    printf("not a string: %d\n", (int)strlen((const char*)&intVar));
    printf("1: %d\n", (int)strlen("not a very long string"));
    printf("2: %d\n", (int)strlen("a"));
    printf("3: %d\n", (int)strlen(""));
    printf("4: %d\n", (int)strlen("also not a very long string but it might be, who knows"));

    char *buf = (char*)malloc(100);
    strcpy(buf, "copied string lol");
    printf("5: %d\n", (int)strlen(buf));
    free(buf);

    return 0;
}
