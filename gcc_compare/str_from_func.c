#include <stdio.h>
#include <stdlib.h>

const char *literal_return()
{
    return "Hi mom";
}

const char *global_string = "hi, I'm global";
const char *global_return()
{
    return global_string;
}

char *malloc_return()
{
    // CISH doesn't support strcpy() yet, lol
    char *str = (char*)malloc(10);
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = ' ';
    str[6] = 'm';
    str[7] = 'o';
    str[8] = 'm';
    str[9] = 0;

    return str;
}

int main()
{
    puts(literal_return());
    puts(global_return());

    char *str = malloc_return();
    puts(str);
    free(str);

    return 0;
}
