#include <stdio.h>

void try_open(const char *path)
{
    if (fopen(path, "r") != 0) {
        printf("found file %s\n", path);

    } else {
        printf("failed to open file %s\n", path);
    }
}

int main()
{
    try_open("textfile.txt");
    try_open("lmao");

    void *file = fopen("textfile.txt", "r");
    printf("did open: %d\n", (file != NULL));
    printf("did close: %d\n", fclose(file));
    printf("did close again: %d\n", fclose(file));

    return 0;
}
