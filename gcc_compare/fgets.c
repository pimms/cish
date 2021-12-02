#include <stdio.h>
#include <stdlib.h>

int main()
{
    void *file = fopen("textfile.txt", "r");
    if (!file) {
        return 1;
    }

    char *buf = (char*)malloc(20);

    while (fgets(buf, 20, file)) {
        printf("'%s' ", buf);
    }

    free(buf);
    return 0;
}
