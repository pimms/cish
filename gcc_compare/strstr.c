#include <string.h>
#include <stdio.h>

int main()
{
    printf("%s\n", strstr("here it is", "here"));
    printf("%s\n", strstr("here it is", "it"));
    printf("%s\n", strstr("here it is", "is"));
    printf("%s\n", strstr("here it is", ""));

    if (strstr("hi mom", "dad")) {
        printf("that's not right\n");
    }

    return 0;
}
