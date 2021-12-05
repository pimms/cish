#include <stdio.h>

/**
 * This comparison exists to ensure that AND-expressions
 * do not evaluate terms if the preceeding terms are
 * false.
 *
 * This was a bug discovered December 4th 2021.
 */
int main()
{
    const char *str = "this is a valid string";
    const char *nullstr = NULL;

    if (str && *str == 't') {
        puts("all good");
    }

    if (nullstr && *nullstr == 't') {
        puts("shouldn't happen");
    } else {
        puts("all good");
    }

    return 0;
}
