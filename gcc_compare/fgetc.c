#include <stdio.h>

int main()
{
    void *file = fopen("textfile.txt", "r");
    if (!file) {
        return 1;
    }

    char c = fgetc(file);
    while (c > 0) {
        printf("%c", c);
        c = fgetc(file);
    }

    // Open it again
    file = fopen("textfile.txt", "r");
    printf("%d\n", fgetc(file));
    fclose(file);
    printf("%d\n", fgetc(file));

    return 0;
}
