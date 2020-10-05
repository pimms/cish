// args These are my command line arguments. There are many like them, but these ones are mine.
#include <stdio.h>

int main(int argc, const char **argv)
{
    printf("%d params\n", argc);

    for (int i=1; i<argc; i++) {
        printf(" - %s\n", argv[i]);
    }

    return argc;
}
